#ifndef SIGNAL_H
#define SIGNAL_H

#include <SDL.h>

#include <vector>
#include "module.h"

class mdSignal {
	private:
		typedef std::vector<Module*> ModuleVector;
		ModuleVector mModules;
		
	public:
		mdSignal();
		~mdSignal();
		
		void startup();
		void shutdown();
		
		void clearData();
		
		void input( SDL_Event *event );
		void process(double delta);
		void draw();
};

extern class mdSignal modSignal;

#endif
