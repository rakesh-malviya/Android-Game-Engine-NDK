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
	float scale;
public:
	Mesh* mesh;
	float* colorArray;

	void	setToRotateAboutX(float theta);
	void	setToRotateAboutY(float theta);
	void	setToRotateAboutZ(float theta);
	void	setToRotateAboutAxis(Vector3 &axis, float theta);

	void	rotateAboutX(float theta);
	void	rotateAboutY(float theta);
	void	rotateAboutZ(float theta);
	void	rotateAboutAxis(Vector3 &axis, float theta);
	void 	setPosition(float x,float y,float z);
	void 	setCentroid(float x,float y,float z);
	void 	setScale(float scale);
	void 	setColorArray(float *colorArray);
	Matrix44 	getPosMatrix();
	Matrix44 	getCentroidMatrix();
	Matrix44	getScaleMatrix();
	Matrix44	getQuatMatrix();
	Matrix44    getMatrix();

	GameObject(char* dataFilePath);
	GameObject(Mesh* m);
	virtual ~GameObject();
};



} /* namespace gameSpace */
#endif /* GAMEOBJECT_H_ */
