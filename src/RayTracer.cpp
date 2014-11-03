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

Vec3f RayTracer::trace(const Ray ray,
		const std::vector<Primitive*> &Primitives) const {
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
		return intersectingPrimitive->getColorForRay(ray, distance);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

