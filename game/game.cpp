#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <GL/gl.h>

#include "game.h"
#include "signal.h"
#include "timer.h"

//#define BLEND

class mdGame modGame;

void mdGame::run()
{
	SDL_Event event;
	
	mTimer.reset_timer();
	
	bool shutdown = false;
	while(!shutdown)
	{
		while( SDL_PollEvent( &event ) ) {
			if( event.type == SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE) ) {
				shutdown = true;
			} else if(
				event.type == SDL_KEYDOWN ||
				event.type == SDL_KEYUP) {
				modSignal.input( &event );
			}
		}
		
		if(mTimer.get_timer()>0) {
			double delta = 0.01;
			
			modSignal.process(delta);
			
			#ifndef BLEND
			glClear( GL_COLOR_BUFFER_BIT );
			#endif
			
			glMatrixMode( GL_MODELVIEW);
			glLoadIdentity();
			
			modSignal.draw();
			
			glMatrixMode( GL_MODELVIEW);
			glLoadIdentity();
			
			#ifdef BLEND
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			double col = 1.0/255.0;
			glColor3f(col,col,col);
			glBegin(GL_QUADS);
				glVertex2f(-2,-2);
				glVertex2f( 2,-2);
				glVertex2f( 2, 2);
				glVertex2f(-2, 2);
			glEnd();
			glDisable(GL_BLEND);
			#endif
			
			SDL_GL_SwapBuffers();
		}
	}
}
