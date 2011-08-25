#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<iostream>
#include <QGLWidget>
#include "camera.h"

class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots

public slots:
        void redraw() {
                paintGL();
        }

        void moveMouseCheck() {

                cam->addVel(lastX, lastY);
        }

public:
        int lastX, lastY;
        Camera *cam;
        MyGLDrawer(Camera *cam, QWidget *parent = 0)
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
                this->cam = cam;
                lastX = width()/2;
                lastY = height()/2;


        }

protected:
        // overridden
        void keyPressEvent (QKeyEvent *event) {
		//              qDebug() << event->key();
        }

        // overriden
        void mouseMoveEvent(QMouseEvent * event) {
                lastX = event->pos().x();
                lastY = event->pos().y();
		//                qDebug() << event->pos().x() << " " << event->pos().y();
        }

        // overridden  
        void mousePressEvent(QMouseEvent * event) {
                int w = width();
                int h = height();
        }



	// overridden
        void initializeGL()
        {
                // Set up the rendering context, define display lists etc.:
                glClearColor( 0.0, 0.0, 0.0, 0.0 );
                //glEnable(GL_DEPTH_TEST);
                glEnable(GL_DOUBLE);
                /*data[0].load(":/graphics/tiles/empty.png");
                textures[0] = bindTexture(data[0].scaled(64,64));
		data[1].load(":/graphics/tiles/metal.png");
                textures[1] = bindTexture(data[1].scaled(64,64));
		data[2].load(":/graphics/tiles/metal2surf.png");
                textures[2] = bindTexture(data[2].scaled(64,64));
                characters[0].load(":/graphics/characters/character1.png");
                chartextures[0] = bindTexture(characters[0].scaled(128,320).mirrored(true, false));

                weapons[0].load(":/graphics/weapons/testweapon.png");
                weaponstextures[0] = bindTexture(weapons[0].scaled(32,64));

                                glEnable(GL_TEXTURE_2D);
                glGenTextures(3,&texture[0]);
                glBindTexture(GL_TEXTURE_2D,texture[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glBindTexture(GL_TEXTURE_2D, texture[0]);
                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, gldata.width(),
                              gldata.height(),
                              0, GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits() );

                glDisable(GL_TEXTURE_2D);*/

        }

        // overridden
        void resizeGL( int w, int h )
        {
                // setup viewport, projection etc.:
                glViewport( 0, 0, (GLint)w, (GLint)h );
                //glFrustum( ... );
        }

        void wheelEvent(QWheelEvent *event) {
                cam->dzoom -= event->delta()/500000.0;//pow(10, cam->zoom);
        }

        // overridden
        void paintGL()
        {
                cam->iter();
                int i, j;
                glClear(GL_COLOR_BUFFER_BIT);
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(-cam->zoom, cam->zoom, -cam->zoom*cam->ratio, cam->zoom*cam->ratio, 0.01, 1000);
                glTranslatef(cam->x,cam->y,-1);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                // draw the scene:
                //glRotatef( ... );
                //glMaterialfv( ... );
                //glDrawPixels(data.width(), data.height(), GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits());
                glEnable(GL_TEXTURE_2D);

                //       glViewport(cam->x, cam->y, getWidth(), getHeight());
                int fx = -cam->x-cam->zoom-1;
                int tx = -cam->x+cam->zoom+1;
                int fy = -cam->y-cam->zoom*cam->ratio-1;
                int ty = -cam->y+cam->zoom*cam->ratio+1;
                for (i = fx; i < tx; i++) {
                        for (j = fy; j < ty; j++) {
                                //glBindTexture(GL_TEXTURE_2D, textures[mt]);
                                glBegin(GL_QUADS);
                                glTexCoord2f(0,1); glVertex2f(i,j+1);  // lower left
                                glTexCoord2f(0,0); glVertex2f(i,j); // lower right
                                glTexCoord2f(1,0); glVertex2f(i+1,j);// upper right
                                glTexCoord2f(1,1); glVertex2f(i+1,j+1); // upper left
                                glEnd();
                        }

                }

                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glEnable(GL_BLEND);

                glBlendFunc(GL_ONE, GL_ZERO);
                glDisable(GL_TEXTURE_2D);
                glDisable(GL_BLEND);
		glFlush();
		glFinish();
                swapBuffers();
        }
        /*double getXPix(int x) {
                return -1+(((double)x)/width())*2-cam->x;
        }
        double getYPix(int y) {
                return 1-(((double)y)/height())*2-cam->y;
        }*/

};
#endif
