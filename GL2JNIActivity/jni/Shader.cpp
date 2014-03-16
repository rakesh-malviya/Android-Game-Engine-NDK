/*
 * Shader.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */

#include "Shader.h"

void Shader::checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }

}



GLuint Shader::loadShader(GLenum shaderType, const char* pSource) {
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

GLuint Shader::createProgram(const char* pVertexSource, const char* pFragmentSource) {
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


bool Shader::glCreateTriProgram()
{
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


		gvNormalHandle = glGetAttribLocation(gProgram, "vNormal");
			checkGlError("glGetAttribLocation");
			LOGI("glGetAttribLocation(\"vNormal\") = %d\n",
					gvNormalHandle);

	    gPMatrixHandle = glGetUniformLocation(gProgram, "projMatrix");
	    gMMatrixHandle = glGetUniformLocation(gProgram, "modelMatrix");

	    return true;
}

bool Shader::glCreateLineProgram()
{
    gLineProgram = createProgram(gLineVertexShader, gFragmentLineShader);
	if (!gProgram) {
		LOGE("Could not create program.");
		return false;
	}
	gvLinePositionHandle = glGetAttribLocation(gLineProgram, "vPosition");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
			gvLinePositionHandle);


	gLineColorHandle = glGetUniformLocation(gLineProgram,"uColor");
	gPLineMatrixHandle = glGetUniformLocation(gLineProgram, "projMatrix");
	gMLineMatrixHandle = glGetUniformLocation(gLineProgram, "modelMatrix");

	return true;
}




void Shader::drawTriShader(gameSpace::GameObject* obj,Matrix44 &modelMatrix,Matrix44 &projMatrix)
{
    glUseProgram(gProgram);
    checkGlError("glUseProgram");
    glUniformMatrix4fv(gMMatrixHandle,1,false,(const float*)(modelMatrix.matData[0]));
    //glUniformMatrix4fv(gMMatrixHandle,1,false,(const float*)(&Model[0][0]));
    glUniformMatrix4fv(gPMatrixHandle,1,false,(const float*)(projMatrix.matData[0]));
    checkGlError("glUniformMatrix4fv");
//    glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);

    glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, obj->mesh->vertexArray);
    glVertexAttribPointer(gvNormalHandle, 3, GL_FLOAT, GL_FALSE, 0, obj->mesh->normalArray);



    checkGlError("glVertexAttribPointer");
    glVertexAttribPointer(gvColorHandle,4,GL_FLOAT,GL_FALSE, 0, obj->colorArray);
    glEnableVertexAttribArray(gvPositionHandle);
    glEnableVertexAttribArray(gvColorHandle);
    glEnableVertexAttribArray(gvNormalHandle);

    checkGlError("glEnableVertexAttribArray");


    //glDrawArrays(GL_TRIANGLES, 0, 51);
    glDrawElements(GL_TRIANGLES,obj->mesh->nfaces*3,GL_UNSIGNED_SHORT,obj->mesh->indices);
    checkGlError("glDrawElements");
}



Shader::Shader(AAssetManager* assetMgr){
	AAsset* asset;
	long sizeAsset;
	asset = AAssetManager_open(assetMgr, "Shaders/triangle.vert", AASSET_MODE_STREAMING);
	sizeAsset = AAsset_getLength(asset);
	gVertexShader = (char*) malloc (sizeof(char)*sizeAsset + 1);
	AAsset_read (asset,gVertexShader,sizeAsset);
	gVertexShader[sizeAsset] = 0;
	AAsset_close(asset);

	asset = AAssetManager_open(assetMgr, "Shaders/triangle.frag", AASSET_MODE_STREAMING);
	sizeAsset = AAsset_getLength(asset);
	gFragmentShader = (char*) malloc (sizeof(char)*sizeAsset + 1);
	AAsset_read (asset,gFragmentShader,sizeAsset);
	gFragmentShader[sizeAsset] = 0;
	AAsset_close(asset);

	asset = AAssetManager_open(assetMgr, "Shaders/line.vert", AASSET_MODE_STREAMING);
	sizeAsset = AAsset_getLength(asset);
	gLineVertexShader = (char*) malloc (sizeof(char)*sizeAsset + 1);
	AAsset_read (asset,gLineVertexShader,sizeAsset);
	gLineVertexShader[sizeAsset] = 0;
	LOGE("%s\n",gLineVertexShader);
	AAsset_close(asset);

	asset = AAssetManager_open(assetMgr, "Shaders/line.frag", AASSET_MODE_STREAMING);
	sizeAsset = AAsset_getLength(asset);
	gFragmentLineShader = (char*) malloc (sizeof(char)*sizeAsset + 1);
	AAsset_read (asset,gFragmentLineShader,sizeAsset);
	gFragmentLineShader[sizeAsset] = 0;
	LOGE("%s\n",gFragmentLineShader);
	AAsset_close(asset);
}

/*
 Shader::Shader(char* dataDir) {
	unsigned int pathlength = strlen(dataDir);
	char* tempFilePath = new char[pathlength + 20];
	memset(tempFilePath,0,pathlength+20);
	strcpy(tempFilePath,dataDir);
	strcat(tempFilePath,"triangle.vert.txt");
	LOGE("FilePath: %s", tempFilePath);
	FILE *f = fopen(tempFilePath,"r");
	if(f==NULL){
		LOGE("file not  found");
		return;
	}
	else
	LOGE("Done");

	long dataSize;
	fseek(f, 0, SEEK_END);
	dataSize = ftell(f);
	rewind(f);
	gVertexShader = new char[dataSize+1];
	memset(gVertexShader,0,dataSize+1);
	fread(gVertexShader,sizeof(char),dataSize,f);
	//LOGE("%s",gVertexShader);
	fclose(f);

	memset(tempFilePath,0,pathlength+20);
	strcpy(tempFilePath,dataDir);
	strcat(tempFilePath,"triangle.frag.txt");
	LOGE("FilePath: %s", tempFilePath);
	f = fopen(tempFilePath,"r");
	if(f==NULL){
		LOGE("file not  found");
		return;
	}
	else
	LOGE("Done");

	fseek(f, 0, SEEK_END);
	dataSize = ftell(f);
	rewind(f);
	gFragmentShader = new char[dataSize+1];
	memset(gFragmentShader,0,dataSize+1);
	fread(gFragmentShader,sizeof(char),dataSize,f);
	//LOGE("%s",gFragmentShader);
	fclose(f);

	memset(tempFilePath,0,pathlength+20);
	strcpy(tempFilePath,dataDir);
	strcat(tempFilePath,"line.vert.txt");
	LOGE("FilePath: %s", tempFilePath);
	f = fopen(tempFilePath,"r");
	if(f==NULL){
		LOGE("file not  found");
		return;
	}
	else
	LOGE("Done");

	fseek(f, 0, SEEK_END);
	dataSize = ftell(f);
	rewind(f);
	gLineVertexShader = new char[dataSize+1];
	memset(gLineVertexShader,0,dataSize+1);
	fread(gLineVertexShader,sizeof(char),dataSize,f);
	//LOGE("%s",gLineVertexShader);
	fclose(f);

	memset(tempFilePath,0,pathlength+20);
	strcpy(tempFilePath,dataDir);
	strcat(tempFilePath,"line.frag.txt");
	//LOGE("FilePath: %s", tempFilePath);
	f = fopen(tempFilePath,"r");
	if(f==NULL){
		LOGE("file not  found");
		return;
	}
	else
	LOGE("Done");

	fseek(f, 0, SEEK_END);
	dataSize = ftell(f);
	rewind(f);
	gFragmentLineShader = new char[dataSize+1];
	memset(gFragmentLineShader,0,dataSize+1);
	fread(gFragmentLineShader,sizeof(char),dataSize,f);
	//LOGE("%s",gFragmentLineShader);
	fclose(f);

	free(tempFilePath);
}
 */
Shader::~Shader() {
	free(gVertexShader);
	free(gFragmentLineShader);
	free(gLineVertexShader);
	free(gFragmentShader);
}

void Shader::drawLineShader(gameSpace::GameObject* obj,Vector3 color,Matrix44 &modelMatrix,Matrix44 &projMatrix,Vector3 *l1)
{
	float vertexArray[6];
	vertexArray[0]=l1->x/3;vertexArray[1]=l1->y/3+2;vertexArray[2]=l1->z/3;
	vertexArray[3]=l1->x/3;vertexArray[4]=l1->y/3-2;vertexArray[5]=l1->z/3;
	glUseProgram(gLineProgram);
    checkGlError("glUseProgram");
    glUniform4f(gLineColorHandle,color.x,color.y,color.z,1);
    glUniformMatrix4fv(gMLineMatrixHandle,1,false,(const float*)(modelMatrix.matData[0]));
    //glUniformMatrix4fv(gMMatrixHandle,1,false,(const float*)(&Model[0][0]));
    glUniformMatrix4fv(gPLineMatrixHandle,1,false,(const float*)(projMatrix.matData[0]));
    checkGlError("glUniformMatrix4fvLine");
//    glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);

    glVertexAttribPointer(gvLinePositionHandle, 3, GL_FLOAT, GL_FALSE, 0, vertexArray);
    checkGlError("glVertexAttribPointerLine");
    glEnableVertexAttribArray(gvLinePositionHandle);
    checkGlError("glEnableVertexAttribArrayLine");

    glDrawArrays(GL_LINES, 0, 2);
    //glDrawElements(GL_TRIANGLES,man->mesh->nfaces*3,GL_UNSIGNED_SHORT,obj->mesh->indices);
    checkGlError("glDrawArrays");
}


