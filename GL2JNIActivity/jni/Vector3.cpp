/*
 * Vector3.cpp
 *
 *  Created on: Sep 15, 2013
 *      Author: malvir
 */


#include "Vector3.h"
#include "Shader.h"



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

Vector3 Vector3::operator -(const Vector3& b) {
	return Vector3(this->x - b.x,this->y - b.y,this->z - b.z);
}

Vector3 Vector3::operator +(const Vector3& b) {
	return Vector3(this->x + b.x, this->y + b.y, this->z + b.z);
}

Vector3 Vector3::operator *(const float& b) {
	return Vector3(this->x*b,this->y*b,this->z*b);
}

float Vector3::length() {
	return sqrt(x*x + y*y + z*z);
}

Vector3 Vector3::operator -() {
	return Vector3(-x,-y,-z);
}

Vector3 Vector3::operator /(float s) {
	return Vector3(x/s,y/s,z/s);
}

float Vector3::operator *(const Vector3& b) {
	return (x*b.x + y*b.y + z*b.z);
}

Vector3 Vector3::operator ^(const Vector3& b) {
	return Vector3(
		y*b.z - z*b.y,
		z*b.x - x*b.z,
		x*b.y - y*b.x
		);
}

bool Vector3::operator ==(const Vector3& b) {
	return (x==b.x && y==b.y && z==b.z);
}

bool Vector3::operator !=(const Vector3& b) {
	return (x!=b.x && y!=b.y && z!=b.z);
}

bool Vector3::operator <(const Vector3& b) {
	return (x<b.x && y<b.y && z<b.z);
}

bool Vector3::operator <=(const Vector3& b) {
	return (x<=b.x && y<=b.y && z<=b.z);
}


void Vector3::operator *=(const float& b) {
	x*=b;
	y*=b;
	z*=b;
}

Vector3 Vector3::getNormal(const Vector3 &a, const Vector3 &b,const Vector3 &c)
{
 Vector3 v=crossProduct(Vector3(b.x-a.x,b.y-a.y,b.z-a.z),Vector3(c.x-a.x,c.y-a.y,c.z-a.z));
 v.normalize();
 return v;
}

 void Vector3::normalize()
 {
		float mag = length();
		if (mag > 0.0f) { // check for divide-by-zero
			float oneOverMag = 1.0f / sqrt(mag);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
}

void Vector3::print() {
	LOGI("V(%f,%f,%f)",x,y,z);
}

