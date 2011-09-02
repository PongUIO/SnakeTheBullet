#include "CircleSpawn.h"
#include "../bullet.h"

void CircleSpawn::generate(double cplx)
{
	mBulletCount = mdBullet::random(2, 4+sqrt(cplx*0.5));
	mBaseSpeed = mdBullet::drandi(0.005*(1.0+sqrt(cplx)), 0.08*(1.0+0.4*sqrt(cplx)));
	
	mInstant = mdBullet::random(0,3);
	mFreeze = mdBullet::random(0,3);
	
	duration = mdBullet::drandi(0.15, 6.5);
	mStartTime = mdBullet::drandi(duration*0.6, duration*0.95);
}

void CircleSpawn::activate(Bullet* b)
{
	if(!mInstant) {
		b->fparam[0] = 0.0;
	}
}

void CircleSpawn::process(Bullet* b, double delta)
{
	if(!mInstant) {
		double prevTime = b->fparam[0];
		b->fparam[0]+=delta;
		double curTime = b->fparam[0];
		
		prevTime -= mStartTime;
		curTime -= mStartTime;
		double interval = (duration-mStartTime) / double(mBulletCount) *0.99;
		
		if(b->fparam[0] > mStartTime) {
			int prevInd, curInd;
			prevInd = int( prevTime / interval );
			curInd = int( curTime / interval );
			spawnBullets(b, prevInd, curInd);
			
			if(mFreeze) {
				b->vx = 0.0;
				b->vy = 0.0;
			}
		}
	}
}

void CircleSpawn::spawnBullets(Bullet* b, int start, int end)
{
	Bullet::Config cfg = Bullet::Config(b->x,b->y, 0.0,0.0, b->getRule(), b->getActiveState()+1);
	for(int i=start; i<end; i++) {
		double angle = b->initialAngle + double(i) / double(mBulletCount) * 2.0*PI;
		
		cfg.vx = cos(angle)*mBaseSpeed;
		cfg.vy = sin(angle)*mBaseSpeed;
		
		if(!mFreeze) {
			cfg.vx += b->vx;
			cfg.vy += b->vy;
		}
		
		modBullet.create( cfg );
	}
}

void CircleSpawn::finishCall(Bullet* b)
{
	if(mInstant)
		spawnBullets(b, 0, mBulletCount);
	
	b->kill();
}


double CircleSpawn::computeComplexity(double prev)
{	/*double mult = 1.0 * (1.0 + 2.0*mBulletCount*mBaseSpeed);
	return (1.0+prev) * mult;*/
	return prev * (mBaseSpeed/0.08) * pow(double(mBulletCount), 1.25);
}
