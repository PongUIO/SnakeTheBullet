#ifndef BSTATE_H
#define BSTATE_H

#include <GL/gl.h>
#include <vector>

class Bullet;

class State {
	public:
		virtual ~State();
		
		virtual double computeComplexity(double prevComplexity)
			{ return prevComplexity; }
		virtual void generate(double complexity)
			{}
		
		virtual void activate(Bullet *b)=0;
		virtual void process(Bullet *b, double delta)=0;
		virtual void finishCall(Bullet *b)=0;
		
		void setup(double cplx);
		
		GLuint bufferGl[2];
		double duration;
		
		struct RenderFragment {
			float baseAngle;
			float moveOffset;
			
			struct Spike {
				float r,g,b;
				float radius;
			};
			
			typedef std::vector<Spike> SpikeVec;
			SpikeVec spikes;
			
			int numVertex;
		};
		
		typedef std::vector<RenderFragment> FragmentVec;
		FragmentVec mFigures;
};

#endif
