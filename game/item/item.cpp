#include <GL/gl.h>
#include <stdio.h>
#include <time.h>

#include "item.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))


class mdItem modItem;


void Item::process(double delta)
{

}

void Item::draw()
{
	double d = 0.2;
	glLoadIdentity();
	glTranslatef(0.7,0.5,0);
	glBegin(GL_QUADS);
		glColor3f(0.0,0.0,1.0);
		glVertex2f( 0, 0 );
		glVertex2f( d, 0 );
		glVertex2f( d, d );
		glVertex2f( 0, d );
	glEnd(); 	

}


bool Item::checkCollision(double tx, double ty, double size)
{
	
}

void mdItem::startup()
{
	
}

void mdItem::shutdown()
{
	clearData();
}

bool mdItem::checkCollision(double tx, double ty, double size)
{
	
}

void mdItem::process(double delta)
{
	
}

void mdItem::draw()
{
	
}

mdItem::mdItem() : Factory()
{}	
	
