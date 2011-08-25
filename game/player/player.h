#ifndef PLAYER_H
#define PLAYER_H

#include "../module.h"

class mdPlayer : public Module {
	public:
		/// @name Signals
		//@{
			void startup()  {}
			void shutdown() {}
			
			void clearData() {}
			
			void input() {}
			void process(double delta);
			void draw();
		//@}
		
	private:
		int x,y;
};

extern class mdPlayer modPlayer;

#endif
