#ifndef GAME_H
#define GAME_H

#include <GL/gl.h>
#include "fbo.h"

class mdGame {
	public:
		void init();
		void run();
		void runFrame();
		
		void updateScreen(int w, int h);
		
		FrameBuffer mFb;
		
		int scrWidth, scrHeight;
};

extern class mdGame modGame;

#endif
