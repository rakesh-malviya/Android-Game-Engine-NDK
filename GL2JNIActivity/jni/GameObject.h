/*
 * GameObject.h
 *
 *  Created on: Sep 13, 2013
 *      Author: malvir
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "Quaternion.h"
#include "OFFviewer.h"
#include "Matrix44.h"

namespace gameSpace {

class GameObject {

private:
	Quaternion q;
	Vector3 position;
	Vector3 centroid;
public:
	Mesh* mesh;

	void	setToRotateAboutX(float theta);
	void	setToRotateAboutY(float theta);
	void	setToRotateAboutZ(float theta);
	void	setToRotateAboutAxis(Vector3 &axis, float theta);

	void	rotateAboutX(float theta);
	void	rotateAboutY(float theta);
	void	rotateAboutZ(float theta);
	void	rotateAboutAxis(Vector3 &axis, float theta);
	void 	setPosition(float x,float y,float z);
	void 	setOrigin(float x,float y,float z);
	Matrix44 	getPosMatrix();
	Matrix44 	getCentroidMatrix();
	Matrix44	getQuatMatrix();

	GameObject(char* dataFilePath);

	virtual ~GameObject();
};



} /* namespace gameSpace */
#endif /* GAMEOBJECT_H_ */
