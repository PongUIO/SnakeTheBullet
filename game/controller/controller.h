#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../module.h"

class mdController : public Module {
	public:
		mdController();
		~mdController();
		
		/// @name Signals
		//@{
			void startup();
			void shutdown();
			
			void clearData() {}
			
			void input( SDL_Event *event ) {}
			void process(double delta);
			void draw() {}
		//@}
		
	private:
		void nextPhase();
		
		double mCurComplexity;
		double mNextPhaseTimer;
};

extern class mdController modController;

#endif
