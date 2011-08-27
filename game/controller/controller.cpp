#include <vector>

#include "controller.h"
#include "../bullet/bullet.h"

class mdController modController;

mdController::mdController()
{}
mdController::~mdController()
{}

void mdController::startup()
{
	mCurComplexity = 600.0;
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


struct Border {
	double xMult, yMult;
	double xOff, yOff;
} borderData[] = {
	{ 0.0, 1.0,-1.0, 0.0 }, // Left
	{ 0.0,-1.0, 1.0, 0.0 }, // Right
	{ 1.0, 0.0, 0.0, 1.0 }, // Top
	{-1.0, 0.0, 0.0,-1.0 }, // Bottom
};

void mdController::nextPhase()
{
	mCurComplexity += 200.0;
	
	int numRules = mdBullet::random(1, 1+sqrt(mCurComplexity / 25.0) );
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
		
		switch( mdBullet::random(0, StyleMax-1) )
		{
			case Circle:
				for(int i=0; i<numBullets; i++) {
					double angle = baseAngle + double(i)/double(numBullets) * 3.1415*2.0;
					modBullet.create(
						Bullet::Config(
							cos(angle)*sqrt(2), sin(angle)*sqrt(2),
							cos(angle+inAngle)*0.1, sin(angle+inAngle)*0.1,
							rule
						)
					);
				}
				break;
			
			case Side:
			{
				for(int i=0; i<numBullets; i++) {
					Border &b = borderData[i%4];
					modBullet.create(
						Bullet::Config(
							b.xOff + b.xMult* double(i)/double(numBullets),
							b.yOff + b.yMult* double(i)/double(numBullets),
							-b.xOff*0.1, -b.yOff*0.1,
							rule
						)
					);
				}
			}
				break;
		}
		
	}
	
	mNextPhaseTimer = maxLength*0.93 + 1.0;
}
