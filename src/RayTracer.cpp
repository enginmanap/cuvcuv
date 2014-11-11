/*
 * RayTracer.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "RayTracer.h"

RayTracer::RayTracer() {

}

RayTracer::~RayTracer() {

}

bool RayTracer::traceToLight(const Ray ray,
		const std::vector<Primitive*> &Primitives, const Light light) const {
	Vec4f route = light.getPosition() - ray.getPosition();
	float distance =  route.length();
	//std::cout << "distance to light " << distance << std::endl;
	float intersectionDistance;
	Primitive* intersectingPrimitive = NULL;

	for (std::vector<Primitive*>::const_iterator it = Primitives.begin();
			it != Primitives.end(); it++) {
		if ((*it)->intersectiontest(ray, intersectionDistance)) {
			//found intersection
			if (distance > intersectionDistance) {
				//std::cout << "hit primitive within" << intersectionDistance << std::endl;
				distance = intersectionDistance;
				intersectingPrimitive = *it;
			}

		}
	}

	if (intersectingPrimitive != NULL) {
		return false;
	} else {
		return true;
	}

}

Vec3f RayTracer::trace(const Ray ray,
		const std::vector<Primitive*> &Primitives, const std::vector<Light> &lights, const unsigned int depth) const {
	float distance = std::numeric_limits<float>::max(); // this is the maximum value float can have, min() returns min positive value.
	float intersectionDistance;
	Primitive* intersectingPrimitive = NULL;

	for (std::vector<Primitive*>::const_iterator it = Primitives.begin();
			it != Primitives.end(); it++) {
		if ((*it)->intersectiontest(ray, intersectionDistance)) {
			//found intersection
			if (distance > intersectionDistance) {
				distance = intersectionDistance;
				intersectingPrimitive = *it;
			}

		}
	}

	if (intersectingPrimitive != NULL) {
		return intersectingPrimitive->getColorForRay(ray, distance, Primitives, lights, depth-1);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

