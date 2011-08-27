#ifndef CIRCLESPAWN_H
#define CIRCLESPAWN_H

#include "../bstate.h"

class CircleSpawn : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b) {}
		void process(Bullet *b, double delta) {}
		void finishCall(Bullet *b);
		
	private:
		int mBulletCount;
		double mBaseSpeed;
};

#endif
