#include "CircleSpawn.h"
#include "../bullet.h"

void CircleSpawn::generate(double cplx)
{
	mBulletCount = mdBullet::random(3, 3+sqrt(cplx*0.5));
	mBaseSpeed = mdBullet::drandi(0.015*sqrt(cplx), 0.05*sqrt(cplx));
	
	duration = mdBullet::drandi(0.1, 6.5);
}

void CircleSpawn::finishCall(Bullet* b)
{
	Bullet::Config cfg = Bullet::Config(
		b->x,b->y, 0.0,0.0,
		b->getRule(), b->getActiveState()+1);
	
	for(int i=0; i<mBulletCount; i++) {
		double angle = double(i) / double(mBulletCount) * 2.0*PI;
		
		cfg.vx = cos(angle)*mBaseSpeed;
		cfg.vy = sin(angle)*mBaseSpeed;
		
		modBullet.create( cfg );
	}
	
	b->kill();
}


double CircleSpawn::computeComplexity(double prev)
{	return (1.0+prev) * 1.0 * (1.0 + 0.05*mBulletCount*mBaseSpeed); }
