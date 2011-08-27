#include "IdleState.h"
#include "../bullet.h"

void IdleState::generate(double cplx)
{
    duration = mdBullet::drandi(3.5, 7.5);
}


void IdleState::finishCall(Bullet* b)
{
	b->kill();
}

void IdleState::process(Bullet* b, double delta)
{
	b->scale = 1.0 - pow(b->ruleTimer/duration, 4.0);
}
