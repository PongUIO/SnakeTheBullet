#ifndef ITEM_H
#define ITEM_H

#include "../factory.h"
#include "../module.h"

class Item {
	public:
		
		Item(double nx,double ny);
		
		void process(double delta);
		void draw();
		
		bool checkCollision(double tx, double ty, double size2); 
		
		bool isDead() { return toDie; }
		void kill() { toDie=true; }
		
		double x,y;
		
		bool toDie;
		
};

class mdItem : public Module, public Factory<Item> {
	public:
		mdItem();
		
		void create(double nx, double ny);
		bool checkCollision(double x, double y, double size);
		
		/// @name Signals
		//@{
			void startup();
			void shutdown();
			
			void clearData() { Factory::killAll(); }
			
			void input( SDL_Event *event ) {}
			void process(double delta);
			void draw();
		//@}
		
		bool hasCol;
	private:
		
};

extern mdItem modItem;


#endif