#ifndef BULLETFAN_H
#define BULLETFAN_H

#include <vector>
#include "../bstate.h"

class BulletFan : public State {
	public:
		double computeComplexity(double prev);
		void generate(double cplx);
		
		void activate(Bullet *b);
		void process(Bullet *b, double delta);
		void finishCall(Bullet *b);
		
	private:
		struct Fan {
			double mMinAngle;
			double mMaxAngle;
			
			double mBaseSpeed;
			int mCount;
			int mFanCount;
			
			double mInterval;
			
			bool mIsStraight;
			bool mInheritVel;
			
			double computeComplexity();
		};
		
		typedef std::vector<Fan> FanVec;
		
		FanVec mFans;
		int mBulletSum;
		
		/*double mAngle;
		double mBaseSpeed;
		int mCount;
		
		bool mInheritVel;*/
};

#endif
