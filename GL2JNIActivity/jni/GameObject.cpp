/*
 * GameObject.cpp
 *
 *  Created on: Sep 13, 2013
 *      Author: malvir
 */

#include "GameObject.h"

namespace gameSpace {


GameObject::GameObject() {
	// TODO Auto-generated constructor stub

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


GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
}

} /* namespace gameSpace */
