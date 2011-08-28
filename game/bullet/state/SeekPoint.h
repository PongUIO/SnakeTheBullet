#ifndef SEEKPOINT_H
#define SEEKPOINT_H

#include "../bstate.h"

class SeekPoint : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b);
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
		
	private:
		double mTargetX, mTargetY;
		double mSpeed;
		
		bool mSeekRandom;
};

#endif
