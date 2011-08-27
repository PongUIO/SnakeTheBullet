#include "MoveRandom.h"
#include "../bullet.h"

void MoveRandom::generate(double cplx)
{
    mBaseSpeed = mdBullet::drandi(0.015*(1.0+sqrt(cplx)), 0.05*(1.0+sqrt(cplx)));
	
	duration = mdBullet::drandi(3.5, 7.0);
}

double MoveRandom::computeComplexity(double prev)
{
    return (1.0+prev) * 4.0 * (1.0 + mBaseSpeed*0.5);
}

void MoveRandom::finishCall(Bullet* b)
{
	double dir = mdBullet::drandi(0.0, 2.0*PI);
	
	b->vx = mBaseSpeed*cos(dir);
	b->vy = mBaseSpeed*sin(dir);
	
	b->nextRule();
}
