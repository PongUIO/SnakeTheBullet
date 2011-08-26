#include <stdio.h>
#include <QGLWidget>
#include "player.h"

#include "../../input.h"

class mdPlayer modPlayer;

void mdPlayer::input()
{
	if(mIn.getKey(OIS::KC_Y))
		printf("Test\n");
}

void mdPlayer::process(double delta)
{
}

void mdPlayer::draw()
{
	glBegin(GL_QUADS);
		glColor3f(1.0,1.0,1.0);
		glVertex2f( -0.5f, -0.5f );
		glVertex2f( 0.5f, -0.5f );
		glVertex2f( 0.5f, 0.5f );
		glVertex2f( -0.5f, 0.5f );
	glEnd();
}
