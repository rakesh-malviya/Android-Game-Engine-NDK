/*
 * GameObject.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: malvir
 */

#include "GameObject.h"

namespace gameSpace {


GameObject::GameObject(char* dataFilePath) {
	mesh = ReadOffFile(dataFilePath);
	position.x = 0;
	position.y = 0;
	position.z = 0;
	centroid.x = 0;
	centroid.y = 0;
	centroid.z = 0;
}



void GameObject::setToRotateAboutX(float theta) {
	q.setToRotateAboutX(theta);
}

void GameObject::setToRotateAboutY(float theta) {
	q.setToRotateAboutY(theta);
}

void GameObject::setToRotateAboutZ(float theta) {
	q.setToRotateAboutZ(theta);
}

void GameObject::setToRotateAboutAxis(Vector3& axis, float theta) {
	q.setToRotateAboutAxis(axis,theta);
}

void GameObject::rotateAboutX(float theta) {
	q.rotateAboutX(theta);
}

void GameObject::rotateAboutY(float theta) {
	q.rotateAboutY(theta);
}

void GameObject::rotateAboutZ(float theta) {
	q.rotateAboutZ(theta);
}

void GameObject::rotateAboutAxis(Vector3& axis, float theta) {
	q.rotateAboutAxis(axis,theta);
}

void GameObject::setPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void GameObject::setOrigin(float x, float y, float z) {
	centroid.x = x;
	centroid.y = y;
	centroid.z = z;
}

Matrix44 GameObject::getPosMatrix() {
	Matrix44 m;
	m.setTranslate(position.x,position.y,position.z);
	return m;
}

Matrix44 GameObject::getCentroidMatrix() {
	Matrix44 m;
	m.setTranslate(-centroid.x,-centroid.y,-centroid.z);
	return m;
}

Matrix44 GameObject::getQuatMatrix() {
	return q.getMatrix();
}


GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
}

} /* namespace gameSpace */
