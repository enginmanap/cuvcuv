/*
 * Ray.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Ray.h"


Ray::Ray(Vec3f& position, Vec3f& direction, float min,float max):position(Vec4f(position,1.0f)), direction(Vec4f(direction,0.0f)), minDistance(min), maxDistance(max) {
	generateInverseDirection();
}

std::ostream& operator<<(std::ostream &strm, const Ray &ray){
	return strm << "pos(" << ray.getPosition() << "), direction(" << ray.getDirection() << ")";
}
