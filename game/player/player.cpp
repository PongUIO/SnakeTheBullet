#include <GL/gl.h>

#include <stdio.h>
#include "player.h"

class mdPlayer modPlayer;

void mdPlayer::input( SDL_Event *event )
{
}

void mdPlayer::process(double delta)
{
}

void mdPlayer::draw()
{
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex2f( 0, 0 );
		glVertex2f( 0.5, 0 );
		glVertex2f( 0.5, 0.5 );
		glVertex2f( 0, 0.5 );
	glEnd();
}
