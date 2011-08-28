#include "IdleState.h"
#include "../bullet.h"

IdleState::IdleState(bool endDeath): mDie(endDeath)
{}


void IdleState::generate(double cplx)
{
    duration = mdBullet::drandi(1.5, 3.5);
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
