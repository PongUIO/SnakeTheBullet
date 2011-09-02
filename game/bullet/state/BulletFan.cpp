#include <algorithm>
#include "BulletFan.h"
#include "../bullet.h"

void BulletFan::generate(double cplx)
{
	duration = mdBullet::drandi(4.5, 8.0);
	mBulletSum = 0;
	
	int fanCount = mdBullet::random(0, 4);
	fanCount = 1+fanCount/mdBullet::random(1,8);
	
	for(int i=0; i<fanCount; i++) {
		Fan fan;
		
		fan.mIsStraight = mdBullet::random(0,4);
		fan.mMinAngle = mdBullet::drandi(-PI, PI);
		if(!fan.mIsStraight) {
			fan.mMaxAngle = mdBullet::drandi(-PI, PI);
			
			if(fan.mMinAngle > fan.mMaxAngle)
				std::swap(fan.mMinAngle, fan.mMaxAngle);
		}
		
		fan.mBaseSpeed = mdBullet::drandi(0.005*(1.0+sqrt(cplx)), 0.06*(1.0+0.4*sqrt(cplx)));
		fan.mCount = mdBullet::random(2, 2+(1.0+pow(cplx,0.75)));
		mBulletSum += fan.mCount;
		fan.mFanCount = mdBullet::random(1, 1+(fan.mCount-1)/2);
		
		fan.mInheritVel = !mdBullet::random(0,3);
		
		fan.mInterval = duration / double(fan.mCount) * 0.99;
		
		mFans.push_back(fan);
	}
}

void BulletFan::activate(Bullet* b)
{
	b->fparam[0] = 0.0;
	b->iparam[0] = mBulletSum;
}

void BulletFan::process(Bullet* b, double delta)
{
	double prevTime = b->fparam[0];
	b->fparam[0] += delta;
	double curTime = b->fparam[0];
	
	for(FanVec::iterator i=mFans.begin(); i!=mFans.end(); i++) {
		Fan &fan = *i;
		
		int prevIndex, curIndex;
		prevIndex = int( prevTime / fan.mInterval );
		curIndex = int( curTime / fan.mInterval );
		for(int cI=prevIndex; cI< curIndex; cI++) {
			Bullet::Config cfg = Bullet::Config(b->x,b->y, 0.0,0.0, b->getRule(), b->getActiveState()+1);
			
			double angle;
			if(fan.mIsStraight)
				angle = fan.mMinAngle;
			else
				angle = fan.mMinAngle +
					(double(cI%fan.mFanCount) / double(fan.mFanCount)) *
					(fan.mMaxAngle-fan.mMinAngle);
			
			cfg.vx = cos(b->initialAngle+angle) * fan.mBaseSpeed;
			cfg.vy = sin(b->initialAngle+angle) * fan.mBaseSpeed;
			
			if(fan.mInheritVel) {
				cfg.vx += b->vx;
				cfg.vy += b->vy;
			}
			
			modBullet.create(cfg);
			b->iparam[0]--;
		}
		
		b->scale = 0.2 + 0.8*(double(b->iparam[0]) / double(mBulletSum));
	}
}

void BulletFan::finishCall(Bullet* b)
{
	b->kill();
}

double BulletFan::Fan::computeComplexity()
{
	double straightMult = mIsStraight ? 1.0 : 2.0;
	
	double res = (mBaseSpeed/0.08)*pow(mCount,1.25)*straightMult;
	return res;
}

double BulletFan::computeComplexity(double prev)
{
	double fanSum = 0.0;
	for(FanVec::iterator i=mFans.begin(); i!=mFans.end(); i++) {
		fanSum += (*i).computeComplexity();
	}
	
	double res = prev*fanSum;
    return res;
}
