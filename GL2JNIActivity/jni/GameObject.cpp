/*
 * GameObject.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: malvir
 */

#include "GameObject.h"
#include "RayBoxIntersection.h"
#include "LineBoxIntersection.h"
#include "Shader.h"

namespace gameSpace {


GameObject::GameObject(char* dataFilePath) {
	mesh = ReadOffFile(dataFilePath);
	scale = 1.0f;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	centroid.x = 0;
	centroid.y = 0;
	centroid.z = 0;
	onCrossHair = false;

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

void GameObject::setCentroid(float x, float y, float z) {
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

void GameObject::setScale(float scale) {
	this->scale=scale;
}

Matrix44 GameObject::getScaleMatrix() {
	Matrix44 m;
	m.setScale(scale);
	return m;
}

Matrix44 GameObject::getMatrix() {
	return(getCentroidMatrix()*getScaleMatrix()*getQuatMatrix()*getPosMatrix());
}


GameObject::GameObject(Mesh* m) {
	this->mesh = m;
	scale = 1.0f;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	centroid.x = 0;
	centroid.y = 0;
	centroid.z = 0;
}

void GameObject::setColorArray(float* colorArray) {
	this->colorArray = colorArray;
}



bool GameObject::collision(Vector3 p2) {

	Vector3 p1(0,0,0);

	Vector3*  b1 = new Vector3(mesh->maxX,mesh->maxY,mesh->maxZ);
	Vector3*  b2 = new Vector3(mesh->minX,mesh->minY,mesh->minZ);
	Matrix44 mult = getCentroidMatrix()*getScaleMatrix();//*getPosMatrix();
	mult.multiply(b1);
	mult.multiply(b2);

	Matrix44 vmult = getInversePosMatrix()*getInverseQuatMatrix();//*getPosMatrix();
	vmult.multiply(&p1);
	vmult.multiply(&p2);

	Vector3 hit;
	if(LineBoxIntersection::CheckLineBox(*b2,*b1,p1,p2,hit))
		return true;
	else
		return false;
}

Matrix44 GameObject::getInverseQuatMatrix() {
	Quaternion inverseq(q.w,-q.x,-q.y,-q.z);
	return inverseq.getMatrix();
}

Matrix44 GameObject::getInversePosMatrix() {
	Matrix44 m;
	m.setTranslate(-position.x,-position.y,-position.z);
	return m;
}

Matrix44 GameObject::getInverseCentroidMatrix() {
	Matrix44 m;
	m.setTranslate(centroid.x,centroid.y,centroid.z);
	return m;
}

Matrix44 GameObject::getInverseScaleMatrix() {
	Matrix44 m;
	if(scale!=0)
	m.setScale(1.0/scale);
	return m;
}

GameObject::~GameObject() {
}
/* namespace gameSpace */

}

