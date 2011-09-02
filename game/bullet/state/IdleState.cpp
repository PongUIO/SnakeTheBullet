#include "IdleState.h"
#include "../bullet.h"

IdleState::IdleState(bool endDeath, bool startup): mDie(endDeath), mStartup(startup)
{}


void IdleState::activate(Bullet* b)
{
}


void IdleState::generate(double cplx)
{
	if(mDie)
		duration = mdBullet::drandi(3.5, 9.5);
	else if(mStartup)
		duration = mdBullet::drandi(3.5,4.5);
	else
		duration = mdBullet::drandi(3, 4);
}


void IdleState::finishCall(Bullet* b)
{
	if(mDie)
		b->kill();
	else
		b->nextRule();
}

void IdleState::process(Bullet* b, double delta)
{
	if(mDie)
		b->scale = 1.0 - pow(b->ruleTimer/duration, 8.0);
}
