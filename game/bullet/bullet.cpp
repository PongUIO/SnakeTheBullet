#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <stdio.h>
#include <time.h>

#include "bullet.h"

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
	
	setRule(config.activeState);
	
	renderAngle = mdBullet::drandi(0.0, 2.0*PI);
	initialAngle = atan2(vy,vx);
	
	scale = 1.0;
}

Bullet::~Bullet()
{
	if(rule)
		rule->unref();
}

void Bullet::process(double delta)
{
	State *state = rule->getState(activeRuleState);
	
	state->process(this, delta);
	
	ruleTimer += delta;
	if(ruleTimer >= state->duration) {
		state->finishCall(this);
	}
	
	x += vx*delta;
	y += vy*delta;
}

void Bullet::draw()
{
	glLoadIdentity();
	glTranslatef(x,y,0);
	glScalef(scale,scale,1.0);
	glRotatef(renderAngle/PI*180.0, 0,0,1.0);
	State *cur = rule->getState(activeRuleState);
	
	glBindBuffer(GL_ARRAY_BUFFER, cur->bufferGl[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cur->bufferGl[1]);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, sizeof(BulletRule::Vertex), 0);
	glColorPointer(3, GL_FLOAT, sizeof(BulletRule::Vertex), BUFFER_OFFSET(sizeof(GLfloat)*2));
	
	int ci = 0;
	//printf("%d\n", cur->mFigures.size());
	for(int i=0; i<cur->mFigures.size(); i++) {
		State::RenderFragment &frag = cur->mFigures[i];
		
		glDrawElements(GL_TRIANGLE_FAN, frag.numVertex+2, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort)*ci) );
		ci += frag.numVertex+2;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Bullet::setRule(int r)
{
	activeRuleState = r;
	ruleTimer = 0.0;
	
	State *state = rule->getState(activeRuleState);
	state->activate(this);
	
	/*switch(state->switchType)
	{
		case BulletRule::Fan:
			fparam[1] = state.iparam[0] / state.length;
			fparam[0] = fparam[1];
			break;
		
		default:
			break;
	}*/
}

/*void Bullet::seekPoint(BulletRule::State& state, double delta)
{
	double len = sqrt(vx*vx+vy*vy);
	double angle = atan2(vy,vx);
	
	double targetAngle = atan2(state.fparam[1]-y, state.fparam[1]-x);
	
	if(targetAngle < angle)
		angle -= 0.5*delta;
	else
		angle += 0.5*delta;
	
	if(len < state.fparam[2])
		len += 0.1*delta;
	else
		len -= 0.1*delta;
	
	vx = cos(angle)*len;
	vy = sin(angle)*len;
}

void Bullet::fan(BulletRule::State& state, double delta)
{
	fparam[0] -= delta;
	if(fparam[0] <= 0.0) {
		Bullet::Config cfg = Bullet::Config(x,y, 0.0,0.0, rule, activeRuleState+1);
		
		cfg.vx = cos(initialAngle+ state.fparam[0]) * state.fparam[1];
		cfg.vy = sin(initialAngle+ state.fparam[0]) * state.fparam[1];
		
		modBullet.create(cfg);
		
		fparam[0] = fparam[1];
	}
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

void Bullet::changeSpeed(BulletRule::State& state)
{
	double dir = atan2(y,x) + mdBullet::drandi(-0.5, 0.5);
	
	vx = cos(dir) * state.fparam[0];
	vy = sin(dir) * state.fparam[0];
	
	nextRule();
}

void Bullet::moveRandom(BulletRule::State& state)
{
	double dir = mdBullet::drandi(0, 2.0*PI);
	
	vx = state.fparam[0]*cos(dir);
	vy = state.fparam[0]*sin(dir);
	
	nextRule();
}

void Bullet::rotateAngle(BulletRule::State& state)
{
	double angle = atan2(vy,vx);
	angle += mdBullet::drandi(-state.fparam[0], state.fparam[0]);
	
	vx = state.fparam[1]*cos(angle);
	vy = state.fparam[1]*sin(angle);
	
	nextRule();
}*/

// mdBullet
//
//
mdBullet::mdBullet() : Factory()
{}

mdBullet::~mdBullet()
{}

void mdBullet::startup()
{
	rng.seed(time(0));
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
}

void mdBullet::draw()
{
	factoryCall( boost::bind(&Bullet::draw, _1) );
}
