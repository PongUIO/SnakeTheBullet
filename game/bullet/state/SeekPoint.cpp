#include "SeekPoint.h"
#include "../bullet.h"

void SeekPoint::generate(double cplx)
{
	mTargetX = mdBullet::drandi(-0.9, 0.9);
	mTargetY = mdBullet::drandi(-0.9, 0.9);
	mSpeed = mdBullet::drandi(0.02*sqrt(cplx), 0.08*sqrt(cplx));
	
	duration = mdBullet::drandi(2.5, 6.0);
}

double SeekPoint::computeComplexity(double prev)
{
    return prev + 0.125 + 0.5*mSpeed;
}

void SeekPoint::finishCall(Bullet* b)
{
	b->nextRule();
}

void SeekPoint::process(Bullet* b, double delta)
{
	double len = sqrt(b->vx*b->vx+b->vy*b->vy);
	double angle = atan2(b->vy,b->vx);
	
	double targetAngle = atan2(mTargetY-b->y, mTargetX-b->x);
	
	if(targetAngle < angle)
		angle -= 0.65*delta;
	else
		angle += 0.65*delta;
	
	if(len < mSpeed)
		len += 0.15*delta;
	else
		len -= 0.15*delta;
	
	b->vx = cos(angle)*len;
	b->vy = sin(angle)*len;
}
