#ifndef MODULE_H
#define MODULE_H

#include <SDL.h>

class Module {
	public:
		virtual void startup()=0;
		virtual void shutdown()=0;
		
		virtual void clearData()=0;
		
		virtual void input( SDL_Event *event )=0;
		virtual void process(double delta)=0;
		virtual void draw()=0;
};

struct ModuleDefinition {
	Module *module;
};

#if 0
	/// @name Signals
	//@{
		void startup();
		void shutdown();
		
		void clearData();
		
		void input( SDL_Event *event );
		void process(double delta);
		void draw();
	//@}
#endif

#endif
