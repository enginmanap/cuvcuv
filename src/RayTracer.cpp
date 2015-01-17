/*
 * RayTracer.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "RayTracer.h"

#ifndef LIGHT_SIZE
#define LIGHT_SIZE 5.0f
#endif //LIGHT_SIZE

RayTracer::RayTracer(char shadowGridSize) :
		shadowGridSize(shadowGridSize) {
}

bool RayTracer::isLightVisible(const Ray& rayToLight, const float distanceToLight, const Octree& octree) const {
	float intersectionDistance;
	Primitive* placeHolder = NULL; //this primitive will not be used, but it is required
	std::set<Primitive*> primitives;
	octree.getIntersectingPrimitives(rayToLight, primitives);
	for (std::set<Primitive*>::const_iterator it = primitives.begin();
			it != primitives.end(); ++it) {
		if ((*it)->intersectiontest(rayToLight, intersectionDistance,
				&placeHolder)) {
			//found intersection, check if it is before the closest one
			if (distanceToLight > intersectionDistance) {
				return false;
			}

		}
	}
	return true;
}

/**
 * returns true if the ray can reach light with out hitting
 * any objects. false if there are any obstacles.
 *
 */
float RayTracer::traceToLight(const Vec4f& intersectionPoint,
		const Octree& octree, const Light& light) const {
	//calculate the mo
	//we should cast number of rays, and average the visibility results
	Vec3f direction;
	Vec4f lightPosition = light.getPosition();
	float distanceToLight =
			((Vec3f) (lightPosition - intersectionPoint)).length();
	if (fabs(lightPosition.w) < EPSILON) {
		direction = lightPosition;
	} else {
		Vec4f lightPos = lightPosition;
		lightPos = (1 / lightPosition.w) * lightPos;
		direction = lightPos - intersectionPoint;
	}
	//calculate max derivation
	float maxDerivation = (LIGHT_SIZE/2)/distanceToLight;
	std::vector<Ray>shadowRays = Ray::generateDeriveredRays(intersectionPoint,direction,shadowGridSize,maxDerivation);

	float visibility = 0.0f;

	if (isLightVisible(shadowRays[0], distanceToLight, octree)) {
		visibility = 1.0;
	}

	if (shadowGridSize > 1) {
		//we start from 1 because 0 is already tested
		for (unsigned char i = 1; i < shadowGridSize*shadowGridSize; ++i) {
			if (isLightVisible(shadowRays[i], distanceToLight, octree)) {
				visibility += 1.0;
			}
			if(i == 4){
				if (visibility == 5.0) { //if all corners and center see light, no one will be blocked
					//std::cout << "all visible " << std::endl;
					return 1.0f;
				} else if (visibility == 0) { // if none of them see light, no one will see either
					//std::cout << "all blocked" << std::endl;
					return 0.0f;
				}
			}
		}
		visibility -= 1.0;//remove center
	}
	//std::cout << "vis: " << visibility << std::endl;
	return visibility / (shadowGridSize * shadowGridSize);//since we calculate gridsize^2 rays, and result must be between 0-1
}

/**
 * traces ray and returns the color for it.
 *
 * @params:
 * ray: Ray to trace
 * Octree: tree that contains Objects.
 * lights: Vector of lights.
 * depth: How many reflections we should trace.
 *
 * @returns:
 * color in Vec3f
 */
Vec3f RayTracer::trace(const Ray& ray, const Octree& octree,
		const std::vector<Light> &lights, const unsigned int depth) const {
	//static unsigned int totalTests = 0, mostTests = 0;
	float distance = std::numeric_limits<float>::max(); // this is the maximum value float can have, min() returns min positive value.

	Primitive* intersectingPrimitive = NULL;
	Primitive* closestIntersectingPrimitive = NULL;

	std::set<Primitive*> primitives;
	octree.getIntersectingPrimitives(ray, primitives);
	if (!primitives.empty()) {
		float intersectionDistance;
		//totalTests += primitives.size();
		//if (primitives.size() > mostTests) {
		//	std::cout << "maximum tests " << primitives.size() << std::endl;
		//	mostTests = primitives.size();
		//}
		for (std::set<Primitive*>::const_iterator it = primitives.begin();
				it != primitives.end(); ++it) {
			intersectingPrimitive = NULL;//this will be determined if intersection is with a collection or single
			if ((*it)->intersectiontest(ray, intersectionDistance,
					&intersectingPrimitive)) {
				//found intersection
				if (distance > intersectionDistance) {
					distance = intersectionDistance;
					if (intersectingPrimitive != NULL) {//if null, the intersection was with a single form, if not null, intersection collection
						closestIntersectingPrimitive = intersectingPrimitive;
					} else {
						closestIntersectingPrimitive = *it;
					}

				}

			}
		}
	}
	//std::cout << "total tests " << totalTests<< std::endl;
	if (closestIntersectingPrimitive != NULL) {
		return closestIntersectingPrimitive->getColorForRay(ray, distance,
				octree, lights, depth - 1, this);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

