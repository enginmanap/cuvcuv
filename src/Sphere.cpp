/*
 * Sphere.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Sphere.h"

Sphere::Sphere(float x, float y, float z, float rad, Vec3f ambientLight) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;

	this->radius = rad;

	this->ambientLight = ambientLight;
}

Sphere::~Sphere() {
	// TODO Auto-generated destructor stub
}

bool Sphere::setAmbientLight(Vec3f ambientLight) {
	this->ambientLight = ambientLight;
	return true;
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
bool Sphere::intersectiontest(Ray ray, float& distance) {
	//the equation is:
	// t^2 * (P1 * P1) + 2 * t * P1 * (P0 - C) + (P0 -C)^2 -r^2 = 0
	float p1s = Vec3f::dot(ray.getDirection(), ray.getDirection());
	Vec3f p0MinusC = ray.getPosition() - this->position;
	float p1timesp0mc = Vec3f::dot(ray.getDirection(), p0MinusC);
	//now the formula is this:
	//(distance * distance) * p1s + distance * (2*p1timesp0mc) + (p0MinusC * p0MinusC) - (this->radius * this->radius) = 0
	//Assign a,b,c so later parts should be easier to follow;

	float a = p1s;
	float b = 2 * p1timesp0mc;
	float c = Vec3f::dot(p0MinusC, p0MinusC) - (this->radius * this->radius);

	//calculate the discriminant
	// discriminant = b^2 - 4ac
	float discriminant = b * b - 4 * a * c;
	//if d > 0, 2 solutions, if d=0 2 solutions are equal
	// if d < 0 no real solutions

	if (fabs(discriminant) < EPSILON) { // because float 0.0F is near impossible to get with calculation.
		//this means solutions are equal
		distance = -1 * b / (2 * a);
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
			//both solutions are positive, set the bigger one
			if (distance1 > distance2)
				distance = distance2;
			else
				distance = distance1;
		}
		//std::cout << "for ray " << ray.getPosition() << ray.getDirection() << std::endl;
		//std::cout << "distance is " << distance << std::endl;
		return true;
	} else { //at that point, discriminant is not near zero, and not positive, so it is negative aka no real solution.
		return false;
	}

	return false;
}

Vec3f Sphere::calculateColorPerLight(const Vec3f direction, const Vec3f color,
		const Vec3f normal, const Vec3f halfVec, const Vec3f diffuse,
		const Vec3f specular, const float shininess) {

	Vec3f lambert, phong;

	float nDotl = Vec3f::dot(normal, direction);

	if (nDotl > 0.0f) {
		lambert = diffuse * color;
		//lambert = Vec3f(diffuse.x * color.x,diffuse.y * color.y,diffuse.z * color.z);
		lambert = nDotl * lambert;
	}

	float nDotH = Vec3f::dot(normal, halfVec);
	if (nDotH > 0.0f) {
		phong = specular * color;
		//phong = Vec3f(specular.x * color.x,specular.y * color.y,specular.z * color.z);
		float temp = (float) pow(nDotH, shininess);
		phong = temp * phong;
	}
	return lambert + phong;

}

Vec3f Sphere::getColorForRay(Ray ray, float distance) {
	Vec3f intersectionPoint = distance * ray.getDirection();
	intersectionPoint = intersectionPoint + ray.getPosition();

	//hardcoding a light for calculation
	Vec3f lightPos(3, 10, -1);
	Vec3f lightColor(0.0f, 1.0f, 0.5f);
	Vec3f diffuse(0.5f, 0.5f, 0.5f);
	Vec3f specular(1, 1, 1);

	Vec3f normalisedLightPos = Vec3f::normalize(lightPos);
	Vec3f normal = Vec3f::normalize(intersectionPoint - this->position);
	Vec3f eyeDirn = Vec3f::normalize(ray.getPosition() - intersectionPoint);
	Vec3f halfVec = Vec3f::normalize(eyeDirn + normalisedLightPos);
	Vec3f color = calculateColorPerLight(normalisedLightPos, lightColor, normal,
			halfVec, diffuse, specular, 100.0f);
	//Opengl auto clamps, we should do it manually;
	return Vec3f::clamp(color+ ambientLight,0,1);
}
