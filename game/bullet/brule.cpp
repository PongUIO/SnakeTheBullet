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

#define ATTEMPTS 6
#define STATE_ATTEMPTS 6

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
		printf("\tComplexity: %g\n", rule->getComplexity());
		
		if(best == NULL)
			best = rule;
		else if( fabs(rule->getComplexity()-complexity) < fabs(best->getComplexity()-complexity) ) {
			delete best;
			best = rule;
		} else {
			delete rule;
		}
		
		if( fabs(best->getComplexity()-complexity) < 1.0 )
			break;
	}
	
	printf("Best rule: %g (ideal %g)\n", best->getComplexity(), complexity);
	printf("Rule state size: %d\n", best->mStateVec.size());
	
	return best;
}

void BulletRule::generate(double complexity)
{
	int numStates = calcNumRules(complexity);
	mComplexity = 0.0;
	mLength = 0.0;
	
	// Append a final state that does nothing
	State *state = new IdleState(false);
	state->setup(0.0);
	mStateVec.push_back( state );
	mLength += mStateVec.back()->duration;
	
	int stateCount=0;
	while(mComplexity < complexity && stateCount<numStates) {
		double stateComplexity = (complexity-mComplexity)/double(numStates-stateCount);
		if(stateComplexity <= 0.0)
			break;
		
		mStateVec.push_back( constructState(stateComplexity) );
		mComplexity = 0.0;
		for(int j=0; j<mStateVec.size(); j++) {
			mComplexity = mStateVec[j]->computeComplexity(mComplexity);
		}
		
		mLength += mStateVec.back()->duration;
		stateCount++;
	}
	
	// Append a final state that does nothing
	state = new IdleState(true);
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
		
		if(mdBullet::random(0,12) == 0)
			type = SwIdleRule;
		
		switch(type)
		{
			case SwIdleRule: state = new IdleState(false); break;
			case SwCircleSpawn: state = new CircleSpawn(); break;
			case SwMoveRandom: state = new MoveRandom(); break;
			case SwSeekPoint: state = new SeekPoint(); break;
			case SwFan: state = new BulletFan(); break;
		}
		state->setup(idealComplexity);
		
		if(best == NULL)
			best = state;
		else if( fabs(state->computeComplexity(1.0)-idealComplexity) < fabs(best->computeComplexity(1.0)-idealComplexity) ) {
			delete best;
			best = state;
		} else {
			delete state;
		}
	}
	return best;
}

struct RenderFragment {
	float baseAngle;
	float moveOffset;
	
	struct Spike {
		float r,g,b;
		float radius;
	};
	
	typedef std::vector<Spike> SpikeVec;
	SpikeVec spikes;
	
	int numVertex;
};

void State::setup(double cplx)
{
	generate(cplx);
	
	double realCplx = computeComplexity(1.0);
	
	// Generate visuals
	{
		if(realCplx < 1.0)
			realCplx = 1.0;
		
		int numFragments = mdBullet::random(2,3+log(1+realCplx));
		int numVertex = 0;
		
		typedef std::vector<RenderFragment> FragmentVec;
		FragmentVec fragments;
		
		mScale = 0.01;
		
		for(int i=0; i<numFragments; i++) {
			double size = sqrt(sqrt(realCplx / double(i+1)));
			
			RenderFragment frag;
			frag.moveOffset = i==0 ? 0.0 : mdBullet::drandi(0.0, size);
			frag.baseAngle = mdBullet::drandi(0.0, 2.0*PI);
			
			frag.spikes.resize(mdBullet::random(1,2+log(1+2.0*realCplx)));
			for(int i=0; i<frag.spikes.size(); i++) {
				RenderFragment::Spike &spike = frag.spikes[i];
				
				spike.radius = mdBullet::drandi(size*0.1, size*1.5);
				
				spike.r = mdBullet::drandi(0.1, 1.0);
				spike.g = mdBullet::drandi(0.1, 1.0);
				spike.b = mdBullet::drandi(0.1, 1.0);
			}
			
			frag.numVertex = mdBullet::random(3,4+log(1+1.25*realCplx*realCplx) );
			
			fragments.push_back(frag);
			
			mFlowers.push_back( RenderFlower(frag.numVertex+2) );
			
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
			RenderFragment &frag = fragments[f];
			
			int first=cv;
			data[cv].x = cos(frag.baseAngle) * frag.moveOffset;
			data[cv].y = sin(frag.baseAngle) * frag.moveOffset;
			data[cv].r = mdBullet::drandi(0.1,1.0);
			data[cv].g = mdBullet::drandi(0.1,1.0);
			data[cv].b = mdBullet::drandi(0.1,1.0);
			
			index[cv] = cv;
			cv++;
			
			for(int p=0; p<fragments[f].numVertex; p++) {
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

/**
 * Calculates the ideal number of rules for a given complexity.
 */
int BulletRule::calcNumRules(double complexity)
{	return mdBullet::random(1, 3+log(1+pow(complexity,1.0)) ); }
