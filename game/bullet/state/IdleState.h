#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "../bstate.h"

class IdleState : public State {
	public:
		IdleState(bool endDeath);
		
		double computeComplexity(double prev) { return 1.0+prev; }
		void generate(double cplx);
		
		void activate(Bullet *b) {}
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
		
	private:
		bool mDie;
};

#endif
