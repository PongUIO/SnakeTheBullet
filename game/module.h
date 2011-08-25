#ifndef MODULE_H
#define MODULE_H

class Module {
	public:
		virtual void startup()=0;
		virtual void shutdown()=0;
		
		virtual void clearData()=0;
		
		virtual void input()=0;
		virtual void process(double delta)=0;
		virtual void draw()=0;
};

struct ModuleDefinition {
	Module *module;
};

#endif
