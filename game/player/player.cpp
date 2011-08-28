#include <GL/gl.h>

#include <stdio.h>
#include "player.h"
#include "../bullet/bullet.h"

class mdPlayer modPlayer;

void mdPlayer::shutdown()
{
}

void mdPlayer::startup()
{
	x = y = 0;
	h = w = 0.15;
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
	double km = 1;
	if(kct) km = 0.5;
	if(ksh) km = 1.5;
	if(kalt) km = 2;
	if(kup && y+h*0.5 <= 0.99) y += delta*km;
	if(kdown && y-h*0.5>= -1) y -= delta*km;
	if(kl && x-w*0.5>=-1) x -= delta*km;
	if(kr && x+w*0.5 <=0.99) x += delta*km;
	if(modBullet.checkCollision(x,y,0.02)) {//first bracket if in file
		printf("hit\n");
	}
}

void mdPlayer::draw()
{
	glTranslatef(x-(w*0.5),y-(h*0.5),0);//becoming scheeme code
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex2f( 0, 0 );
		glVertex2f( h, 0 );
		glVertex2f( h, w );
		glVertex2f( 0, w );
	glEnd(); 
	//test hitboxpolygon
	//glLoadIdentity();
	glTranslatef(w*0.5-0.01, h*0.5-0.01, 0.0);
	glBegin(GL_QUADS);
		glColor3f(1.0,0.0,0.0);
		glVertex2f( 0, 0 );
		glVertex2f( 0.02, 0 );
		glVertex2f( 0.02, 0.02 );
		glVertex2f( 0, 0.02);
	glEnd(); 
}


