#define GL_GLEXT_PROTOTYPES
#include <math.h>

#include "brule.h"
#include "bullet.h"

#include <GL/glext.h>

#define ATTEMPTS 8
#define STATE_ATTEMPTS 4
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
	
	for(int i=0; i<best->mStateVec.size(); i++)
		printf("%d ", best->mStateVec[i].switchType);
	printf("\n");
	
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
		mComplexity = 0.0;
		for(int j=mStateVec.size()-1; j>=0; j--)
			mComplexity += mStateVec[j].computeComplexity(mComplexity);
		
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
		switchType = (SwitchType)mdBullet::random(IdleRule+1,MaxSwitch-1);
	else
		switchType = IdleRule;
	
	length = 0.0;
	switch(switchType)
	{
		case CircleSpawn:
			iparam[0] = mdBullet::random(3,3+sqrt(complexity) );
			fparam[0] = mdBullet::drandi(0.015*sqrt(complexity), 0.05*sqrt(complexity));
			break;
			
		case RotateAngle:
			fparam[0] = mdBullet::drandi(0, PI);
			fparam[1] = mdBullet::drandi(0.015*sqrt(complexity), 0.05*sqrt(complexity));
			break;
			
		case MoveRandom:
			fparam[0] = mdBullet::drandi(0.015*sqrt(complexity), 0.05*sqrt(complexity));
			break;
		
		case ChangeSpeed:
			fparam[0] = mdBullet::drandi(0.015*sqrt(complexity), 0.05*sqrt(complexity));
			break;
		
		case SeekPoint:
			fparam[0] = mdBullet::drandi(-0.9,0.9);
			fparam[1] = mdBullet::drandi(-0.9, 0.9);
			fparam[2] = mdBullet::drandi(0.025*sqrt(complexity), 0.1*sqrt(complexity));
			length = 2.0;
			break;
		
		case Fan:
			fparam[0] = mdBullet::drandi(0.0, 2.0*PI);
			fparam[1] = mdBullet::drandi(0.015*sqrt(complexity), 0.05*sqrt(complexity));
			iparam[0] = mdBullet::random(1, 1+sqrt(complexity));
			break;
		
		default:
			break;
	}
	
	double realComplexity = computeComplexity();
	
	if(complexity <= 0.0) {
		length = mdBullet::drandi(1.0, 2.0);
	} else {
		length += mdBullet::drandi(3.0*sqrt(realComplexity), (15.0-complexity*2.0)*sqrt(realComplexity));
		if(length < 3.0)
			length = 3.0;
	}
	
	realComplexity = computeComplexity();
	
	// Generate visuals
	{
		if(realComplexity < 1.0)
			realComplexity = 1.0;
		
		int numFragments = mdBullet::random(2,3+log(1+realComplexity));
		int numVertex = 0;
		
		for(int i=0; i<numFragments; i++) {
			double size = sqrt(realComplexity) / sqrt(double(i+1));
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
				
				spike.radius = mdBullet::drandi(size*0.75, size*1.5);
				
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
{	return mdBullet::random(1, 1+log(1+pow(complexity,1.5)) ); }

/**
 * Computes the complexity of a given input state.
 */
double BulletRule::State::computeComplexity(double prevComplexity)
{
	double tmp = 0.0;
	
	switch(switchType)
	{
		case CircleSpawn:
			tmp = (1.0+prevComplexity) * 1.5 * (1.0 + iparam[0]*0.25*(fparam[0]/2.0));
			break;
			
		case RotateAngle:
			tmp = prevComplexity+0.125;
			break;
			
		case MoveRandom:
 			tmp = (1.0+prevComplexity) * 2.0 * (1.0 + fparam[0]/2.0);
			break;
		
		case ChangeSpeed:
			tmp = prevComplexity + 0.05 + 0.5*fparam[0];
			break;
		
		case SeekPoint:
			tmp = prevComplexity + 0.125 + 0.5*fparam[2];
			break;
		
		case Fan:
			tmp = (1.0+prevComplexity) * (iparam[0]);
			break;
		
		case IdleRule:
			tmp = prevComplexity;
			break;
		
		default:
			tmp = 10;
			break;
	}
	
	return tmp;
}
