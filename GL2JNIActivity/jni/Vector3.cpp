/*
 * Vector3.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */


#include "Vector3.h"


 Vector3 Vector3::crossProduct(const Vector3 &a, const Vector3 &b) {
	return Vector3(
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
		);
 }



 float Vector3::dotProduct(const Vector3& a, const Vector3& b) {
	 return (a.x*b.x + a.y*b.y + a.z*b.z);
}

Vector3 Vector3::getNormal(const Vector3 &a, const Vector3 &b,const Vector3 &c)
 {
	 Vector3 v=crossProduct(Vector3(b.x-a.x,b.y-a.y,b.z-a.z),Vector3(c.x-a.x,c.y-a.y,c.z-a.z));
	 v.normalize();
	 return v;
 }

 void Vector3::normalize()
 {
		float magSq = x*x + y*y + z*z;
		if (magSq > 0.0f) { // check for divide-by-zero
			float oneOverMag = 1.0f / sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
}
