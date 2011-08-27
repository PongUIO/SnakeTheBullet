#include "BulletFan.h"
#include "../bullet.h"

void BulletFan::generate(double cplx)
{
    mAngle = mdBullet::drandi(0.0, 2.0*PI);
	mBaseSpeed = mdBullet::drandi(0.005*(1.0+sqrt(cplx)), 0.05*(1.0+0.5*sqrt(cplx)));
	mCount = mdBullet::random(2, 2+2.0*(1.0+pow(cplx,0.75)));
	
	duration = mdBullet::drandi(4.5, 8.0);
}

void BulletFan::activate(Bullet* b)
{
	b->fparam[1] = (duration / mCount) * 0.99;
	b->fparam[0] = b->fparam[1];
	b->iparam[0] = mCount;
}

void BulletFan::process(Bullet* b, double delta)
{
	b->fparam[0] -= delta;
	if(b->fparam[0] <= 0.0 && b->iparam[0]>0) {
		Bullet::Config cfg = Bullet::Config(b->x,b->y, 0.0,0.0, b->getRule(), b->getActiveState()+1);
		
		cfg.vx = cos(b->initialAngle+mAngle) * mBaseSpeed;
		cfg.vy = sin(b->initialAngle+mAngle) * mBaseSpeed;
		
		modBullet.create(cfg);
		
		b->fparam[0] = b->fparam[1];
		b->iparam[0]--;
		b->scale = double(b->iparam[0]) / double(mCount);
		
		if(b->iparam[0] <= 0)
			b->kill();
	}
}

void BulletFan::finishCall(Bullet* b)
{
	b->kill();
}

double BulletFan::computeComplexity(double prev)
{
    return (1.0+prev) * 1.5 * pow(mCount, 0.66);
}
