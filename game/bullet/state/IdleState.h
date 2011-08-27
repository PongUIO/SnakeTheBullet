#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "../bstate.h"

class IdleState : public State {
	public:
		double computeComplexity(double prev) { return prev; }
		void generate(double cplx);
		
		void activate(Bullet *b) {}
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
};

#endif
