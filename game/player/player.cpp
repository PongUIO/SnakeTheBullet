#include <GL/gl.h>

#include <stdio.h>
#include "player.h"
#include "../bullet/bullet.h"
#include "../item/item.h"

class mdPlayer modPlayer;

void mdPlayer::shutdown()
{
	printf("%d", s);
}

void mdPlayer::startup()
{
	x = y = 0;
	ph = pw = 0.1;
	phb = 0.01;
	km = 0.7;
	s = 0;
	l = 5;
}

void mdPlayer::score()
{
	printf("item get\n score: %d", s);
	int g = 1.01;
	phb = phb*g;
	ph = ph*g;
	pw = pw*g;
	++s;
}


void mdPlayer::input( SDL_Event *event )
{
	bool upflag = event->type == SDL_KEYDOWN;
	switch( event->key.keysym.sym ) { 
		case SDLK_UP: kup=upflag; break; 
		case SDLK_DOWN: kdown=upflag; break; 
		case SDLK_LEFT: kl=upflag; break; 
		case SDLK_RIGHT: kr=upflag; break;
		case SDLK_LSHIFT: ksh=upflag; break;
		case SDLK_LCTRL: kct=upflag; break;
		case SDLK_LALT: kalt=upflag; break;
	}
}

void mdPlayer::process(double delta)
{
	if(kct) km = km*0.5;
	if(ksh) km = km*1.5;
	if(kalt) km = km*2;
	if(kup && y+phb*0.5 <= 0.99) y += delta*km;
	if(kdown && y-phb*0.5>= -1) y -= delta*km;
	if(kl && x-phb*0.5>=-1) x -= delta*km;
	if(kr && x+phb*0.5 <=0.99) x += delta*km;
	if(modBullet.checkCollision(x,y,phb)) //first bracket if in file
	{
		printf("hit\n");
		--l;
		printf("%d \n", l);
		x = 0;
		y = 0;
		if(l <= 0) printf("death\n");
	} 
	if(modItem.checkCollision(x,y,phb*1.5)) score();
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


