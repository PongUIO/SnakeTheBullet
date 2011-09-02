#define GL_GLEXT_PROTOTYPES
#include <SDL.h>
#include <GL/gl.h>

#include "game.h"
#include "signal.h"
#include "timer.h"

class mdGame modGame;

void mdGame::init()
{
#ifdef FBO
	mFb.init(scrWidth,scrHeight);
	mFb.clearTextures();
#endif
	
	mTimer.reset_timer();
}

void mdGame::runFrame()
{
#ifdef DELTATIME
	double delta = DELTATIME;
#else
	double delta = 0.01;
#endif
	
	modSignal.process(delta);
	
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	
#ifdef FBO
	mFb.startRender();
#endif
	
	modSignal.draw();
	
#ifdef FBO
	mFb.endRender();
#endif
	
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	
#ifdef FBO
	mFb.screenBlit();
#endif
	
#ifndef SCREENSAVER
	SDL_GL_SwapBuffers();
#endif
}


void mdGame::run()
{
	SDL_Event event;
	
	init();
	
	bool shutdown = false;
	while(!shutdown)
	{
#ifndef SCREENSAVER
		while( SDL_PollEvent( &event ) ) {
			if( event.type == SDL_QUIT || (event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE) ) {
				shutdown = true;
			} else if(
				event.type == SDL_KEYDOWN ||
				event.type == SDL_KEYUP) {
				modSignal.input( &event );
			}
		}
#endif
		
		if(mTimer.get_timer()>0) {
			runFrame();
		}
	}
}

void mdGame::updateScreen(int w, int h)
{
	scrWidth = w;
	scrHeight = h;
	
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-1,1, -1,1, -1,1);
	glScalef(double(h)/double(w), 1,1);
	
	glClear(GL_COLOR_BUFFER_BIT);
}

