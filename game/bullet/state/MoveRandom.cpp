#include "MoveRandom.h"
#include "../bullet.h"

void MoveRandom::generate(double cplx)
{
    mBaseSpeed = mdBullet::drandi(0.01*(1.0+sqrt(cplx)), 0.02*(1.0+sqrt(cplx)));
	
	duration = mdBullet::drandi(3.5, 7.0);
}

double MoveRandom::computeComplexity(double prev)
{
	double res = prev + 0.25*prev + 1.0;
    return res;//(1.0+prev) * 4.0 * (1.0 + mBaseSpeed*0.5);
}

void MoveRandom::finishCall(Bullet* b)
{
	double dir = mdBullet::drandi(0.0, 2.0*PI);
	
	b->vx = mBaseSpeed*cos(dir);
	b->vy = mBaseSpeed*sin(dir);
	
	bool isLeft = (-b->vx*b->y + b->vy*b->x) > 0;
	
	double len = sqrt(b->vx*b->vx + b->vy*b->vy);
	dir = atan2(b->vy,b->vx);
	if(isLeft)
		dir += mdBullet::drandi(0.2, PI/2.0);
	else
		dir -= mdBullet::drandi(0.2, PI/2.0);
	
	b->vx = cos(dir)*len;
	b->vy = sin(dir)*len;
	
	b->nextRule();
}
