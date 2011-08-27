#ifndef BULLET_H
#define BULLET_H

#include <boost/random.hpp>

#ifndef PI
#define PI 3.1415927
#endif

#include "../factory.h"
#include "../module.h"

#include "brule.h"

class Bullet {
	public:
		struct Config {
			Config(double x,double y, double vx, double vy, BulletRule *rule, int activeState=0) :
				x(x), y(y), vx(vx), vy(vy), rule(rule), activeState(activeState)
				{}
			
			double x,y;
			double vx, vy;
			BulletRule *rule;
			int activeState;
		};
		
		Bullet(const Config &config);
		~Bullet();
		
		void process(double delta);
		void draw();
		
		bool isDead() { return toDie; }
		void kill() { toDie=true; }
		
		BulletRule *getRule() { return rule; }
		int getActiveState() { return activeRuleState; }
		
		double x,y;
		double vx, vy;
		
		double fparam[4];
		double iparam[4];
		
		void nextRule() { setRule(activeRuleState+1); }
	
		BulletRule *rule;
		int activeRuleState;
		double ruleTimer;
		
		double renderAngle;
		double initialAngle;
		
		double scale;
		
		bool toDie;
		
		void setRule(int rule);
};

class mdBullet : public Module, public Factory<Bullet> {
	public:
		mdBullet();
		~mdBullet();
		
		void create(const Bullet::Config &config);
		
		/// @name Signals
		//@{
			void startup();
			void shutdown();
			
			void clearData() { Factory::killAll(); }
			
			void input( SDL_Event *event ) {}
			void process(double delta);
			void draw();
		//@}
		
		/// @name Rng
		//@{
			static int random(int min, int max);
			static double drandi(double min, double max);
		//@}
		
	private:
		
};

extern mdBullet modBullet;

#endif
