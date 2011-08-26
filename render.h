#ifndef RENDER_H
#define RENDER_H
#include<QtGui>
#include<iostream>
#include <QGLWidget>

#include "game/signal.h"

class MyGLDrawer : public QGLWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots
	
	public slots:
		void redraw() {
			paintGL();
		}
	
	public:
		MyGLDrawer(QWidget *parent = 0)
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
		}
	
	protected:
		// overridden
		void initializeGL()
		{
			glClearColor( 0.0, 0.0, 0.0, 0.0 );
			glEnable(GL_DOUBLE);
		}

		// overridden
		void resizeGL( int w, int h )
		{
			// setup viewport, projection etc.:
			glViewport( 0, 0, (GLint)w, (GLint)h );
			//glFrustum( ... );
		}
		
		// overridden
		void paintGL()
		{
			int i, j;
			glClear(GL_COLOR_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-1,1,-1,1, 0.01, 1000);
			glTranslatef(0,0,-1);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			
			modSignal.draw();
			
			glBlendFunc(GL_ONE, GL_ZERO);
			glDisable(GL_BLEND);
			
			glFlush();
			glFinish();
			swapBuffers();
		}
};
#endif
