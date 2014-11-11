/*
 * Sphere.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Sphere.h"
#include "RayTracer.h"

Sphere::Sphere(float x, float y, float z, float rad) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->radius = rad;

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
			//both solutions have same sign, we should eliminate negatives{
			if(distance1 < 0.0f)
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
		return true;
	} else { //at that point, discriminant is not near zero, and not positive, so it is negative aka no real solution.
		//std::cout << "negative disc" << std::endl;
		return false;
	}
	return false;
}

Vec3f Sphere::getColorForRay(const Ray ray, float distance,
		const std::vector<Primitive*>& primitives,
		const std::vector<Light>& lights, const unsigned int depth) const {
	Vec3f color;

	RayTracer tracer;

	Vec4f intersectionPoint = distance * ray.getDirection();
	intersectionPoint = intersectionPoint + ray.getPosition();

	//calculate the normal, if there were no transformations, than inverse transpose the normal.
	Vec3f normal = ((Vec3f) (intersectionPoint * this->inverseTransformMat))
			- this->position;
	normal = Vec4f(normal, 0.0f) * this->inverseTransformMat.transpose();
	normal = vec3fNS::normalize(normal);
	Vec4f normal4(normal,0.0f);
	Vec3f eyeDirn = vec3fNS::normalize(
			((Vec3f) ray.getPosition()) - intersectionPoint);

	for (unsigned int i = 0; i < lights.size(); i++) {
		Light it = lights[i];
		Vec3f lightPos;
		Vec3f direction;
		lightPos.x = it.getPosition().x;
		lightPos.y = it.getPosition().y;
		lightPos.z = it.getPosition().z;
		if (fabs(it.getPosition().w) < EPSILON) {
			direction = vec3fNS::normalize(lightPos);
		} else {
			lightPos = (1 / it.getPosition().w) * lightPos;
			direction = vec3fNS::normalize(lightPos - intersectionPoint);
		}
		//check if light is blocked or not

		Ray rayToLight(intersectionPoint + EPSILON * normal4, direction, 0, 100);
		//if(rayToLight.getPosition().getElement(0) * rayToLight.getPosition().getElement(0) +rayToLight.getPosition().getElement(1) * rayToLight.getPosition().getElement(1) + rayToLight.getPosition().getElement(2) * rayToLight.getPosition().getElement(2) < 1)
		//std::cout << "ray origin (" << rayToLight.getPosition() << ") direction (" << rayToLight.getDirection()<< ")" <<std::endl;

		if (tracer.traceToLight(rayToLight, primitives, *(&it))) {
			Vec3f halfVec = vec3fNS::normalize(direction + eyeDirn);

			color = color
					+ calculateColorPerLight(direction, it.getColor(), normal,
							halfVec, diffuse, specular, shininess);
		}

	}
	//now we have the color for this object itself, calculate reflections.
	if (fabs(this->specular.x) < EPSILON && fabs(this->specular.y) < EPSILON
			&& fabs(this->specular.z) < EPSILON) {
		//the object is not reflective, so stop here
	} else {
		if (depth < MAX_DEPTH) {

			Ray reflectionRay(intersectionPoint + EPSILON * normal4,
					ray.getDirection()
							- 2 * Vec4fNS::dot(ray.getDirection(), normal4)
									* normal4, 0, 100);
			Vec3f reflectedColor = tracer.trace(reflectionRay, primitives,
					lights, depth);
			reflectedColor = vec3fNS::clamp(reflectedColor, 0, 1);
			//std::cout << "reflection " << reflectedColor << std::endl;
			color = color + specular * reflectedColor;

		}
	}
	color = color + ambientLight;
	//Opengl auto clamps, we should do it manually;
	return vec3fNS::clamp(color, 0, 1); //TODO move clamping to last step, just before writing the pixel.
}
