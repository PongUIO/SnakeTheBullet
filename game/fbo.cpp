#include <stdio.h>
#include <math.h>

#include "fbo.h"

static const char *vBlendShader =
"void main() {\n"
" gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"
" gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}"
;

#ifndef TRAILPOWER
#define TRAILPOWER 255.0
#endif

#ifndef BLURFOCUS
#define BLURFOCUS 1.0
#endif

#define XSTR(x) STR(x)
#define STR(x) #x

static const char *fBlendShader =
"uniform sampler2D srcTex;\n"
"uniform float blurScale;\n"
#ifdef USE_BLUR
"const float mixf = 1.0/(4.0 + " XSTR(BLURFOCUS) ");\n"
"vec4 getBlur(vec2 off) { return texture2D(srcTex, gl_TexCoord[0].st+off); }\n"
"vec4 getColor() {\n"
" return \n"
"	getBlur(vec2(0,0))*" XSTR(BLURFOCUS) "*mixf +"
"   getBlur(vec2(-blurScale, 0))*mixf +\n"
"   getBlur(vec2( blurScale, 0))*mixf +\n"
"   getBlur(vec2( 0,-blurScale))*mixf +\n"
"   getBlur(vec2( 0, blurScale))*mixf;"
"}\n"
#else
"vec4 getColor() { return texture2D(srcTex, gl_TexCoord[0].st); }\n"
#endif
"void main() {\n"
" vec4 col = getColor();\n"
" gl_FragColor = col - vec4(1.0/" XSTR(TRAILPOWER) ");\n"
"}"
;

static const char *fFluidShader =
"uniform sampler2D srcTex, blurTex;"

"void main() {\n"
" gl_FragColor = vec4(0);\n"
"}"
;

void FrameBuffer::init(int w, int h, double blurScale)
{
	this->blurScale = blurScale;
	scrW = w;
	scrH = h;
	activeRender = TexBuffer1;
	
	texSize = (scrW>scrH) ? scrW : scrH;
	texSize = 1<<int(ceil(log2(texSize)));
	printf("Texture size: %d\n", texSize);
	printf("Width: %d, Height: %d\n", scrW, scrH);
	
	/// Initializing textures
	glEnable(GL_TEXTURE_2D);
	
	glGenTextures(TextureMax, renderTex);
	for(int i=0; i<TextureMax; i++) {
		glBindTexture(GL_TEXTURE_2D, renderTex[i]);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, texSize,texSize, 0, GL_RGB, GL_FLOAT, NULL);
		
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	
	/// Initializing framebuffer
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
	for(int i=0; i<TextureMax; i++) {
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i,
			GL_TEXTURE_2D, renderTex[i], 0);
	}
	
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
		printf("Framebuffer error\n");
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	initShader();
}

void FrameBuffer::initShader()
{
	vbs = glCreateShader(GL_VERTEX_SHADER);
	fbs = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(vbs, 1, &vBlendShader, NULL);
	glShaderSource(fbs, 1, &fBlendShader, NULL);
	
	glCompileShader(vbs);
	glCompileShader(fbs);
	
	bprog = glCreateProgram();
	
	glAttachShader(bprog, fbs);
	glAttachShader(bprog, vbs);
	
	glLinkProgram(bprog);
	
	shaderLog(vbs);
	shaderLog(fbs);
	programLog(bprog);
	
	glUseProgram(bprog);
	int blurNum = glGetUniformLocation(bprog, "blurScale");
	glUniform1f(blurNum, 1.0 / double(texSize) * blurScale);
	glUseProgram(0);
}

#define LOGDEF(name,getIv, getLog) \
	void FrameBuffer::name(GLuint v) {\
		int logLen = 0; \
		int charCount = 0; \
		getIv(v, GL_INFO_LOG_LENGTH, &logLen); \
		if(logLen>0) {\
			char *infoLog = new char[logLen]; \
			getLog(v, logLen, &charCount, infoLog); \
			printf("%s\n", infoLog); \
			delete[] infoLog; \
		} \
	}

LOGDEF(shaderLog, glGetShaderiv, glGetShaderInfoLog)
LOGDEF(programLog, glGetProgramiv, glGetProgramInfoLog)

void FrameBuffer::clearTextures()
{
	for(int i=0; i<TextureMax; i++) {
		renderTarget((Texture)i);
		
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		endRender(false);
	}
}



void FrameBuffer::startRender()
{
	renderTarget(activeRender);
}

void FrameBuffer::renderTarget(Texture target)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glPushAttrib(GL_VIEWPORT_BIT | GL_ENABLE_BIT);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT+target);
	glViewport(0,0,scrW,scrH);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void FrameBuffer::runShader(GLuint shId, int numTex, GLuint texId[], const char* sampId[])
{
	glEnable(GL_TEXTURE_2D);
	glUseProgram(shId);
	
	for(int i=0; i<numTex; i++) {
		int sampNum = glGetUniformLocation(bprog, sampId[i]);
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, renderTex[texId[i]]);
		glUniform1i(sampNum, i);
	}
	
	fullscreenQuad();
	
	for(int i=0; i<numTex; i++) {
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	glActiveTexture(GL_TEXTURE0);
	
	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
}


void FrameBuffer::blendBuffers()
{
	renderTarget(activeRender);
	
	GLuint texId[] = 
		{
			Texture((activeRender-1+TexBufferMax) % TexBufferMax),
		};
	const char *sampId[] =
		{
			"srcTex"
		};
	
	runShader(bprog, 1, texId, sampId);
	
	endRender(false);
}

void FrameBuffer::endRender(bool flip)
{
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	if(flip) {
		activeRender = Texture((activeRender+1) % TexBufferMax);
		
		blendBuffers();
	}
}

void FrameBuffer::fullscreenQuad()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1,0,1,-1,1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		double wc = double(scrW) / double(texSize);
		double hc = double(scrH) / double(texSize);
		glTexCoord2f( 0, 0); glVertex2f(0,0);
		glTexCoord2f(wc, 0); glVertex2f(1,0);
		glTexCoord2f(wc,hc); glVertex2f(1,1);
		glTexCoord2f( 0,hc); glVertex2f(0,1);
	glEnd();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void FrameBuffer::screenBlit()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D, renderTex[activeRender]);
	fullscreenQuad();
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_TEXTURE_2D);
}
