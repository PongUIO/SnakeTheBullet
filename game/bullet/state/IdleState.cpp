#include "IdleState.h"
#include "../bullet.h"

void IdleState::generate(double cplx)
{
    duration = mdBullet::drandi(1.5, 3.5*(1.0+sqrt(cplx)));
}


void IdleState::finishCall(Bullet* b)
{
	b->kill();
}

void IdleState::process(Bullet* b, double delta)
{
	b->scale = 1.0 - pow(b->ruleTimer/duration, 4.0);
}
