#include "ChangeMove.h"
#include "../bullet.h"

void ChangeMove::generate(double cplx)
{
	mStyle = (Style)0;
	
	int val = mdBullet::random(0,100);
	
	// 40% chance of altering speed
	int modval = val % 10;
	if( modval>=0 && modval<=3 )
		mStyle |= AlterSpeed;
	// 70% chance of altering angle
	else if(modval>=3 && modval<=9)
		mStyle |= AlterAngle;
	// 10% Effective chance of both
		
	duration = mdBullet::drandi(0.5, 2.0);
	mFullTime = duration + mdBullet::drandi(2.0, 10.0);
	
	if(mStyle & AlterSpeed) {
		mSpdOff = mdBullet::drandi(0.005*sqrt(cplx), 0.015*sqrt(cplx))*0.1/duration;
		mSpdOff = mdBullet::random(0,1) ? -mSpdOff : mSpdOff;
		mFullTime *= 0.25;
	} else
		mSpdOff = 0.0;
	
	if(mStyle & AlterAngle)
		mAngleOff = mdBullet::drandi(-PI,PI)/duration * 1.5;
	else
		mAngleOff = 0.0;
}

void ChangeMove::activate(Bullet* b)
{
	b->rang = mAngleOff;
	b->racc = mSpdOff;
	b->rhalt = mFullTime;
}

void ChangeMove::process(Bullet* b, double delta)
{
}

void ChangeMove::finishCall(Bullet* b)
{
	b->nextRule();
}

double ChangeMove::computeComplexity(double prev)
{
    double spdComplexity = fabs(mSpdOff)/0.1;
	double angleComplexity = 1.0;
	
	double sum = 
		2.5*
			(
				((mStyle&AlterAngle)!=0)*angleComplexity +
				((mStyle&AlterSpeed)!=0)*spdComplexity
			);
	
	return prev*1.25 + sum;
}
