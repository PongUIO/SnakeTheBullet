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
		
		enum GenStyle {
			Circle,
			MultiCircle,
			Side,
			
			StyleMax
		};
		
		double mStyleWeight[StyleMax];
		double mStyleWeightMax;
		
		double mCurComplexity;
		double mNextPhaseTimer;
		double mItemSpawnTimer;
		int mCurPhase;
};

extern class mdController modController;

#endif
