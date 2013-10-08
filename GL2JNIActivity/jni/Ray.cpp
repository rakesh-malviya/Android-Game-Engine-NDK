/*
 * Ray.cpp
 *
 *  Created on: Oct 6, 2013
 *      Author: malvir
 */

#include "Ray.h"

Ray::Ray() {
	// TODO Auto-generated constructor stub

}

Ray::Ray(Vector3 o, Vector3 d) {
	origin = o;
	direction = d;
	if(d.x == 0.0)
		d.x = 0.000001;
	if(d.y == 0.0)
		d.y = 0.000001;
	if(d.z == 0.0)
		d.z = 0.000001;
	inv_direction = Vector3(1/d.x, 1/d.y, 1/d.z);
	sign[0] = (inv_direction.x < 0);
	sign[1] = (inv_direction.y < 0);
	sign[2] = (inv_direction.z < 0);
}

Ray::Ray(const Ray& r) {
	origin = r.origin;
	direction = r.direction;
	inv_direction = r.inv_direction;
	sign[0] = r.sign[0]; sign[1] = r.sign[1]; sign[2] = r.sign[2];
}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

