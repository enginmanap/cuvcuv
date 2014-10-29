/*
 * Sphere.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Sphere.h"

Sphere::Sphere(float x, float y , float z, float rad) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;

	this->radius = rad;
}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

bool Sphere::intersectiontest(Ray){
	return false;
}
