#ifndef GAME_FBO_H
#define GAME_FBO_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

class FrameBuffer {
	public:
		void init(int w,int h, double blurScale=1.0);
		void clearTextures();
		
		void startRender();
		void endRender(bool flip=true);
		
		void screenBlit();
		
	private:
		enum Texture {
			TexBuffer1 = 0,
			TexBuffer2,
			
			TextureMax,
			TexBufferMax = TexBuffer2+1,
		};
		
		void fullscreenQuad();
		
		void renderTarget(Texture target);
		void genTextures();
		void initShader();
		
		void shaderLog(GLuint shader);
		void programLog(GLuint program);
		
		void runShader(GLuint shId, int numTex, GLuint texId[], const char *sampId[]);
		void blendBuffers();
		
		GLuint fbo;
		GLuint renderTex[TextureMax];
		
		// Blending shaders
		GLuint vbs, fbs, bprog;
		
		Texture activeRender;
		
		int texSize;
		int scrW,scrH;
		double blurScale;
};

#endif
