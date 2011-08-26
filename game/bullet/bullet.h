#ifndef BULLET_H
#define BULLET_H

#include "../factory.h"
#include "../module.h"

class Bullet {
	public:
		struct Config {
			Config(double x,double y) : x(x), y(y)
				{}
			
			double x,y;
		};
		
		Bullet(const Config &config);
		~Bullet();
		
		void process(double delta);
		void draw();
		
		bool isDead() { return toDie; }
		
	private:
		double x,y;
		double vx, vy;
		
		bool toDie;
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
			
			void clearData() {}
			
			void input( SDL_Event *event ) {}
			void process(double delta);
			void draw();
		//@}
		
	private:
		
};

extern mdBullet modBullet;

#endif
