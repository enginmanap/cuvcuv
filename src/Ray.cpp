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

Ray::Ray(Vec3f position, Vec3f direction, float min,float max) {
	this->position = position;
	this->direction = direction;

	this->minDistance = min;
	this->maxDistance = max;
}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

Vec3f Ray::getPosition() const{
	return position;
}

Vec3f Ray::getDirection() const{
	return direction;
}

std::ostream& operator<<(std::ostream &strm, const Ray &ray){
	return strm << "pos(" << ray.getPosition() << "), direction(" << ray.getDirection() << ")";
}
