#include <GL/gl.h>

#include <stdio.h>
#include "player.h"

class mdPlayer modPlayer;

void mdPlayer::shutdown()
{
}

void mdPlayer::startup()
{
	x = y = 0;
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
	if(kup) y += delta*km;
	if(kdown) y -= delta*km;
	if(kl) x -= delta*km;
	if(kr) x += delta*km;
}

void mdPlayer::draw()
{
	glTranslatef(x,y,0);
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex2f( 0, 0 );
		glVertex2f( 0.5, 0 );
		glVertex2f( 0.5, 0.5 );
		glVertex2f( 0, 0.5 );
	glEnd();
}
