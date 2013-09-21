/*
 * Matrix44.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */

#include "Matrix44.h"

Matrix44 Matrix44::operator *(const Matrix44 &b){
	Matrix44 r;
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			r.matData[i][j]=0;
			for(int k=0;k<4;k++)
			{
				r.matData[i][j] += (this->matData[i][k])*(b.matData[k][j]);
			}
		}
	}
	return r;
}

void Matrix44::operator *=(const Matrix44 &b){
	Matrix44 r;
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				r.matData[i][j]=0;
				for(int k=0;k<4;k++)
				{
					r.matData[i][j] += (this->matData[i][k])*(b.matData[k][j]);
				}
		}
	}

	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			this->matData[i][j]=r.matData[i][j];
		}
	}

	return;
}

void Matrix44::setTranslate(float x, float y, float z)
{
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			this->matData[i][j]=0;
	}

	this->matData[3][0]=x;
	this->matData[3][1]=y;
	this->matData[3][2]=z;
	this->matData[0][0]=1;
	this->matData[1][1]=1;
	this->matData[2][2]=1;
	this->matData[3][3]=1;
}



void Matrix44::setAsPerspective(float fov, float near, float far, float w , float h){
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
			this->matData[i][j]=0;
	}

	float aspectRatio  = w/h;
	h = near * tanf(fov * 3.1415927 / 360.0);
	w = h*aspectRatio;
	this->matData[0][0] = near/w;
	this->matData[1][1] = near/h;
	this->matData[2][2] = -1*(far+near)/(far-near);
	this->matData[3][2] = -1*(2*far*near)/(far-near);
	this->matData[2][3] = -1;
}
