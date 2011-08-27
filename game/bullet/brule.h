#ifndef BRULE_H
#define BRULE_H

#include <vector>
#include <GL/gl.h>

#define MAX_PARAM 4

class BulletRule {
	public:
		BulletRule() : mReferences(0), mComplexity(0.0) {}
		~BulletRule();
		
		static BulletRule *designRule(double complexity);
		
		double getComplexity() { return mComplexity; }
		double getLength() { return mLength; }
		
		void ref() { mReferences++; }
		void unref() {
			mReferences--;
			if(mReferences <= 0)
				delete this;
		}
		
		/**
		* When a movement phase is over,
		* a switchtype is activated.
		*/
		enum SwitchType {
			IdleRule=0,
			CircleSpawn, // Spawns a circle of bullets (iparam[0]=numBullets, fparam[0]=baseSpeed)
			RotateAngle, // Rotates the active velocity of the bullet (fparam[0]=angle)
			MoveRandom,  // Moves in a random direction
			ChangeSpeed, // Alters the speed of the rule
			
			MaxSwitch
		};
		
		struct Vertex {
			GLfloat x,y;
			GLfloat r,g,b;
		};
		
		struct RenderFragment {
			float r,g,b;
			float baseAngle;
			float moveOffset;
			
			struct Spike {
				float r,g,b;
				
				float radius;
			};
			typedef std::vector<Spike> SpikeVec;
			SpikeVec spikes;
			
			int numPoints;
		};
		
		struct State {
			SwitchType switchType;
			
			/// Switch parameters
			int iparam[MAX_PARAM];
			double fparam[MAX_PARAM];
			
			double length;
			
			typedef std::vector<RenderFragment> FragmentVec;
			FragmentVec mFigures;
			
			GLuint bufferGl[2];
			int numPts;
			
			double computeComplexity();
			
			void generate(double complexity);
		};
		
		typedef std::vector<State> StateVec;
		StateVec mStateVec;
		
		State &getState(int id) { return mStateVec[id]; }
		int numStates() { return mStateVec.size(); }
		
	private:
		void generate(double complexity);
		
		static int calcNumRules(double complexity);
		static State constructState(double idealComplexity);
		
		int mReferences; ///< Number of bullets that reference this rule
		double mComplexity; ///< Real calculated complexity
		double mLength; /// < The length of the rule
};

#endif
