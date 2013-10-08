/*
 * LineBoxIntersection.h
 *
 *  Created on: Oct 8, 2013
 *      Author: malvir
 */

#ifndef LINEBOXINTERSECTION_H_
#define LINEBOXINTERSECTION_H_

#include "Vector3.h"

class LineBoxIntersection {
public:
	static int GetIntersection( float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	static int InBox( Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);
	static int CheckLineBox( Vector3 B1, Vector3 B2, Vector3 L1, Vector3 L2, Vector3 &Hit);
	LineBoxIntersection();
	virtual ~LineBoxIntersection();
};

#endif /* LINEBOXINTERSECTION_H_ */
