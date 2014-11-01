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


/**
 * This function gets a ray, calculates if there is an intersection with sphere,
 * if there is returns true, else returns false.
 * if there is a intersection, distance is the intersection distance,
 * if there is no intersection, distance is not altered.
 *
 * params:
 * 		Ray: ray to test intersection
 * 		float: distance to intersect
 *
 * 	returns:
 * 		bool: if there is an intersection
 */
bool Sphere::intersectiontest(Ray ray, float& distance, Vec3f& intersectionPoint){
	//the equation is:
	// t^2 * (P1 * P1) + 2 * t * P1 * (P0 - C) + (P0 -C)^2 -r^2 = 0
	float p1s = Vec3f::dot(ray.getDirection(), ray.getDirection());
	Vec3f p0MinusC = ray.getPosition() - this->position;
	float p1timesp0mc = Vec3f::dot(ray.getDirection(), p0MinusC);
	float t;
	//now the formula is this:
	//(t * t) * p1s + t * (2*p1timesp0mc) + (p0MinusC * p0MinusC) - (this->radius * this->radius) = 0

	//calculate the discriminant
	// discriminant = b^2 - 4ac
	float dis= (2*p1timesp0mc) * (2*p1timesp0mc) - ( 4* (p1s * ((Vec3f::dot(p0MinusC, p0MinusC)) - (this->radius * this->radius))));
	//if d > 0, 2 solutions, if d=0 2 solutions are equal
	// if d < 0 no real solutions

	if(dis > 0){
		return true;
	} else if (dis == 0){
		return true;
	} else {
		return false;
	}

	return false;
}

Vec3f Sphere::getColorForRay(Ray ray, Vec3f intersectedPoint) {
	return Vec3f(255.0f,0.2f,0.2f);
}
