/*
 * Ray.h
 *
 *  Created on: Oct 6, 2013
 *      Author: malvir
 */

#ifndef RAY_H_
#define RAY_H_

#include "Vector3.h"
class Ray {
public:
	Vector3 origin;
	Vector3 direction;
	Vector3 inv_direction;
	int sign[3];
	Ray();
	Ray(Vector3 o,Vector3 d);
	Ray(const Ray &r);
	virtual ~Ray();
};

#endif /* RAY_H_ */
