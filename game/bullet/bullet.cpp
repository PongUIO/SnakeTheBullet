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
	rang(0), racc(0),
	rhalt(0),
	
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
	
	if(rang != 0 || racc != 0) {
		double angle = atan2(vy,vx);
		double len =  sqrt(vx*vx+vy*vy);
		
		angle += rang*delta;
		initialAngle += rang*delta;
		len += racc*delta;
		
		vx = cos(angle)*len;
		vy = sin(angle)*len;
		
		rhalt -= delta;
		if(rhalt <= 0.0) {
			rang = racc = 0.0;
		}
	}
	
	double cDist2 = x*x+y*y;
	static const double edgeDist = sqrt(2.2);
	if(cDist2 > edgeDist*edgeDist) {
		double dist = cDist2;
		double cDirX = -x/dist;
		double cDirY = -y/dist;
		
		double eMult = (dist-edgeDist)*0.1;
		
		vx += cDirX*eMult*delta;
		vy += cDirY*eMult*delta;
		
		double speed = sqrt(vx*vx+vy*vy);
		if(speed > 0.15) {
			vx = vx/speed*0.15;
			vy = vy/speed*0.15;
		}
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
	
	glScalef(cur->mScale, cur->mScale, 1.0);
	
	glBindBuffer(GL_ARRAY_BUFFER, cur->bufferGl[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cur->bufferGl[1]);
	
	glVertexPointer(2, GL_FLOAT, sizeof(BulletRule::Vertex), 0);
	glColorPointer(3, GL_FLOAT, sizeof(BulletRule::Vertex), BUFFER_OFFSET(sizeof(GLfloat)*2));
	
	int ci = 0;
	//printf("%d\n", cur->mFigures.size());
	for(int i=0; i<cur->mFlowers.size(); i++) {
		State::RenderFlower &flower = cur->mFlowers[i];
		
		glDrawElements(GL_TRIANGLE_FAN, flower.mVertexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(sizeof(GLushort)*ci) );
		ci += flower.mVertexCount;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Bullet::setRule(int r)
{
	activeRuleState = r;
	ruleTimer = 0.0;
	
	State *state = rule->getState(activeRuleState);
	state->activate(this);
}

bool Bullet::checkCollision(double tx, double ty, double size2)
{
	double dist2 = (tx-x)*(tx-x) + (ty-y)*(ty-y);
	if(dist2 < size2)
		modBullet.hasCol = true;
}



// mdBullet
//
//
mdBullet::mdBullet() : Factory<Bullet>()
{}

mdBullet::~mdBullet()
{}

void mdBullet::startup()
{
	seed = time(0);
	rng.seed(seed); // Try: 535
}

void mdBullet::shutdown()
{
	clearData();
	printf("Seed: %d\n", seed);
}

bool mdBullet::checkCollision(double tx, double ty, double size)
{
	hasCol=false;
	factoryCall( boost::bind(&Bullet::checkCollision, _1, tx, ty, size*size) );
	return hasCol;
}



void mdBullet::create(const Bullet::Config& config)
{
	Bullet *obj = new Bullet(config);
	addObj(obj);
}

void mdBullet::process(double delta)
{
	factoryCall( boost::bind(&Bullet::process, _1, delta) );
	
	/*
	static int lastCount = 0;
	if( getActive() != lastCount ) {
		printf("Active bullets: %d\n", getActive());
		lastCount = getActive();
	}
	*/
}

void mdBullet::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	factoryCall( boost::bind(&Bullet::draw, _1) );
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
