#include "SeekPoint.h"
#include "../bullet.h"

void SeekPoint::generate(double cplx)
{
	mTargetX = mdBullet::drandi(-0.9, 0.9);
	mTargetY = mdBullet::drandi(-0.9, 0.9);
	mSpeed = mdBullet::drandi(0.03*(1.0+sqrt(cplx)), 0.06*(1.0+sqrt(cplx)));
	
	duration = mdBullet::drandi(3.5, 5.0);
}

double SeekPoint::computeComplexity(double prev)
{
    return prev + 0.1*prev + 2.0 + 0.5*mSpeed;
}

void SeekPoint::finishCall(Bullet* b)
{
	b->nextRule();
}

void SeekPoint::process(Bullet* b, double delta)
{
	double len = sqrt(b->vx*b->vx+b->vy*b->vy);
	double angle = atan2(b->vy,b->vx);
	
	bool isLeft = (b->vx*(mTargetY-b->y) - b->vy*(mTargetX-b->x)) > 0;
	
	if(isLeft)
		angle += 5.0*mSpeed*delta;
	else
		angle -= 5.0*mSpeed*delta;
	
	if(len < mSpeed)
		len += 0.15*delta;
	else
		len -= 0.15*delta;
	
	b->vx = cos(angle)*len;
	b->vy = sin(angle)*len;
}
