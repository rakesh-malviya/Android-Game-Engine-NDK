/*
 * RayBoxIntersection.cpp
 *
 *  Created on: Oct 6, 2013
 *      Author: malvir
 */

#include "RayBoxIntersection.h"


RayBoxIntersection::RayBoxIntersection() {
	// TODO Auto-generated constructor stub

}

bool RayBoxIntersection::intersect(Vector3 minBox, Vector3 maxBox, const Ray& r,
		float t0, float t1) {
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	  Vector3 parameters[2];
	  parameters[0] = minBox;
	  parameters[1] = maxBox;
	  tmin = (parameters[r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	  tmax = (parameters[1-r.sign[0]].x - r.origin.x) * r.inv_direction.x;
	  tymin = (parameters[r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	  tymax = (parameters[1-r.sign[1]].y - r.origin.y) * r.inv_direction.y;
	  if ( (tmin > tymax) || (tymin > tmax) )
	    return false;
	  if (tymin > tmin)
	    tmin = tymin;
	  if (tymax < tmax)
	    tmax = tymax;
	  tzmin = (parameters[r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	  tzmax = (parameters[1-r.sign[2]].z - r.origin.z) * r.inv_direction.z;
	  if ( (tmin > tzmax) || (tzmin > tmax) )
	    return false;
	  if (tzmin > tmin)
	    tmin = tzmin;
	  if (tzmax < tmax)
	    tmax = tzmax;
	  return ( (tmin < t1) && (tmax > t0) );
}

RayBoxIntersection::~RayBoxIntersection() {
	// TODO Auto-generated destructor stub
}

