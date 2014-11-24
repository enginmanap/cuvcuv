/*
 * Ray.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Ray.h"


Ray::Ray(Vec3f& position, Vec3f& direction, float min,float max) {
	this->position = Vec4f(position,1.0f);
	this->direction = Vec4f(direction,0.0f);

	this->inverseDirection = Vec4f(1.0f/this->direction.x, 1.0f/this->direction.y, 1.0f/this->direction.z, 0.0f);

	this->minDistance = min;
	this->maxDistance = max;
}

std::ostream& operator<<(std::ostream &strm, const Ray &ray){
	return strm << "pos(" << ray.getPosition() << "), direction(" << ray.getDirection() << ")";
}
