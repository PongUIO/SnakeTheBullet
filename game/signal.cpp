#include "signal.h"
#include "module.h"

extern struct ModuleDefinition ModuleData[];

class mdSignal modSignal;

mdSignal::mdSignal()
{}

mdSignal::~mdSignal()
{}

#define CALL \
	for(ModuleVector::iterator i=mModules.begin(); i!=mModules.end(); ++i) (*i)->
#define REVERSE_CALL \
	for(ModuleVector::reverse_iterator i=mModules.rbegin(); i!=mModules.rend();i++) (*i)->

void mdSignal::startup()
{
	for(size_t i=0; ModuleData[i].module; i++) {
		mModules.push_back(ModuleData[i].module);
	}
	
	CALL startup();
}

void mdSignal::shutdown()
{
	REVERSE_CALL shutdown();
	
	mModules.clear();
}

void mdSignal::clearData()
{
	CALL clearData();
}


void mdSignal::input( SDL_Event *event )
{
	CALL input(event);
}

void mdSignal::process(double delta)
{
	CALL process(delta);
}

void mdSignal::draw()
{
	CALL draw();
}
