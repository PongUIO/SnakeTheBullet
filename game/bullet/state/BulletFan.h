#ifndef BULLETFAN_H
#define BULLETFAN_H

#include "../bstate.h"

class BulletFan : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b);
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
		
	private:
		double mAngle;
		double mBaseSpeed;
		int mCount;
		
		bool mInheritVel;
};

#endif
