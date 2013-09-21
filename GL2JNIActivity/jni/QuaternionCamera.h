/*
 * QuaternionCamera.h
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */

#ifndef QUATERNIONCAMERA_H_
#define QUATERNIONCAMERA_H_

#include"Quaternion.h"
#include"trackball.h"



class QuaternionCamera
{
	public:
	Vector3 camPos;
	float q[4];

	QuaternionCamera()
	{
		camPos=Vector3(0,0,0);
		q[0]=0;
		q[1]=0;
		q[2]=0;
		q[3]=1;
	}

	void updateQuaternion( float p1x, float p1y, float p2x, float p2y);

	void getRotMat(float *m);
	void moveForward(float grad);
	void moveBackward(float grad);
	//void moveLeft(float grad);
	//void moveRight(float grad);


};


#endif /* QUATERNIONCAMERA_H_ */
