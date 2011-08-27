#ifndef MOVERANDOM_H
#define MOVERANDOM_H

#include "../bstate.h"

class MoveRandom : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b) {}
		void process(Bullet *b, double delta) {}
		void finishCall(Bullet *b);
		
	private:
		double mBaseSpeed;
};

#endif
