#include <GL/gl.h>
#include <stdio.h>

#include "bullet.h"

class mdBullet modBullet;

static double drand()
{	return double(rand())/double(RAND_MAX);}

static double drandi(double min, double max)
{
	return min + drand()*(max-min);
}

// Bullet
//
//
Bullet::Bullet(const Bullet::Config& config) :
	x(config.x),
	y(config.y),
	toDie(false)
{
	vx = drandi(-0.1,0.1);
	vy = drandi(-0.1,0.1);
}

Bullet::~Bullet()
{
}

void Bullet::process(double delta)
{
	x += vx*delta;
	y += vy*delta;
}

void Bullet::draw()
{
	double s = 0.01;
	glLoadIdentity();
	glTranslatef(x,y,0);
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glVertex2f( -s, -s );
		glVertex2f( s, -s );
		glVertex2f( s, s );
		glVertex2f( -s, s );
	glEnd();
}


// mdBullet
//
//
mdBullet::mdBullet() : Factory()
{}

mdBullet::~mdBullet()
{}

void mdBullet::startup()
{
	srand(1234);
}

void mdBullet::shutdown()
{}



void mdBullet::create(const Bullet::Config& config)
{
	Bullet *obj = new Bullet(config);
	addObj(obj);
}

void mdBullet::process(double delta)
{
	factoryCall( boost::bind(&Bullet::process, _1, delta) );
}

void mdBullet::draw()
{
	factoryCall( boost::bind(&Bullet::draw, _1) );
}
