#ifndef GRAVITY_H
#define GRAVITY_H

#include <vector>
#include "../bstate.h"

class Gravity : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b);
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
		
	private:
		double mGravAngle;
		double mGravDist;
		double mGravStrength;
		bool mInheritAngle;
};

#endif
