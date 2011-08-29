#ifndef PLAYER_H
#define PLAYER_H

#include "../module.h"

class mdPlayer : public Module {
	public:
		/// @name Signals
		//@{
			void startup();
			void shutdown();
			
			void clearData() {}
			
			void input( SDL_Event *event );
			void process(double delta);
			void draw();
		//@}
		
	private:
		void score();
		double x,y;
		double ph,pw,phb;
		int s,l;
		bool kup, kdown, kl, kr; 
		bool kct, ksh, kalt;
};

extern class mdPlayer modPlayer;

#endif
