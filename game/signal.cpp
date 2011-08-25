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
	for(ModVector::reverse_iterator i=modules[type].rbegin(); i!=modules[type].rend();i++) (*i)->

void mdSignal::startup()
{
	for(size_t i=0; ModuleData[i].module; i++) {
		mModules.push_back(ModuleData[i].module);
	}
	
	CALL startup();
}

void mdSignal::shutdown()
{
	CALL shutdown();
	
	mModules.clear();
}

void mdSignal::clearData()
{
	CALL clearData();
}


void mdSignal::input()
{
	CALL input();
}

void mdSignal::process(double delta)
{
	CALL process(delta);
}

void mdSignal::draw()
{
	CALL draw();
}
