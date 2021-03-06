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
#include "GameObject.h"
#include "Shader.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#define FAR 600



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



GLboolean touchStarted;
Matrix44 projMatrix;
Matrix44 modelMatrix;
Matrix44 camQuatMatrix;
QuaternionCamera jetCam;
AAssetManager* assetMgr;
char* dataDir;
float* ManColorArray;
const unsigned int noOfEnemies = 3;
gameSpace::GameObject *man[3];
float xPos = 0;
float zPos = -100.0;

Vector3* camEye1;
Vector3* camEye;
Shader* gameShader;

static GLfloat xMousePos,yMousePos;
static GLuint windowWidth,windowHeight;

double randDouble(double x)
{
    return x*((double)rand() / (double)RAND_MAX) ;
}

void longPressEvent()
{
	for(int i=0;i<noOfEnemies;i++)
	{
		if(man[i]->onCrossHair)
		{
			double thetaRandom = randDouble(M_PI*2);
			man[i]->setPosition(zPos*sinf(thetaRandom),0,zPos*cosf(thetaRandom));
		}
	}


}

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
			{
				float camCalib = 0.5f;
				float normX = -between1minus1(x,windowWidth)*camCalib;
				float normY = -between1minus1(y,windowHeight)*camCalib;
				float normPrevX = -between1minus1(xMousePos,windowWidth)*camCalib;
				float normPrevY = -between1minus1(yMousePos,windowHeight)*camCalib;
				jetCam.updateQuaternion(normX,normY,normPrevX,normPrevY);
			}


			yMousePos=y;
			xMousePos=x;
			break;
	}
	return true;
}



bool setup()
{
	camEye1 = new Vector3(0,0,0);
	camEye = new Vector3(0,0,0);
	touchStarted = false;
	gameShader = new Shader(assetMgr);
	unsigned int pathLength = strlen(dataDir);
	char* manFile = new char[pathLength + 30];
	memset(manFile,0,pathLength + 30);
	strcpy(manFile,dataDir);
	strcat(manFile,"m39.off.txt");
	man[0] = new gameSpace::GameObject(manFile);
	man[0]->setCentroid(0.154467,0.4f,0.083040);
	man[0]->setScale(50);
	man[0]->setPosition(xPos,0,zPos);

	for(int i=1;i<noOfEnemies;i++)
	{
		man[i] = new gameSpace::GameObject(man[0]->mesh);
		man[i]->setCentroid(0.154467,0.4f,0.083040);
		man[i]->setScale(50);
		double thetaRandom = randDouble(M_PI*2);
		man[i]->setPosition(zPos*sinf(thetaRandom),0,zPos*cosf(thetaRandom));
	}
	//Seting color

	ManColorArray  = new float[(man[0]->mesh->nverts)*4];
	for(int i=0;i<man[0]->mesh->nverts;i++)
	{
		ManColorArray[i*4+0] = (float)(rand()%25)/100.0;
		ManColorArray[i*4+1] = (float)(rand()%50)/100.0 + 0.5;
		ManColorArray[i*4+2] = (float)(rand()%25)/100.0;
		ManColorArray[i*4+3] = 1.0;
	}


	for(int i=0;i<noOfEnemies;i++)
	{
		man[i]->setColorArray(ManColorArray);
	}

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet
	glCullFace(GL_BACK);

	free(manFile);
	return true;
}

bool setupGraphics(int w, int h) {

	//set Model
	jetCam.getRotMat((float*)&camQuatMatrix.matData[0][0]);


	windowHeight = h;
	windowWidth = w;
	projMatrix.setAsPerspective(75,1.0,FAR,w,h);
	printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);

    gameShader->glCreateTriProgram();
    gameShader->glCreateLineProgram();

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
    	jetCam.getRotMat((float*)&camQuatMatrix.matData[0][0]);
   	}

    for(int i=0;i<noOfEnemies;i++)
	{
    	modelMatrix = man[i]->getMatrix()*camQuatMatrix;

		gameShader->drawTriShader(man[i],modelMatrix,projMatrix);

	}

//	modelMatrix = man->getMatrix()*camQuatMatrix;
//
//	gameShader->drawTriShader(man,modelMatrix,projMatrix);
//
//	modelMatrix = man1->getMatrix()*camQuatMatrix;
//
//	gameShader->drawTriShader(man1,modelMatrix,projMatrix);


	camEye1->x = 0;
	camEye1->y = 0;
	camEye1->z = 0;
	camEye->x = 0;
	camEye->y = 0;
	camEye->z = -FAR;

	Quaternion inverseCam(jetCam.q[3],-jetCam.q[0],-jetCam.q[1],-jetCam.q[2]);
	inverseCam.getMatrix().multiply(camEye1);
	inverseCam.getMatrix().multiply(camEye);
	//camQuatMatrix.multiply(camEye);
	glLineWidth(2);

	//detect collision
	Vector3 crosshairColor(0,0,1);

	for(int i=0;i<noOfEnemies;i++)
	{
		if(man[i]->collision(*camEye))
		{
			LOGE("man Collision");
			crosshairColor.x = 1;
			crosshairColor.y = 0;
			crosshairColor.z = 0;
			man[i]->onCrossHair = true;
		}
		else{
			man[i]->onCrossHair = false;
		}
	}




//	if(man1->collision(*camEye))
//	{
//		LOGE("man1 Collision");
//		crosshairColor.x = 1;
//		crosshairColor.y = 0;
//		crosshairColor.z = 0;
//		man1->onCrossHair = true;
//	}
//	else{
//		man1->onCrossHair = false;
//	}


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelMatrix=camQuatMatrix;//.setIdentity();
	gameShader->drawLineShader(man[0],crosshairColor,modelMatrix,projMatrix,camEye);
	glDisable(GL_BLEND);




}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSurfaceChanged(JNIEnv * env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onDrawFrame(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onLongPress(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSurfaceCreated(JNIEnv * env, jobject obj);
    JNIEXPORT jint JNICALL Java_com_android_gl2jni_GL2JNILib_onTouchEvent(JNIEnv * env, jobject obj, jint eventId, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setFilePath(JNIEnv * env, jobject obj, jstring filePath);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_createAssets(JNIEnv * env, jobject obj, jobject assetManager);
};



JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSurfaceChanged(JNIEnv * env, jobject obj,  jint width, jint height)
{
	setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onDrawFrame(JNIEnv * env, jobject obj)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onLongPress(JNIEnv * env, jobject obj)
{
    longPressEvent();
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
	LOGE("FilePath: %s", dataDir);

}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_createAssets(JNIEnv * env, jobject obj, jobject assetManager)
{
	assetMgr = AAssetManager_fromJava(env, assetManager);
	assert(NULL != assetMgr);

}
