#ifndef CIRCLESPAWN_H
#define CIRCLESPAWN_H

#include "../bstate.h"

class CircleSpawn : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b);
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
		
	private:
		void spawnBullets(Bullet *b, int start, int end);
		
		int mBulletCount;
		double mBaseSpeed;
		
		bool mInstant;
		double mStartTime;
		
		bool mFreeze;
};

#endif
