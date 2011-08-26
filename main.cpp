#include <SDL.h>
#include <GL/gl.h>

#include "game/signal.h"
#include "game/game.h"
#include "game/timer.h"

SDL_Surface *screen;

void init_GL() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	double ratio = double(screen->w) / double(screen->h);
	glOrtho(-ratio, ratio, -1, 1, 1,1);
}

void startup() {
	SDL_Init( SDL_INIT_EVERYTHING );
	
	screen = SDL_SetVideoMode(0,0,0, SDL_OPENGL);
	init_GL();
	
	mTimer.setup();
	
	modSignal.startup();
}

void shutdown() {
	modSignal.shutdown();
}

int main(int argc, char *argv[]) {
	startup();
		modGame.run();
	shutdown();
	
	return 0;
}
