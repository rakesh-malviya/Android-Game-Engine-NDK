/*
 * RayBoxIntersection.h
 *
 *  Created on: Oct 6, 2013
 *      Author: malvir
 */

#ifndef RAYBOXINTERSECTION_H_
#define RAYBOXINTERSECTION_H_

#include "Vector3.h"
#include "Ray.h"

class RayBoxIntersection {
public:
	static bool intersect(Vector3 minBox,Vector3 maxBox,const Ray &r, float t0, float t1);
	RayBoxIntersection();
	virtual ~RayBoxIntersection();
};

#endif /* RAYBOXINTERSECTION_H_ */
