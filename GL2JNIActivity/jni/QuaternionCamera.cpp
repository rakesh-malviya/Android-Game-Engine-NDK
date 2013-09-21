/*
 * QuaternionCamera.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */

#include "QuaternionCamera.h"

void QuaternionCamera::updateQuaternion( float p1x, float p1y, float p2x, float p2y)
{
	float qRot[4];
	trackball(qRot,-p1x,p1y,-p2x,p2y);
	float qTemp[4];
	qTemp[0]=q[0];
	qTemp[1]=q[1];
	qTemp[2]=q[2];
	qTemp[3]=q[3];

	add_quats(qRot,qTemp,this->q);
	normalize_quat(q);

}

void QuaternionCamera::getRotMat(float* rotMat)
{
	build_rotmatrix(rotMat,q);

	return;
}

void QuaternionCamera::moveBackward(float grad)
{
	this->camPos.z=this->camPos.z-grad;
}

void QuaternionCamera::moveForward(float grad)
{
	this->camPos.z=this->camPos.z+grad;
}
