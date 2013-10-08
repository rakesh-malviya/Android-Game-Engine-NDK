/*
 * Shader.h
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */

#ifndef SHADER_H_
#define SHADER_H_


#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <android/log.h>
#include "GameObject.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

class Shader
{
private:
	void checkGlError(const char* op);
public:

	 GLuint gProgram;
	 GLuint gLineProgram;
	 GLuint gvLinePositionHandle;
	 GLuint gLineColorHandle;
	 GLuint gPLineMatrixHandle;
	 GLuint gMLineMatrixHandle;
	 GLuint gvPositionHandle;
	 GLuint gvNormalHandle;
	 GLuint gvColorHandle;
	 GLuint gPMatrixHandle;
	 GLuint gMMatrixHandle;

	Shader(char* dataDir);
	~Shader();

	 char* gVertexShader;


	 char* gFragmentShader;


	 char* gLineVertexShader;

	 char* gFragmentLineShader;


	 GLuint loadShader(GLenum shaderType, const char* pSource);
	 GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
	 bool glCreateTriProgram();
	 bool glCreateLineProgram();
	 void drawTriShader(gameSpace::GameObject* obj,Matrix44 &modelMatrix,Matrix44 &projMatrix);
	 void drawLineShader(gameSpace::GameObject* obj,Vector3 color,Matrix44 &modelMatrix,Matrix44 &projMatrix,Vector3 *l1,Vector3 *l2);
};
#endif /* SHADER_H_ */
