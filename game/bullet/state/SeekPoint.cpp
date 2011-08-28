#include "SeekPoint.h"
#include "../bullet.h"

#define DIST 0.8

void SeekPoint::generate(double cplx)
{
	if(mdBullet::random(0,4) ) {
		mTargetX = mdBullet::drandi(-DIST, DIST);
		mTargetY = mdBullet::drandi(-DIST, DIST);
	} else {
		mTargetX = 0.0;
		mTargetY = 0.0;
	}
	mSpeed = mdBullet::drandi(0.005*sqrt(cplx), 0.015*sqrt(cplx));
	
	mSeekRandom = !mdBullet::random(0,4);
	
	duration = mdBullet::drandi(3.5, 5.0);
}

void SeekPoint::activate(Bullet* b)
{
	if(mSeekRandom) {
		b->fparam[0] = mdBullet::drandi(-DIST, DIST);
		b->fparam[1] = mdBullet::drandi(-DIST, DIST);
	} else {
		b->fparam[0] = mTargetX;
		b->fparam[1] = mTargetY;
	}
}


double SeekPoint::computeComplexity(double prev)
{
    return prev + 0.05*prev + 1.0 + 0.05*prev*mSeekRandom;
}

void SeekPoint::finishCall(Bullet* b)
{
	b->nextRule();
}

void SeekPoint::process(Bullet* b, double delta)
{
	double tx = b->fparam[0], ty = b->fparam[1];
	
	double len = sqrt(b->vx*b->vx+b->vy*b->vy);
	double angle = atan2(b->vy,b->vx);
	
	bool isLeft = (b->vx*(ty-b->y) - b->vy*(tx-b->x)) > 0;
	
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
