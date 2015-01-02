/*
 * Triangle.cpp
 *
 *  Created on: 2 Oca 2015
 *      Author: engin
 */

#include "Triangle.h"

Vec3f Triangle::calculateNormal(const Vec4f& intersectionPoint) const{
	if(!smoothShading){
		return triangleNormal;
	} else {
		/**
		 * since we have the intersection point, we will assume it is
		 * in  the triangle plane, and interpolate based on distance
		 */
		Vec3f intersection = intersectionPoint;
		Vec3f distance1 = this->a - intersection;
		Vec3f distance2 = this->b - intersection;
		Vec3f distance3 = this->c - intersection;
		Vec3f totalDistance = distance1 + distance2 + distance3;
		Vec3f normal = distance1.length() * n1 + distance2.length() * n2 + distance3.length() * n3;
		normal = normal * (1 / totalDistance.length());// a dirt hack to
		return normal;
	}

}

