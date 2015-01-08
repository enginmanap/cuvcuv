/*
 * RayTracer.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "RayTracer.h"

RayTracer::RayTracer() {

}

RayTracer::~RayTracer() {

}

/**
 * returns true if the ray can reach light with out hitting
 * any objects. false if there are any obstacles.
 *
 */
bool RayTracer::traceToLight(const Vec4f& intersectionPoint, const Octree& octree,
		const Light& light) const {
	Vec4f direction;

	if (fabs(light.getPosition().w) < EPSILON) {
		direction = light.getPosition();
	} else {
		Vec4f lightPos = light.getPosition();
		lightPos = (1 / light.getPosition().w) * lightPos;
		direction = lightPos - intersectionPoint;
	}



	direction = direction.normalize();
	Ray rayToLight(intersectionPoint,
			direction, 0, 100);
	float distanceToLight = ((Vec3f)(light.getPosition() - intersectionPoint)).length();

	float intersectionDistance;
	Primitive* placeHolder = NULL; //this primitive will not be used, but it is required
	std::set<Primitive*> primitives;
	octree.getIntersectingPrimitives(rayToLight,primitives);
	for (std::set<Primitive*>::const_iterator it = primitives.begin();
			it != primitives.end(); ++it) {
		if ((*it)->intersectiontest(rayToLight, intersectionDistance,&placeHolder)) {
			//found intersection, check if it is before the closest one
			if (distanceToLight > intersectionDistance) {
				return true; //if we have one blocking object, it is enough
			}

		}
	}
	return false;
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
	Primitive* closestIntersectingPrimitive=NULL;

	std::set<Primitive*> primitives;
	octree.getIntersectingPrimitives(ray,primitives);
	if (!primitives.empty()) {
		float intersectionDistance;
		//totalTests += primitives.size();
		//if (primitives.size() > mostTests) {
		//	std::cout << "maximum tests " << primitives.size() << std::endl;
		//	mostTests = primitives.size();
		//}
		for (std::set<Primitive*>::const_iterator it = primitives.begin();
				it != primitives.end(); ++it) {
			intersectingPrimitive=NULL;//this will be determined if intersection is with a collection or single
			if ((*it)->intersectiontest(ray, intersectionDistance,&intersectingPrimitive)) {
				//found intersection
				if (distance > intersectionDistance) {
					distance = intersectionDistance;
					if(intersectingPrimitive != NULL){//if null, the intersection was with a single form, if not null, intersection collection
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
		return closestIntersectingPrimitive->getColorForRay(ray, distance, octree,
				lights, depth - 1);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

