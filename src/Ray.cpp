/*
 * Ray.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Ray.h"

Ray::Ray(){
	this->minDistance = 0;
	this->maxDistance = 0;
}

Ray::Ray(Vec4f position, Vec4f direction, float min,float max) {
	this->position = position;
	this->direction = direction;

	this->minDistance = min;
	this->maxDistance = max;
}

Ray::Ray(Vec3f position, Vec3f direction, float min,float max) {
	this->position = Vec4f(position,1.0f);
	this->direction = Vec4f(direction,0.0f);

	this->minDistance = min;
	this->maxDistance = max;
}

Vec4f Ray::getPosition() const{
	return position;
}

Vec4f Ray::getDirection() const{
	return direction;
}

std::ostream& operator<<(std::ostream &strm, const Ray &ray){
	return strm << "pos(" << ray.getPosition() << "), direction(" << ray.getDirection() << ")";
}
