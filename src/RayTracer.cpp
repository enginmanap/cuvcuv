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
	Vec4f route = ray.getPosition() - light.getPosition();
	Vec4f routeN =  route.normalize();
	float distance =  route.x / routeN.x;
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
		return intersectingPrimitive->getColorForRay(ray, distance, lights);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

