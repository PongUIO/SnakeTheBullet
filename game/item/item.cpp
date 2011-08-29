#include <GL/gl.h>
#include <stdio.h>
#include <time.h>

#include "item.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))


class mdItem modItem;


void Item::process(double delta)
{

}

Item::Item(double nx, double ny)
{
	x = nx;
	y = ny;
	toDie = false;
}

void mdItem::create(double nx, double ny)
{
	Item *obj = new Item(nx, ny);
	addObj(obj);
}

void Item::draw()
{
	double d = 0.02;
	glLoadIdentity();
	glTranslatef(x,y,0);
	glBegin(GL_QUADS);
		glColor3f(0.0,0.0,1.0);
		glVertex2f( 0, 0 );
		glVertex2f( d, 0 );
		glVertex2f( d, d );
		glVertex2f( 0, d );
	glEnd(); 	

}


bool Item::checkCollision(double tx, double ty, double size2)
{
	double dist2 = (tx-x)*(tx-x) + (ty-y)*(ty-y);
	if(dist2 < size2){
		modItem.hasCol = true;
		kill();
	}
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
	hasCol=false;
	factoryCall( boost::bind(&Item::checkCollision, _1, tx, ty, size*size) );
	return hasCol;
}

void mdItem::process(double delta)
{
	
}

void mdItem::draw()
{
	factoryCall( boost::bind(&Item::draw, _1) );
}

mdItem::mdItem() : Factory()
{}	
	
