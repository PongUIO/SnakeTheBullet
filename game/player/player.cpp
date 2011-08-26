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
		case SDLK_RIGHT: kr=upflag; break; }
}

void mdPlayer::process(double delta)
{
	if(kup) y += delta;
	if(kdown) y -= delta;
	if(kl) x -= delta;
	if(kr) x += delta;
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
