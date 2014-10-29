/*
 * Ray.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Ray.h"

Ray::Ray(Vec3f position, Vec3f direction, float min,float max) {
	this->position = position;
	this->direction = direction;

	this->minDistance = min;
	this->maxDistance = max;
}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

