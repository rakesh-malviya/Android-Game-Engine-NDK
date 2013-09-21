/*
 * GameObject.h
 *
 *  Created on: Sep 13, 2013
 *      Author: malvir
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Quaternion.h"

namespace gameSpace {

class GameObject {

private:
	Quaternion q;
public:


	void	setToRotateAboutX(float theta);
	void	setToRotateAboutY(float theta);
	void	setToRotateAboutZ(float theta);
	void	setToRotateAboutAxis(Vector3 &axis, float theta);

	void	rotateAboutX(float theta);
	void	rotateAboutY(float theta);
	void	rotateAboutZ(float theta);
	void	rotateAboutAxis(Vector3 &axis, float theta);

	GameObject();

	virtual ~GameObject();
};



} /* namespace gameSpace */
#endif /* GAMEOBJECT_H_ */
