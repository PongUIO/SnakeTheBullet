#include "CircleSpawn.h"
#include "../bullet.h"

void CircleSpawn::generate(double cplx)
{
	mBulletCount = mdBullet::random(2, 4+sqrt(cplx*0.5));
	mBaseSpeed = mdBullet::drandi(0.005*(1.0+sqrt(cplx)), 0.08*(1.0+0.4*sqrt(cplx)));
	
	duration = mdBullet::drandi(0.1, 6.5);
}

void CircleSpawn::finishCall(Bullet* b)
{
	Bullet::Config cfg = Bullet::Config(
		b->x,b->y, 0.0,0.0,
		b->getRule(), b->getActiveState()+1);
	
	for(int i=0; i<mBulletCount; i++) {
		double angle = b->initialAngle + double(i) / double(mBulletCount) * 2.0*PI;
		
		cfg.vx = cos(angle)*mBaseSpeed;
		cfg.vy = sin(angle)*mBaseSpeed;
		
		modBullet.create( cfg );
	}
	
	b->kill();
}


double CircleSpawn::computeComplexity(double prev)
{	/*double mult = 1.0 * (1.0 + 2.0*mBulletCount*mBaseSpeed);
	return (1.0+prev) * mult;*/
	return prev * (mBaseSpeed/0.08) * pow(double(mBulletCount), 1.0);
}
