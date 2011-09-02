#include "Gravity.h"
#include "../bullet.h"

void Gravity::generate(double cplx)
{
	duration = mdBullet::drandi(3.0, 5.5);
	
	mGravAngle = mdBullet::drandi(0.0, PI*2.0);
	mGravDist = mdBullet::drandi(0.0, 0.5);
	if(mdBullet::random(0,5))
		mGravDist = 0.0;
	mGravStrength = mdBullet::drandi(0.003, 0.003+pow(cplx,1.0/6.0)*0.005);
	
	mInheritAngle = mdBullet::random(0,4);
}

void Gravity::activate(Bullet* b)
{
	b->fparam[0] = cos(mGravAngle+b->initialAngle*mInheritAngle)*mGravDist;
	b->fparam[1] = sin(mGravAngle+b->initialAngle*mInheritAngle)*mGravDist;
}

void Gravity::process(Bullet* b, double delta)
{
	double xL = b->fparam[0]-b->x;
	double yL = b->fparam[1]-b->y;
	double Ldot = xL*xL+yL*yL;
	
	double dist = sqrt(Ldot);
	if(dist < 0.15)
		dist = 0.15;
	
	double xDir = xL/dist;
	double yDir = yL/dist;
	
	double gravStr = mGravStrength/(dist*dist);
	
	b->vx += xDir*gravStr*delta;
	b->vy += yDir*gravStr*delta;
	
	b->x += xDir*gravStr*0.5*delta*delta;
	b->y += yDir*gravStr*0.5*delta*delta;
}

void Gravity::finishCall(Bullet* b)
{
	b->nextRule();
}


double Gravity::computeComplexity(double prev)
{
    double sum = 2.0*mGravStrength/0.0075 * (duration/3.5);
	
	double res = prev*1.1+sum;
	return res;
}
