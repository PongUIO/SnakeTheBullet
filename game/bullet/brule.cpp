#define GL_GLEXT_PROTOTYPES
#include <math.h>

#include "brule.h"
#include "bullet.h"

#include "state/CircleSpawn.h"
#include "state/IdleState.h"
#include "state/BulletFan.h"
#include "state/MoveRandom.h"
#include "state/SeekPoint.h"

#include <GL/glext.h>

#define ATTEMPTS 8
#define STATE_ATTEMPTS 4

BulletRule::~BulletRule()
{
	for(int i=0; i<mStateVec.size(); i++) {
		delete mStateVec[i];
	}
}

State::~State()
{
	glDeleteBuffers(2, bufferGl);
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
		if(stateComplexity <= 0.0)
			stateComplexity = 0.0;
		
		mStateVec.push_back( constructState(stateComplexity) );
		mComplexity = 0.0;
		for(int j=mStateVec.size()-1; j>=0; j--)
			mComplexity += mStateVec[j]->computeComplexity(mComplexity);
		
		mLength += mStateVec.back()->duration;
	}
	
	// Append a final state that does nothing
	State *state = new IdleState();
	state->setup(0.0);
	mStateVec.push_back( state );
	mLength += mStateVec.back()->duration;
}

/**
 * Constructs a state aiming for a specific complexity.
 */
State *BulletRule::constructState(double idealComplexity)
{
	State *best=NULL;
	State *state;
	
	for(int i=0; i<STATE_ATTEMPTS; i++) {
		int type;
		if(idealComplexity > 0.0)
			type = mdBullet::random(SwIdleRule+1,MaxSwitch-1);
		else
			type = SwIdleRule;
		
		switch(type)
		{
			case SwIdleRule: state = new IdleState(); break;
			case SwCircleSpawn: state = new CircleSpawn(); break;
			case SwMoveRandom: state = new MoveRandom(); break;
			case SwSeekPoint: state = new SeekPoint(); break;
			case SwFan: state = new BulletFan(); break;
		}
		state->setup(idealComplexity);
		
		if(best == NULL)
			best = state;
		else if( fabs(state->computeComplexity(0.0)-idealComplexity) < fabs(best->computeComplexity(0.0)-idealComplexity) ) {
			delete best;
			best = state;
		}
	}
	return state;
}

void State::setup(double cplx)
{
	generate(cplx);
	
	double realCplx = computeComplexity(0.0);
	
	// Generate visuals
	{
		if(realCplx < 1.0)
			realCplx = 1.0;
		
		int numFragments = mdBullet::random(2,3+log(1+realCplx));
		int numVertex = 0;
		
		for(int i=0; i<numFragments; i++) {
			double size = sqrt(realCplx) / sqrt(double(i+1));
			size *= 0.015;
			
			RenderFragment frag;
			frag.moveOffset = i==0 ? 0.0 : mdBullet::drandi(0.0, size*0.5 * double(i)/double(numFragments));
			frag.baseAngle = mdBullet::drandi(0.0, 2.0*PI);
			
			frag.spikes.resize(mdBullet::random(1,2+log(1+realCplx)));
			for(int i=0; i<frag.spikes.size(); i++) {
				RenderFragment::Spike &spike = frag.spikes[i];
				
				spike.radius = mdBullet::drandi(size*0.1, size*1.5);
				
				spike.r = mdBullet::drandi(0.1, 1.0);
				spike.g = mdBullet::drandi(0.1, 1.0);
				spike.b = mdBullet::drandi(0.1, 1.0);
			}
			
			frag.numVertex = mdBullet::random(4,5+log(1+realCplx*realCplx) );
			
			mFigures.push_back(frag);
			
			// 1 center point + numPoints edges.
			numVertex += 2+frag.numVertex;
		}
		
		glGenBuffers(2, bufferGl);
		glBindBuffer(GL_ARRAY_BUFFER, bufferGl[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferGl[1]);
		
		// numFragments * (root-vertex (xy-rgb) + numPoints*(xy-rgb))
		BulletRule::Vertex data[ numVertex ];
		GLushort index[ numVertex ];
		
		int cv = 0;
		for(int f=0; f<numFragments; f++) {
			RenderFragment &frag = mFigures[f];
			
			int first=cv;
			data[cv].x = 0.0;
			data[cv].y = 0.0;
			data[cv].r = mdBullet::drandi(0.1,1.0);
			data[cv].g = mdBullet::drandi(0.1,1.0);
			data[cv].b = mdBullet::drandi(0.1,1.0);
			
			index[cv] = cv;
			cv++;
			
			for(int p=0; p<mFigures[f].numVertex; p++) {
				RenderFragment::Spike &spike = frag.spikes[p%frag.spikes.size()];
				
				double curAngle = frag.baseAngle + (p / double(frag.numVertex) ) *2.0*PI;
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
			
			data[cv] = data[first+1];
			index[cv] = cv;
			cv++;
		}
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(BulletRule::Vertex)*numVertex, data, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numVertex, index, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

/*void BulletRule::State::generate(double complexity)
{
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
	
	
}*/

/**
 * Calculates the ideal number of rules for a given complexity.
 */
int BulletRule::calcNumRules(double complexity)
{	return mdBullet::random(1, 1+log(1+pow(complexity,1.5)) ); }

/**
 * Computes the complexity of a given input state.
 */
/*double BulletRule::State::computeComplexity(double prevComplexity)
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
*/
