#define GL_GLEXT_PROTOTYPES
#include <math.h>

#include "brule.h"
#include "bullet.h"

#include <GL/glext.h>

#define ATTEMPTS 5
#define STATE_ATTEMPTS 2
#define PI 3.1415927

BulletRule::~BulletRule()
{
	for(int i=0; i<mStateVec.size(); i++)
		glDeleteBuffers(2, mStateVec[i].bufferGl);
}


BulletRule* BulletRule::designRule(double complexity)
{
	BulletRule *best = NULL;
	
	for(int i=0; i<ATTEMPTS; i++) {
		BulletRule *rule = new BulletRule();
		
		rule->generate(complexity);
		
		if(best == NULL)
			best = rule;
		else if( fabs(rule->getComplexity()-complexity) < fabs(best->getComplexity()-complexity) ) {
			delete best;
			best = rule;
		}
		
		if( fabs(best->getComplexity()-complexity) < 1.0 )
			break;
	}
	
	return best;
}

void BulletRule::generate(double complexity)
{
	int numStates = calcNumRules(complexity);
	mComplexity = 0.0;
	mLength = 0.0;
	
	for(int i=0; i<numStates; i++) {
		double stateComplexity = (complexity-mComplexity)/double(numStates-i);
		
		mStateVec.push_back( constructState(stateComplexity) );
		mComplexity += mStateVec.back().computeComplexity();
		mLength += mStateVec.back().length;
	}
	
	// Append a final state that does nothing
	State state;
	state.generate(0.0);
	mStateVec.push_back( state );
	mLength += mStateVec.back().length;
}

/**
 * Constructs a state aiming for a specific complexity.
 */
BulletRule::State BulletRule::constructState(double idealComplexity)
{
	bool isFirst=true;
	State best, state;
	
	for(int i=0; i<STATE_ATTEMPTS; i++) {
		state = State();
		state.generate(idealComplexity);
		
		if(isFirst) {
			best = state;
			isFirst=false;
		} else if( fabs(state.computeComplexity()-idealComplexity) < fabs(best.computeComplexity()-idealComplexity) ) {
			best = state;
		}
	}
	return state;
}

void BulletRule::State::generate(double complexity)
{
	// Generate logic
	if(complexity > 0.0)
		switchType = CircleSpawn;
		//switchType = (SwitchType)mdBullet::random(0,MaxSwitch-1);
	else
		switchType = IdleRule;
	
	switch(switchType)
	{
		case CircleSpawn:
			iparam[0] = mdBullet::random(3,3+sqrt(complexity) );
			fparam[0] = mdBullet::drandi(0.025*sqrt(complexity), 0.15*sqrt(complexity));
			break;
			
		case RotateAngle:
			fparam[0] = mdBullet::drandi(0, PI);
			break;
			
		case MoveRandom:
			fparam[0] = mdBullet::drandi(0.25*complexity, 1.5*complexity);
			break;
		
		default:
			break;
	}
	if(complexity <= 0.0) {
		length = mdBullet::drandi(1.0, 2.0);
	} else {
		length = mdBullet::drandi(3.0, 15.0-complexity*2.0);
		if(length < 3.0)
			length = 3.0;
	}
	
	double realComplexity = computeComplexity();
	
	// Generate visuals
	{
		if(realComplexity < 1.0)
			realComplexity = 1.0;
		
		int numFragments = mdBullet::random(2,3+log(1+realComplexity));
		int numVertex = 0;
		
		for(int i=0; i<numFragments; i++) {
			double size = realComplexity / double(i+1);
			size *= 0.015;
			
			RenderFragment frag;
			frag.r = mdBullet::drandi(0.1,1.0);
			frag.g = mdBullet::drandi(0.1,1.0);
			frag.b = mdBullet::drandi(0.1,1.0);
			
			frag.moveOffset = i==0 ? 0.0 : mdBullet::drandi(0.0, size*0.5 * double(i)/double(numFragments));
			frag.baseAngle = mdBullet::drandi(0.0, 2.0*PI);
			
			frag.spikes.resize(mdBullet::random(1,2+log(1+realComplexity)));
			for(int i=0; i<frag.spikes.size(); i++) {
				RenderFragment::Spike &spike = frag.spikes[i];
				
				spike.radius = mdBullet::drandi(size*0.25, size*2.0);
				
				spike.r = mdBullet::drandi(0.1, 1.0);
				spike.g = mdBullet::drandi(0.1, 1.0);
				spike.b = mdBullet::drandi(0.1, 1.0);
			}
			
			frag.numPoints = mdBullet::random(4,5+log(1+realComplexity*realComplexity) );
			
			mFigures.push_back(frag);
			
			// 1 center point + numPoints edges.
			numVertex += 1+frag.numPoints;
		}
		
		glGenBuffers(2, bufferGl);
		glBindBuffer(GL_ARRAY_BUFFER, bufferGl[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferGl[1]);
		
		// numFragments * (root-vertex (xy-rgb) + numPoints*(xy-rgb))
		Vertex data[ numVertex ];
		GLushort index[ numVertex ];
		
		int cv = 0;
		for(int f=0; f<numFragments; f++) {
			RenderFragment &frag = mFigures[f];
			
			data[cv].x = 0.0;
			data[cv].y = 0.0;
			data[cv].r = frag.r;
			data[cv].g = frag.g;
			data[cv].b = frag.b;
			
			index[cv] = cv;
			cv++;
			
			for(int p=0; p<mFigures[f].numPoints; p++) {
				RenderFragment::Spike &spike = frag.spikes[p%frag.spikes.size()];
				
				double curAngle = frag.baseAngle + (p / double(frag.numPoints) ) *2.0*PI;
				double radius = spike.radius;
				
				double xPos = cos(frag.baseAngle)*frag.moveOffset + cos(curAngle)*radius;
				double yPos = sin(frag.baseAngle)*frag.moveOffset + sin(curAngle)*radius;;
				
				printf("%g %g\n", xPos, yPos);
				data[cv].x = xPos;
				data[cv].y = yPos;
				data[cv].r = spike.r;
				data[cv].g = spike.g;
				data[cv].b = spike.b;
				index[cv] = cv;
				cv++;
			}
		}
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertex, data, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numVertex, index, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

/**
 * Calculates the ideal number of rules for a given complexity.
 */
int BulletRule::calcNumRules(double complexity)
{	return mdBullet::random(1, 1+log(1+complexity) ); }

/**
 * Computes the complexity of a given input state.
 */
double BulletRule::State::computeComplexity()
{
	double tmp = 0.0;
	
	switch(switchType)
	{
		case CircleSpawn:
			tmp = 1.0 + iparam[0]*0.25*(fparam[0]/2.0);
			break;
			
		case RotateAngle:
			tmp = 0.25;
			break;
			
		case MoveRandom:
			tmp = 0.75 + fparam[0]/2.0;
			break;
		
		case ChangeSpeed:
			tmp = 0.1 + fparam[0];
			break;
		
		case IdleRule:
			tmp = 0.0;
			break;
		
		default:
			tmp = 10;
			break;
	}
	
	return tmp;
}
