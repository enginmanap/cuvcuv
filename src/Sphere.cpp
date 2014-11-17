/*
 * Sphere.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Sphere.h"


void Sphere::generateBoundingBox(){
	//calculate the bounding box for this sphere

	//we assume it was a sphere in 0,0,0 with radius 1, we will put real values in motion afterwards
	float xRadius, yRadius, zRadius;
	xRadius = sqrt(pow(this->transformMatrix.getElement(0,0),2) +pow(this->transformMatrix.getElement(1,0),2) + pow(this->transformMatrix.getElement(2,0),2));
	yRadius = sqrt(pow(this->transformMatrix.getElement(0,1),2) +pow(this->transformMatrix.getElement(1,1),2) +pow(this->transformMatrix.getElement(2,1),2));
	zRadius = sqrt(pow(this->transformMatrix.getElement(0,2),2) +pow(this->transformMatrix.getElement(1,2),2) +pow(this->transformMatrix.getElement(2,2),2));

	//now put real radius in use
	xRadius *=this->radius;
	yRadius *=this->radius;
	zRadius *=this->radius;

	//now calculate max and min points for bounding box. We are adding real position at this step
	float xMax = xRadius + this->position.x + this->transformMatrix.getElement(3,0);
	float yMax = yRadius + this->position.y + this->transformMatrix.getElement(3,1);
	float zMax = zRadius + this->position.z + this->transformMatrix.getElement(3,2);
	this->bbUpper = Vec3f(xMax,yMax,zMax);

	float xMin = -xRadius + this->position.x + this->transformMatrix.getElement(3,0);
	float yMin = -yRadius + this->position.y + this->transformMatrix.getElement(3,1);
	float zMin = -zRadius + this->position.z + this->transformMatrix.getElement(3,2);
	this->bbLower=Vec3f(xMin,yMin,zMin);

	float xCenter = (xMax + xMin) /2;
	float yCenter = (yMax + yMin) /2;
	float zCenter = (zMax + zMin) /2;
	this->bbCenter=Vec3f(xCenter,yCenter,zCenter);

	this->bbwidths = Vec3f(xRadius,yRadius,zRadius);
}

Sphere::Sphere(float x, float y, float z, float rad) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->radius = rad;
	generateBoundingBox();
}

Sphere::~Sphere() {

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
bool Sphere::intersectiontest(Ray ray, float& distance) const {
	Ray transformedRay = generateTransformedRay(ray);
	//the equation is:
	// t^2 * (P1 * P1) + 2 * t * P1 * (P0 - C) + (P0 -C)^2 -r^2 = 0
	Vec3f rayDirection = transformedRay.getDirection();
	float p1s = vec3fNS::dot(rayDirection, rayDirection);
	Vec3f rayPosition = transformedRay.getPosition();

	Vec3f p0MinusC = rayPosition - this->position;
	float p1timesp0mc = vec3fNS::dot(rayDirection, p0MinusC);
	//now the formula is this:
	//(distance * distance) * p1s + distance * (2*p1timesp0mc) + (p0MinusC * p0MinusC) - (this->radius * this->radius) = 0
	//Assign a,b,c so later parts should be easier to follow;

	float a = p1s;
	float b = 2 * p1timesp0mc;
	float c = vec3fNS::dot(p0MinusC, p0MinusC) - (this->radius * this->radius);

	//calculate the discriminant
	// discriminant = b^2 - 4ac
	float discriminant = b * b - 4 * a * c;
	//if d > 0, 2 solutions, if d=0 2 solutions are equal
	// if d < 0 no real solutions

	if (discriminant < EPSILON && discriminant > 0.0f) { // because float 0.0F is near impossible to get with calculation.
	//this means solutions are equal
		distance = -1 * b / (2 * a);
		if (distance < EPSILON) { //this means intersection within, we should pass
			return false;
		}
		return true;
	} else if (discriminant > 0) { //we know that it is not near 0, so positive is positive
		//solution1 = (-b + sqrt(discriminant)) / (2*a)
		//solution2  = (-b - sqrt(discriminant)) / (2*a)
		//we need to calculate both, because one positive, one negative is different then both positive
		float distance1 = (-1 * b + sqrt(discriminant)) / (2 * a);
		float distance2 = (-1 * b - sqrt(discriminant)) / (2 * a);
		if (distance1 * distance2 < 0) {
			//set the big one, because camera is in the sphere
			if (distance1 > distance2)
				distance = distance1;
			else
				distance = distance2;
		} else {
			//both solutions have same sign, we should eliminate negatives{
			if (distance1 < 0.0f)
				return false;
			//both solutions are positive, set the smaller one
			if (distance1 > distance2)
				distance = distance2;
			else
				distance = distance1;
		}
		//std::cout << "for transformedRay " << transformedRay.getPosition() << transformedRay.getDirection() << std::endl;
		//std::cout << "discriminant: " << discriminant<< ", a: "<< a << ", b: " << b << ", c: " << c << std::endl;
		//std::cout << "distances: "<< distance1 << " , " <<distance2 << std::endl;
		if (distance < EPSILON) { //this means intersection within, we should pass
			return false;
		}
		return true;
	} else { //at that point, discriminant is not near zero, and not positive, so it is negative aka no real solution.
		//std::cout << "negative disc" << std::endl;
		return false;
	}
	return false;
}

Vec3f Sphere::calculateNormal(const Vec4f& position) const {
	//calculate the normal, if there were no transformations, than inverse transpose the normal.
	Vec3f normal = ((Vec3f) (position * this->inverseTransformMat))	- this->position;
	return vec3fNS::normalize(Vec4f(normal, 0.0f) * this->inverseTransformMat.transpose());

}
