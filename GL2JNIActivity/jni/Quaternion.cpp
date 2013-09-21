/*
 * Quaternion.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */

#include "Quaternion.h"

// Same as acos(x), but if x is out of range, it is "clamped" to the nearest
// valid value.  The value returned is in range 0...pi, the same as the
// standard C acos() function

float safeAcos(float x)
{
	if (x <= -1.0f) {
		return (22.0f/7.0f);
	}

	if (x >= 1.0f) {
		return 0.0f;
	}

	return acos(x);
}

void Quaternion::setInverse()
{
	x=-x;
	y=-y;
	z=-z;
}

void Quaternion::setToRotateAboutX(float theta) {

	// Compute the half angle

	float	thetahalf = theta * .5f;

	w = cos(thetahalf);
	x = sin(thetahalf);
	y = 0.0f;
	z = 0.0f;
}

void Quaternion::setToRotateAboutY(float theta) {

	// Compute the half angle

	float	thetahalf = theta * .5f;

	w = cos(thetahalf);
	x = 0.0f;
	y = sin(thetahalf);
	z = 0.0f;
}

void Quaternion::setToRotateAboutZ(float theta) {

	// Compute the half angle

	float	thetahalf = theta * .5f;

	w = cos(thetahalf);
	x = 0.0f;
	y = 0.0f;
	z = sin(thetahalf);
}

void Quaternion::rotateAboutX(float theta) {


	Quaternion t;
	t.setToRotateAboutX(theta);

	*this=(*this)*t;
}

void Quaternion::rotateAboutY(float theta) {

	Quaternion t;
	t.setToRotateAboutY(theta);

	*this=(*this)*t;
}
void Quaternion::rotateAboutZ(float theta) {

	Quaternion t;
	t.setToRotateAboutZ(theta);

	*this=(*this)*t;
}

void Quaternion::rotateAboutAxis(Vector3 &axis, float theta)
{
	Quaternion t;
		t.setToRotateAboutAxis(axis,theta);
		*this=(*this)*t;
}


void Quaternion::setToRotateAboutAxis(Vector3 &axis, float theta) {

	// The axis of rotation must be normalized

	axis.normalize();
	// Compute the half angle and its sin

	float	thetaOver2 = theta * .5f;
	float	sinThetaOver2 = sin(thetaOver2);

	// Set the values

	w = cos(thetaOver2);
	x = axis.x * sinThetaOver2;
	y = axis.y * sinThetaOver2;
	z = axis.z * sinThetaOver2;
}


// Quaternion cross product, which concatonates multiple angular
// displacements.  The order of multiplication, from left to right,
// corresponds to the order that the angular displacements are
// applied.
Quaternion Quaternion::operator *(const Quaternion &a) const {
	Quaternion result;

	result.w = w*a.w - x*a.x - y*a.y - z*a.z;
	result.x = w*a.x + x*a.w + z*a.y - y*a.z;
	result.y = w*a.y + y*a.w + x*a.z - z*a.x;
	result.z = w*a.z + z*a.w + y*a.x - x*a.y;

	return result;
}

void	Quaternion::normalize() {

	// to Compute magnitude of the quaternion

	float	mag = (float)sqrt(w*w + x*x + y*y + z*z);

	// Check for dummy length, to protect against divide by zero

	if (mag > 0.0f) {

		// Normalize it

		float	oneOverMag = 1.0f / mag;
		w *= oneOverMag;
		x *= oneOverMag;
		y *= oneOverMag;
		z *= oneOverMag;

	} else {


		//Give some default value

		identity();
	}
}

float	Quaternion::getRotationAngle() const {

	// Compute the half angle.  w = cos(theta / 2)

	float thetahalf = safeAcos(w);

	// Return the rotation angle

	return thetahalf * 2.0f;
}


// Return the rotation axis

Vector3	Quaternion::getRotationAxis() const {

	// Compute [sin(theta/2)]^2.  w = cos(theta/2),
	// and [sin(x)]^2 + [cos(x)]^2 = 1

	float sinThetahalfSq = 1.0f - w*w;

	// Protect against numerical imprecision

	if (sinThetahalfSq <= 0.0f) {

		// Identity quaternion, or numerical imprecision.  Just
		// return any valid vector, since it doesn't matter

		return Vector3(1.0f, 0.0f, 0.0f);
	}

	// Compute 1 / sin(theta/2)

	float	onebySinThetahalf = 1.0f / sqrt(sinThetahalfSq);

	// Return axis of rotation

	return Vector3(
		x * onebySinThetahalf,
		y * onebySinThetahalf,
		z * onebySinThetahalf
	);
}

Matrix44 Quaternion::getMatrix(){
	Matrix44 m;

	m.matData[0][0] = 1.0f - 2.0f * (this->y * this->y + this->z * this->z);
	m.matData[0][1] = 2.0f * (this->x * this->y - this->z * this->w);
	m.matData[0][2] = 2.0f * (this->z * this->x + this->y * this->w);
	m.matData[0][3] = 0.0f;

	m.matData[1][0] = 2.0f * (this->x * this->y + this->z * this->w);
	m.matData[1][1]= 1.0f - 2.0f * (this->z * this->z + this->x * this->x);
	m.matData[1][2] = 2.0f * (this->y * this->z - this->x * this->w);
	m.matData[1][3] = 0.0f;

	m.matData[2][0] = 2.0f * (this->z * this->x - this->y * this->w);
	m.matData[2][1] = 2.0f * (this->y * this->z + this->x * this->w);
	m.matData[2][2] = 1.0f - 2.0f * (this->y * this->y + this->x * this->x);
	m.matData[2][3] = 0.0f;

	m.matData[3][0] = 0.0f;
	m.matData[3][1] = 0.0f;
	m.matData[3][2] = 0.0f;
	m.matData[3][3] = 1.0f;

	return m;
}


