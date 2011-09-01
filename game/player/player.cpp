#include <GL/gl.h>

#include <stdio.h>
#include "player.h"
#include "../bullet/bullet.h"
#include "../item/item.h"

class mdPlayer modPlayer;

void mdPlayer::shutdown()
{}

void mdPlayer::startup()
{
	x = y = 0;
	ph = pw = 0.1;
	phb = 0.01;
	pe = 0.000001;
	pib = 0.3;
	s = 0;
	l = 5;
}

void mdPlayer::score()
{
	printf("item get\n score: %d", ++s);
	int g = 1.01;
	phb = phb*g;
	pib = -pib*g;
	ph = ph*g;
	pw = pw*g;
	pe = pe*g;
}


void mdPlayer::input( SDL_Event *event )
{
	bool upflag = event->type == SDL_KEYDOWN;
	switch( event->key.keysym.sym ) { 
		case SDLK_UP: kup=upflag; break; 
		case SDLK_w: kup=upflag; break;
		case SDLK_DOWN: kdown=upflag; break;
		case SDLK_s: kdown=upflag; break;
		case SDLK_LEFT: kl=upflag; break; 
		case SDLK_a: kl=upflag; break;
		case SDLK_RIGHT: kr=upflag; break;
		case SDLK_d: kr=upflag; break;
		case SDLK_LSHIFT: ksh=upflag; break;
		case SDLK_LCTRL: kct=upflag; break;
		case SDLK_LALT: kalt=upflag; break;
	}
}

void mdPlayer::process(double delta)
{
	km = 0.7/(pe/0.000001);
	if(kct) km = km/2-pe;
	if(ksh) km = km*1.5+pe;
	if(kalt) km = km*2+pe;
	if(kup && y+phb*0.5 <= 0.99) y += delta*km;
	if(kdown && y-phb*0.5>= -1) y -= delta*km;
	if(kl && x-phb*0.5>=-1) x -= delta*km;
	if(kr && x+phb*0.5 <=0.99) x += delta*km;
	if(modBullet.checkCollision(x,y,phb)) {
		printf("hit\n");
		--l;
		printf("%d \n", l);
		x = 0;
		y = 0;
		if(l <= 0) printf("death\n");
	} 
	if(modItem.checkCollision(x,y,pib*2)) score();
}

void mdPlayer::draw()
{
	glLoadIdentity();
	glTranslatef(x-(pw*0.5),y-(ph*0.5),0);//becoming scheeme code
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex2f( 0, 0 );
		glVertex2f( ph, 0 );
		glVertex2f( ph, pw );
		glVertex2f( 0, pw );
	glEnd(); 
	//test hitboxpolygon
	glTranslatef(pw*0.5-0.01, ph*0.5-0.01, 0.0);
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glVertex2f( 0, 0 );
		glVertex2f( phb, 0 );
		glVertex2f( phb, phb );
		glVertex2f( 0, phb);
	glEnd(); 
}


