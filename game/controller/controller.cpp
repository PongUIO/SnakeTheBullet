#include <vector>

#include "controller.h"
#include "../bullet/bullet.h"
#include "../item/item.h"

class mdController modController;

mdController::mdController()
{}
mdController::~mdController()
{}

void mdController::startup()
{
	mCurComplexity = 250.0;
	mItemSpawnTimer = 0.0;
	mCurPhase = 0;
	
	mStyleWeight[Circle] = 1.0;
	mStyleWeight[MultiCircle] = 0.5;
	mStyleWeight[Side] = 0.1;
	
	mStyleWeightMax = 0.0;
	for(int i=0; i<StyleMax; i++) {
		mStyleWeightMax += mStyleWeight[i];
	}
	
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
	
	mItemSpawnTimer -= delta;
	if(mItemSpawnTimer <= 0.0) {
		int numi = mdBullet::random(1,1+log(1+mCurComplexity/500.0));
		while( (numi--) ) {
			double x,y;
			x = mdBullet::drandi(-0.9, 0.9);
			y = mdBullet::drandi(-0.9,0.9);
#ifdef USE_PLAYERITEM
			modItem.create(x,y);
#endif
		}
		mItemSpawnTimer = mNextPhaseTimer+1;
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
#ifdef USE_PLAYERITEM
	mCurComplexity += 25.0 + 0.035*mCurComplexity;
#else
	mCurComplexity = mdBullet::drandi(100.0, 7500.0);
#endif
	mCurPhase++;
	
	int numRules1 = mdBullet::random(1, 1 + log(1+mCurComplexity/500.0) );
	int numRules2 = mdBullet::random(1, 1 + log(1+mCurComplexity/500.0) );
	int numRules = numRules1<numRules2 ? numRules1 : numRules2;
	double maxLength = 0.0;
	
	double cplxShare = mCurComplexity;
	for(int i=0; i<numRules; i++) {
		double cShare = cplxShare / double(numRules-i);
		
		BulletRule *rule = BulletRule::designRule(
			cShare / pow(cShare, 0.33));
		
		if(rule->getLength() > maxLength)
			maxLength = rule->getLength();
		mNextPhaseTimer = rule->getLength() + 1.0;
		
		mItemSpawnTimer = mdBullet::drandi(mNextPhaseTimer*0.3, mNextPhaseTimer*0.7);
		
		int numBullets = cShare / rule->getComplexity();
		double inAngle = mdBullet::drandi(PI - PI/12.0, PI + PI/12.0);
		double baseAngle = mdBullet::drandi(0.0, 2.0*PI);
		
		cplxShare -= numBullets * rule->getComplexity();
		
		double bS = mdBullet::drandi(0.1,0.2);
		
		double styleVal = mdBullet::drandi(0.0, mStyleWeightMax);
		GenStyle style = GenStyle(0);
		while(styleVal >= 0.0) {
			if(style >= StyleMax || styleVal < mStyleWeight[style])
				break;
			
			styleVal -= mStyleWeight[style];
			style = GenStyle(style+1);
		}
		if(style >= StyleMax)
			style = Circle;
		
		if( style == MultiCircle && numBullets<12)
			style = Circle;
		
		switch( style )
		{
			case Circle:
				for(int i=0; i<numBullets; i++) {
					double angle = baseAngle + double(i)/double(numBullets) * PI*2.0;
					modBullet.create(
						Bullet::Config(
							cos(angle)*sqrt(2), sin(angle)*sqrt(2),
							cos(angle+inAngle)*bS, sin(angle+inAngle)*bS,
							rule
						)
					);
				}
				break;
			
			case MultiCircle:
			{
				int numCircle = mdBullet::random(2,4);
				double circDist = mdBullet::drandi(0.01, 0.05);
				
				int circBullets = (numBullets + numBullets%numCircle)/numCircle;
				
				bS *= 0.5;
				double cD = mdBullet::drandi(0.25,0.5);
				
				for(int c=0; c<numCircle; c++) {
					double circAngle = baseAngle*baseAngle + double(c)/double(numCircle) * PI*2.0;
					
					for(int i=0; i<circBullets; i++) {
						double angle = baseAngle + double(i)/double(circBullets) * PI*2.0;
						//inAngle = circAngle+angle + PI;
						modBullet.create(
						Bullet::Config(
							cos(angle)*cD + cos(circAngle)*sqrt(circDist), sin(angle)*cD + sin(circAngle)*sqrt(circDist),
							cos(circAngle+inAngle)*bS, sin(circAngle+inAngle)*bS,
							rule
						)
					);
					}
				}
			}
				break;
			
			case Side:
			{
				numBullets += numBullets%4;
				double off = mdBullet::drandi(0.0, 0.25);
				for(int i=0; i<numBullets; i++) {
					Border &b = borderData[i%4];
					modBullet.create(
						Bullet::Config(
							b.xOff*1.05 + b.xMult* (off+double(i)/double(numBullets)),
							b.yOff*1.05 + b.yMult* (off+double(i)/double(numBullets)),
							-b.xOff*bS, -b.yOff*bS,
							rule
						)
					);
				}
			}
				break;
		}
		
		//printf("Share complexity: %g\n", numBullets*rule->getComplexity());
		//printf("Duration: %g\n", rule->getLength());
	}
	printf("Total complexity: %g / %g\n", mCurComplexity-cplxShare, mCurComplexity);
	printf("Current phase: %d\n", mCurPhase);
	printf("\n");
	
#ifdef USE_PLAYERITEM
	mNextPhaseTimer = maxLength*0.6 + 1.0;
#else
	mNextPhaseTimer = maxLength*0.8 + 1.0;
#endif
}
