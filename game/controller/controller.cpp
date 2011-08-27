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
	mCurComplexity += 100.0;
	
	int numRules = mdBullet::random(1, sqrt(mCurComplexity / 25.0) );
	double maxLength = 0.0;
	for(int i=0; i<numRules; i++) {
		double cShare = mCurComplexity / double(numRules);
		
		BulletRule *rule = BulletRule::designRule(
			cShare / sqrt(cShare));
		
		if(rule->getLength() > maxLength)
			maxLength = rule->getLength();
		mNextPhaseTimer = rule->getLength() + 1.0;
		
		int numBullets = cShare / rule->getComplexity();
		double inAngle = mdBullet::drandi(2.5, 3.5);
		double baseAngle = mdBullet::drandi(0.0, 2.0*PI);
		
		for(int i=0; i<numBullets; i++) {
			double angle = baseAngle + double(i)/double(numBullets) * 3.1415*2.0;
			modBullet.create(
				Bullet::Config(
					cos(angle), sin(angle),
					cos(angle+inAngle)*0.1, sin(angle+inAngle)*0.1,
					rule
				)
			);
		}
	}
	
	mNextPhaseTimer = maxLength + 1.0;
}
