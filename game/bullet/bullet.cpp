#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <stdio.h>

#include "bullet.h"
#define PI 3.1415927

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

class mdBullet modBullet;

// Random number generator
boost::mt19937 rng;
typedef boost::uniform_int<> r_int;
typedef boost::uniform_real<> r_real;

typedef boost::variate_generator<boost::mt19937&, r_int> int_gen;
typedef boost::variate_generator<boost::mt19937&, r_real> real_gen;

static double drand()
{	static r_real type(0,1);
	static real_gen gen(rng, type);
	return gen();
}

double mdBullet::drandi(double min, double max)
{	return min + drand()*(max-min); }

int mdBullet::random(int min, int max)
{
	if(min==max)
		return min;
	
	r_int type(min,max);
	int_gen gen(rng, type);
	
	return gen();
}


// Bullet
//
//
Bullet::Bullet(const Bullet::Config& config) :
	x(config.x),y(config.y),
	vx(config.vx),vy(config.vy),
	
	rule(0),
	ruleTimer(0.0),
	
	toDie(false)
{
	rule = config.rule;
	if(rule)
		rule->ref();
	
	ruleTimer = 0.0;
	activeRuleState = config.activeState;
	
	baseAngle = mdBullet::drandi(0.0, 2.0*PI);
}

Bullet::~Bullet()
{
	if(rule)
		rule->unref();
}

void Bullet::process(double delta)
{
	ruleTimer += delta;
	
	BulletRule::State &state = rule->getState(activeRuleState);
	
	if(ruleTimer >= state.length) {
		switch(state.switchType)
		{
			case BulletRule::CircleSpawn:
				circleSpawn(state);
				break;
			
			case BulletRule::IdleRule:
				toDie=true;
				break;
			
			default:
				toDie=true;
				break;
		}
	}
	
	x += vx*delta;
	y += vy*delta;
}

void Bullet::draw()
{
	glLoadIdentity();
	glTranslatef(x,y,0);
	glRotatef(baseAngle/PI*180.0, 0,0,1.0);
	BulletRule::State &curState = rule->getState(activeRuleState);
	
	glBindBuffer(GL_ARRAY_BUFFER, curState.bufferGl[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curState.bufferGl[1]);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, sizeof(BulletRule::Vertex), 0);
	glColorPointer(3, GL_FLOAT, sizeof(BulletRule::Vertex), BUFFER_OFFSET(sizeof(GLfloat)*2));
	
	int ci = 0;
	for(int i=0; i<curState.mFigures.size(); i++) {
		BulletRule::RenderFragment &frag = curState.mFigures[i];
		
		glDrawElements(GL_TRIANGLE_FAN, frag.numPoints+1, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort)*ci) );
		ci += frag.numPoints+1;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Bullet::circleSpawn(BulletRule::State& state)
{
	Bullet::Config cfg = Bullet::Config(x,y, 0.0,0.0, rule, activeRuleState+1);
	
	for(int i=0; i<state.iparam[0]; i++) {
		double angle = double(i) / double(state.iparam[0]) * 2.0*PI;
		
		cfg.vx = cos(angle)*state.fparam[0];
		cfg.vy = sin(angle)*state.fparam[0];
		
		modBullet.create( cfg );
	}
	
	toDie=true;
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
	rng.seed(1236);
}

void mdBullet::shutdown()
{
	clearData();
}



void mdBullet::create(const Bullet::Config& config)
{
	Bullet *obj = new Bullet(config);
	addObj(obj);
}

void mdBullet::process(double delta)
{
	factoryCall( boost::bind(&Bullet::process, _1, delta) );
	printf("%d\n", getActive());
}

void mdBullet::draw()
{
	factoryCall( boost::bind(&Bullet::draw, _1) );
}
