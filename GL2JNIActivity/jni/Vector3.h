#ifndef _VECTOR3_H
#define _VECTOR3_H

#include<math.h>

class Vector3
{
public:
	float x,y,z;

	Vector3(){};

	Vector3(float x,float y,float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	};

	Vector3(const Vector3 &v)
	{
		this->x=v.x;this->y=v.y;this->z=v.z;
	};

	void normalize();
	float length();
	Vector3 operator-();
	Vector3 operator/(float s);
	float operator*(const Vector3 &b);
	Vector3 operator^(const Vector3 &b);
	bool operator==(const Vector3 &b);
	bool operator!=(const Vector3 &b);
	bool operator<(const Vector3 &b);
	bool operator<=(const Vector3 &b);
	Vector3 operator-(const Vector3 &b);
	Vector3 operator+(const Vector3 &b);
	Vector3 operator*(const float &b);
	void operator*=(const float &b);

	Vector3 crossProduct(const Vector3 &a, const Vector3 &b);
	float dotProduct(const Vector3 &a, const Vector3 &b);
	Vector3 getNormal(const Vector3 &a, const Vector3 &b,const Vector3 &c);
	void print();

};



#endif
