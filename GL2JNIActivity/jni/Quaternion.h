#ifndef _QUATERNION_H
#define _QUATERNION_H

#include<math.h>
#include"Vector3.h"
#include"Matrix44.h"

class Quaternion {
public:
	float w,x,y,z;

	//Default as identity
	Quaternion(){w = 1.0f; x = y = z = 0.0f; }

	Quaternion(float w,float x,float y,float z)
	{
		this->w=w;
		this->x=x;
		this->y=y;
		this->z=z;
	}
	void	identity() { w = 1.0f; x = y = z = 0.0f; }

	void    setInverse();

	//functions for rotation
	void	setToRotateAboutX(float theta);
	void	setToRotateAboutY(float theta);
	void	setToRotateAboutZ(float theta);
	void	setToRotateAboutAxis(Vector3 &axis, float theta);

	void	rotateAboutX(float theta);
	void	rotateAboutY(float theta);
	void	rotateAboutZ(float theta);
	void	rotateAboutAxis(Vector3 &axis, float theta);
	Matrix44 getMatrix();

	Quaternion operator *(const Quaternion &a) const;

	void	normalize();

	float	getRotationAngle() const;
	Vector3	getRotationAxis() const;
};


#endif
