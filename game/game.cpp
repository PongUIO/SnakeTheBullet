#include <SDL.h>
#include <GL/gl.h>

#include "game.h"
#include "signal.h"
#include "timer.h"

#include "bullet/bullet.h"

class mdGame modGame;

void mdGame::run()
{
	SDL_Event event;
	
	mTimer.reset_timer();
	
	bool shutdown = false;
	while(!shutdown)
	{
		while( SDL_PollEvent( &event ) ) {
			if( event.type == SDL_QUIT ) {
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
			
			glClear( GL_COLOR_BUFFER_BIT );
			glMatrixMode( GL_MODELVIEW);
			glLoadIdentity();
			
			modSignal.draw();
			
			SDL_GL_SwapBuffers();
		}
	}
}
