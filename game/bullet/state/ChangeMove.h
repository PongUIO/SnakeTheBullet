#ifndef CHANGEMOVE_H
#define CHANGEMOVE_H

#include "../bstate.h"

class ChangeMove : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b);
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
		
	private:
		double mAngleOff;
		double mSpdOff;
		double mFullTime;
		
		enum Style {
			AlterSpeed = 	(1<<0),
			AlterAngle = 	(1<<1),
		};
		
		int mStyle;
};

#endif
