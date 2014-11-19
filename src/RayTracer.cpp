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

bool RayTracer::traceToLight(const Ray ray, const Octree& octree,
		const Light light) const {
	Vec4f route = light.getPosition() - ray.getPosition();
	float distance = route.length();
	//std::cout << "distance to light " << distance << std::endl;
	float intersectionDistance;
	Primitive* intersectingPrimitive = NULL;

	std::set<Primitive*> primitives = octree.getIntersectingPrimitives(ray);
	for (std::set<Primitive*>::const_iterator it = primitives.begin();
			it != primitives.end(); it++) {
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

Vec3f RayTracer::trace(const Ray ray, const Octree& octree,
		const std::vector<Light> &lights, const unsigned int depth) const {
	static unsigned int totalTests = 0, mostTests = 0;
	float distance = std::numeric_limits<float>::max(); // this is the maximum value float can have, min() returns min positive value.
	float intersectionDistance;
	Primitive* intersectingPrimitive = NULL;

	std::set<Primitive*> primitives = octree.getIntersectingPrimitives(ray);
	if (primitives.size() > 0) {
		totalTests += primitives.size();
		if (primitives.size() > mostTests) {
			std::cout << "maximum tests " << primitives.size() << std::endl;
			mostTests = primitives.size();
		}
		for (std::set<Primitive*>::const_iterator it = primitives.begin();
				it != primitives.end(); it++) {
			if ((*it)->intersectiontest(ray, intersectionDistance)) {
				//found intersection
				if (distance > intersectionDistance) {
					distance = intersectionDistance;
					intersectingPrimitive = *it;
				}

			}
		}
	}
	std::cout << "total tests " << totalTests<< std::endl;
	if (intersectingPrimitive != NULL) {
		return intersectingPrimitive->getColorForRay(ray, distance, octree,
				lights, depth - 1);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

