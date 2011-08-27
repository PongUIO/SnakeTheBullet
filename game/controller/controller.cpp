#include "controller.h"
#include "../bullet/bullet.h"

class mdController modController;

mdController::mdController()
{}
mdController::~mdController()
{}

void mdController::startup()
{
	mCurComplexity = 200.0;
	nextPhase();
}

void mdController::shutdown()
{

}

void mdController::process(double delta)
{
	mNextPhaseTimer -= delta;
	if(mNextPhaseTimer <= 0.0) {
		nextPhase();
	}
}

void mdController::nextPhase()
{
	mCurComplexity += 50.0;
	
	BulletRule *rule = BulletRule::designRule(
		mCurComplexity / sqrt(mCurComplexity));
	
	mNextPhaseTimer = rule->getLength() + 1.0;
	
	int numBullets = mCurComplexity / rule->getComplexity();
	double inAngle = mdBullet::drandi(2.5, 3.5);
	for(int i=0; i<numBullets; i++) {
		double angle = double(i)/double(numBullets) * 3.1415*2.0;
		modBullet.create(
			Bullet::Config(
				cos(angle), sin(angle),
				cos(angle+inAngle)*0.1, sin(angle+inAngle)*0.1,
				rule
			)
		);
	}
}
