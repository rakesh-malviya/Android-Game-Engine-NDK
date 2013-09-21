/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <unistd.h>
#include <jni.h>
#include <android/log.h>
#include <android/input.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "Vector3.h"
#include "Matrix44.h"
#include "QuaternionCamera.h"
#include <string.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] = 
    "attribute vec4 vPosition;\n"
	"attribute vec4 vColor;\n"
	"varying vec4 vFinalColor;\n"
	"uniform mat4 projMatrix;\n"
	"uniform mat4 modelMatrix;\n"

    "void main() {\n"
    "  gl_Position = projMatrix*modelMatrix*vPosition;\n"
	" vFinalColor = vColor; \n "
    "}\n";

static const char gFragmentShader[] =
    "precision mediump float;\n"
	"varying vec4 vFinalColor;\n"
    "void main() {\n"
    "  gl_FragColor = vFinalColor;\n"
    "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = new char[infoLen];
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    delete[] buf;
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = new char[bufLength];
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    delete[] buf;
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}


//const GLfloat gTriangleVertices[] = { 0.0f, 50.0f, -1.0f , -50.0f, -50.0f, -1.0f ,
//        50.0f, -50.0f , -1.0f };
//
//const GLfloat colorArray[] = {
//									0,0,1,1,        1,0,0,1,
//									0,1,0,1};
//GLfloat stemp = 40.0f;
//GLfloat ztemp = -100.0f;
//GLfloat znear = -50.0f;
//const GLfloat gTriangleVertices[] =
//{
//		-stemp,stemp,ztemp,	stemp,stemp,ztemp,
//		-stemp,-stemp,ztemp,	-stemp,-stemp,ztemp,
//		stemp,stemp,ztemp,	stemp,-stemp,ztemp
//};
//
//const GLfloat colorArray[] = {
//									0,0,1,1,        1,0,0,1,
//									0,1,0,1,		0,1,0,1,
//									1,0,0,1,		0,0,1,1
//};
//const GLfloat gTriangleVertices[] = {
//		0.0f, stemp, ztemp , 		-stemp, -stemp, ztemp ,	    stemp, -stemp , ztemp,
//		0.0f, stemp, ztemp , 		-stemp, -stemp, ztemp ,	    0, 0 , znear,
//		0.0f, stemp, ztemp , 		0, 0 , znear,				    stemp, -stemp , ztemp,
//		0, 0 , znear,	 			-stemp, -stemp, ztemp ,	    stemp, -stemp , ztemp
//};
//
//const GLfloat colorArray[] = {
//		0,0,1,1,        1,0,0,1,		0,1,0,1,
//		0,0,1,1,        1,0,0,1,		1,1,1,1,
//		0,0,1,1,        1,1,1,1,		0,1,0,1,
//		1,1,1,1,        1,0,0,1,		0,1,0,1,
//};
GLfloat zDiff =0;
const GLfloat gTriangleVertices[] = {
		0.0f, 0.0f, 60.0f-zDiff,		-15.0f, 0.0f, 30.0f-zDiff,	15.0f,0.0f,30.0f-zDiff,
		15.0f, 0.0f,  30.0f-zDiff,	0.0f,  15.0f, 30.0f-zDiff,	0.0f,  0.0f,  60.0f-zDiff,
		0.0f, 0.0f, 60.0f-zDiff,		0.0f, 15.0f, 30.0f-zDiff,		-15.0f, 0.0f, 30.0f-zDiff,
		-15.0f, 0.0f, 30.0f-zDiff,	0.0f, 15.0f, 30.0f-zDiff,		0.0f, 0.0f, -56.0f-zDiff,
		0.0f, 0.0f, -56.0f-zDiff,		0.0f, 15.0f, 30.0f-zDiff,		15.0f,0.0f,30.0f-zDiff,
		15.0f,0.0f,30.0f-zDiff,		-15.0f, 0.0f, 30.0f-zDiff,	0.0f, 0.0f, -56.0f-zDiff,
		0.0f,2.0f,27.0f-zDiff,		-60.0f, 2.0f, -8.0f-zDiff,	60.0f, 2.0f, -8.0f-zDiff,
		60.0f, 2.0f, -8.0f-zDiff,		0.0f, 7.0f, -8.0f-zDiff,		0.0f,2.0f,27.0f-zDiff,
		60.0f, 2.0f, -8.0f-zDiff,		-60.0f, 2.0f, -8.0f-zDiff,	0.0f,7.0f,-8.0f-zDiff,
		0.0f,2.0f,27.0f-zDiff,		0.0f, 7.0f, -8.0f-zDiff,		-60.0f, 2.0f, -8.0f-zDiff,
		-30.0f, -0.50f, -57.0f-zDiff,		30.0f, -0.50f, -57.0f-zDiff,			0.0f,-0.50f,-40.0f-zDiff,
		0.0f,-0.5f,-40.0f-zDiff,			30.0f, -0.5f, -57.0f-zDiff,			0.0f, 4.0f, -57.0f-zDiff,
		0.0f, 4.0f, -57.0f-zDiff,			-30.0f, -0.5f, -57.0f-zDiff,			0.0f,-0.5f,-40.0f-zDiff,
		30.0f,-0.5f,-57.0f-zDiff,			-30.0f, -0.5f, -57.0f-zDiff,			0.0f, 4.0f, -57.0f-zDiff,
		0.0f,0.5f,-40.0f-zDiff,			3.0f, 0.5f, -57.0f-zDiff,				0.0f, 25.0f, -65.0f-zDiff,
		0.0f, 25.0f, -65.0f-zDiff,		-3.0f, 0.5f, -57.0f-zDiff,			0.0f,0.5f,-40.0f-zDiff,
		3.0f,0.5f,-57.0f-zDiff,			-3.0f, 0.5f, -57.0f-zDiff,			0.0f, 25.0f, -65.0f-zDiff
};



const GLfloat colorArray[] = {
		1,0,0,1,	0,1,0,1,	0,0,1,1,
		0,0,1,1,	0,1,1,1,	1,0,0,1,
		1,0,0,1,	0,1,1,1,	0,1,0,1,
		0,1,0,1,	0,1,1,1,	1,1,0,1,
		1,1,0,1,	0,1,1,1,	0,0,1,1,
		0,0,1,1,	0,1,0,1,	1,1,0,1,
		1,0,1,1,	0,1,0,1,	1,0,1,1,
		1,0,1,1,	0,1,1,1,	0,1,0,1,
		1,0,1,1,	0,1,0,1,	0,1,1,1,
		0,1,0,1,	0,1,1,1,	0,1,0,1,
		1,1,0,1,	0,1,0,1,	0,1,1,1,
		0,1,1,1,	0,1,0,1,	0,0,1,1,
		0,0,1,1,	1,1,0,1,	0,1,1,1,
		0,1,0,1,	1,1,0,1,	0,0,1,1,
		0,1,1,1,	0,0,1,1,	1,0,0,1,
		1,0,0,1,	1,0,1,1,	0,1,1,1,
		0,0,1,1,	1,0,1,1,	1,0,0,1
};



GLuint gProgram;
GLuint gvPositionHandle;
GLuint gvColorHandle;
GLuint gPMatrixHandle;
GLuint gMMatrixHandle;
GLfloat angle;
GLboolean touchStarted;
Matrix44 *projMatrix;
Matrix44 modelMatrix;
QuaternionCamera jetCam;
Quaternion jetQuat;
char* dataDir;
Matrix44 tranformationMatrix;

static GLfloat xMousePos,yMousePos;
static GLuint windowWidth,windowHeight;

float degree2Radian(float deg)
{
	return (M_PI*deg/180.0);
}

float between1minus1(int x,int divide)
{
	return (((float)x/((float)divide/2.0f))-1.0f);
}

bool onTouchEvent(int eventId, int x, int y)
{
	switch (eventId) {
		case AMOTION_EVENT_ACTION_DOWN:
			yMousePos=y;
			xMousePos=x;
			touchStarted = true;
			break;
		case AMOTION_EVENT_ACTION_UP:
			touchStarted = false;
			break;
		case AMOTION_EVENT_ACTION_MOVE:
			if(!(y==yMousePos && x==xMousePos ))
				jetCam.updateQuaternion(between1minus1(x,windowWidth),between1minus1(y,windowHeight),between1minus1(xMousePos,windowWidth),between1minus1(yMousePos,windowHeight));

			yMousePos=y;
			xMousePos=x;
			LOGE("Mouse Pos=(%d,%d)",(int)x,(int)y);
			break;
	}
	return true;
}


bool setup()
{
	touchStarted = false;
	projMatrix = new Matrix44();
	angle=0;

	tranformationMatrix.setTranslate(0,0,-150);

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet

	return true;
}

bool setupGraphics(int w, int h) {

	windowHeight = h;
	windowWidth = w;
	//setPerspective(45,0.001,100,w,h);
	projMatrix->setAsPerspective(45,1.0,225,w,h);
	printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);

    gvColorHandle = glGetAttribLocation(gProgram, "vColor");
        checkGlError("glGetAttribLocation");
        LOGI("glGetAttribLocation(\"vColor\") = %d\n",
                gvColorHandle);

    gPMatrixHandle = glGetUniformLocation(gProgram, "projMatrix");
    gMMatrixHandle = glGetUniformLocation(gProgram, "modelMatrix");
    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}


void renderFrame() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(0, 1, 1, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");


    if(touchStarted)
	{
    	angle+=1;
    	Vector3 temp(0,0,1);
    	jetQuat.setToRotateAboutAxis(temp,degree2Radian(angle));
//    	setRotation(angle);
		//jetCam.getRotMat((float*)&Model[0][0]);
	}

    modelMatrix = tranformationMatrix*(jetQuat.getMatrix());

    glUseProgram(gProgram);
    checkGlError("glUseProgram");
    glUniformMatrix4fv(gMMatrixHandle,1,false,(const float*)(modelMatrix.matData[0]));
    glUniformMatrix4fv(gPMatrixHandle,1,false,(const float*)(projMatrix->matData[0]));
    checkGlError("glUniformMatrix4fv");
    glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glVertexAttribPointer(gvColorHandle,4,GL_FLOAT,GL_FALSE, 0, colorArray);
    glEnableVertexAttribArray(gvPositionHandle);
    glEnableVertexAttribArray(gvColorHandle);
    checkGlError("glEnableVertexAttribArray");


    glDrawArrays(GL_TRIANGLES, 0, 51);
    checkGlError("glDrawArrays");
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSurfaceChanged(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onDrawFrame(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSurfaceCreated(JNIEnv * env, jobject obj);
    JNIEXPORT jint JNICALL Java_com_android_gl2jni_GL2JNILib_onTouchEvent(JNIEnv * env, jobject obj, jint eventId, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setFilePath(JNIEnv * env, jobject obj, jstring filePath);
};



JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSurfaceChanged(JNIEnv * env, jobject obj,  jint width, jint height)
{
	setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onDrawFrame(JNIEnv * env, jobject obj)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSurfaceCreated(JNIEnv * env, jobject obj)
{
    setup();
}

JNIEXPORT jint JNICALL Java_com_android_gl2jni_GL2JNILib_onTouchEvent(JNIEnv * env, jobject obj, jint eventId, jfloat x, jfloat y)
{
	onTouchEvent(eventId,x,y);
	return 0;
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setFilePath(JNIEnv * env, jobject obj, jstring fP)
{
	dataDir = (char*)(env->GetStringUTFChars(fP,NULL));
	strcat(dataDir,"triangle.vert.txt");
	LOGE("FilePath: %s", dataDir);
	FILE *f = fopen(dataDir,"r");
	if(f==NULL){
		LOGE("file not  found");
		return;
	}
	else
	LOGE("Done");

	char * shaderData;
	long dataSize;
	fseek(f, 0, SEEK_END);
	dataSize = ftell(f);
	rewind(f);
	shaderData = new char[dataSize+1];
	fread(shaderData,sizeof(char),dataSize,f);
	LOGE("%s",shaderData);
	fclose(f);
}