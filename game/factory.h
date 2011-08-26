#ifndef FACTORY_H
#define FACTORY_H

#include <stack>
#include <vector>
#include <algorithm>
#include <stdint.h>

#include <stdio.h>

#include <boost/bind.hpp>


/**
 * This template provides an id-referenced grouping of
 * objects.
 * 
 * When calling specific functions (like step()),
 * every single object is applied the same function ordered
 * by their ID in ascending order (eg. the object with id 1
 * is always called before id 2).
 * 
 * Any class inheriting this template is required to have the
 * following implemented:
 * - deleteInstance(T *obj) : Called when an object is dead.
 * 
 * The object this template uses is required to have the following
 * implemented:
 * - isDead() : Should return true when the object is to be purged.
 * - getId() : Returns the ID of this object
 */
template<class T>
class Factory {
	protected:
		typedef std::stack<uint32_t> IdStack;
		
	public:
		typedef std::vector<T*> ObjVec;
		
		Factory() :
			mIdCounter(0)
			{}
		virtual ~Factory() {}
		
		virtual void deleteInstance(T *obj) { delete obj; }
		
		void addObj(T *obj) {
			mData[newId()] = obj;
		}
		
		template<typename Func>
		void factoryCall(Func func)
		{
			/// @note If new objects are created during this
			/// it is unpredictable whether they are called or not.
			for(uint32_t i = 0; i<mData.size(); i++) {
				T *obj = mData[i];
				
				if(obj) {
					// If the object is not dead, then process it
					if(!obj->isDead())
						func(obj);
					
					// Free and disable this ID
					// if the object is dead
					if(obj->isDead()) {
						deleteInstance(obj);
						freeId(i);
						mData[i] = NULL;
					}
				}
			}
		}
		
	protected:
		void setObject(T *obj, uint32_t id) {
			if(id >= mData.size())
				mData.resize(id+1);
			
			mData[id] = obj;
		}
		
		T *getObject(uint32_t id) const {
			if(id >= mData.size())
				return NULL;
			else
				return mData[id];
		}
		
		uint32_t newId() {
			if(mFreeId.empty()) {
				// Creates a new slot in the vector
				mData.push_back(NULL);
				return mIdCounter++;
			} else {
				uint32_t id = mFreeId.top();
				mFreeId.pop();
				return id;
			}
		}
		
		void freeId(uint32_t id) {
			mFreeId.push(id);
		}
		
		void killAll() {
			for(uint32_t i = 0; i<mData.size(); i++) {
				deleteInstance(mData[i]);
			}
			mData.clear();
			mFreeId = IdStack(); // Clears the free ID list
			mIdCounter = 0;
		}
		
		ObjVec mData;
		IdStack mFreeId;
		uint32_t mIdCounter;
};

#endif
