#include <SDL.h>
#include <GL/gl.h>

#include "game/signal.h"
#include "game/game.h"
#include "game/timer.h"

SDL_Surface *screen;

void init_GL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	double s = 1.0;
	glOrtho(-1, 1, -1, 1, 1,1);
	glScalef(s,s,s);
}

void startup() {
	SDL_Init( SDL_INIT_VIDEO );
	
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	int size = info->current_h<info->current_w ? info->current_h : info->current_w;
	size = double(size)*0.85;
	screen = SDL_SetVideoMode(size,size,0, SDL_OPENGL);
	init_GL();
	
	mTimer.setup();
	
	modSignal.startup();
}

void shutdown() {
	modSignal.shutdown();
	
	SDL_Quit();
}

int main(int argc, char *argv[]) {
	startup();
		modGame.run();
	shutdown();
	
	return 0;
}
