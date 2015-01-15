/*
 * RayTracer.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "RayTracer.h"

#ifndef LIGHT_SIZE
#define LIGHT_SIZE 10.0f
#endif //LIGHT_SIZE

RayTracer::RayTracer(char shadowGridSize) :
		shadowGridSize(shadowGridSize) {
	float halfSize = LIGHT_SIZE / 2;
	if (shadowGridSize > 2) { //since 2x2 means randomize this
		corners[0][0] = -1 * halfSize;
		corners[0][1] = -1 * halfSize; //check lower left
		corners[1][0] = halfSize;
		corners[1][1] = halfSize; //check upper right
		corners[2][0] = -1 * halfSize;
		corners[2][1] = halfSize; //upper left
		corners[3][0] = halfSize;
		corners[3][1] = -1 * halfSize; //lower right
	}
}

bool RayTracer::isLightVisible(const Vec4f& intersectionPoint,
		const Octree& octree, const Light& light, float offsetX,
		float offsetY) const {
	Vec3f direction;
	Vec4f lightPosition = light.getPosition();
	float distanceToLight =
			((Vec3f) (lightPosition - intersectionPoint)).length();
	lightPosition.x += offsetX;
	lightPosition.y += offsetY;
	if (fabs(lightPosition.w) < EPSILON) {
		direction = lightPosition;
	} else {
		Vec4f lightPos = lightPosition;
		lightPos = (1 / lightPosition.w) * lightPos;
		direction = lightPos - intersectionPoint;
	}

	Ray rayToLight(intersectionPoint, direction.normalize(), 0, 100);

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
	float visibility = 0.0f;

	if (isLightVisible(intersectionPoint, octree, light, 0, 0)) {
		visibility = 1.0;
	}

	//set the corners specific values, so they can be tested beforehand
	if (shadowGridSize > 2) {
		for (unsigned int i = 0; i < 4; ++i) {
			if (isLightVisible(intersectionPoint, octree, light, corners[i][0],
					corners[i][1])) {
				visibility += 1.0;
			}
		}
		if (visibility == 5.0) { //if all corners and center see light, no one will be blocked
			//std::cout << "all visible " << std::endl;
			return 1.0f;
		} else if (visibility == 0) { // if none of them see light, no one will see either
			//std::cout << "all blocked" << std::endl;
			return 0.0f;
		}
	}
	//at this point we know there is a penumbra we calculate light
	if (shadowGridSize >= 2) {
		float gridSize = LIGHT_SIZE / shadowGridSize;

		float offsetX, offsetY;
		visibility -= 1.0;			//remove center
		for (unsigned char i = 0; i < shadowGridSize; ++i) {
			for (unsigned char j = 0; j < shadowGridSize; ++j) {
				//pass the corners
				if (!((i == 0 && j == 0) || (i == shadowGridSize - 1 && j == 0)
						|| (i == 0 && j == shadowGridSize - 1)
						|| (i == shadowGridSize - 1 && j == shadowGridSize - 1))) {
					offsetX = ((rand() / float(RAND_MAX + 1) * gridSize)
							+ gridSize * i) - LIGHT_SIZE / 2;
					offsetY = ((rand() / float(RAND_MAX + 1) * gridSize)
							+ gridSize * j) - LIGHT_SIZE / 2;

					if (isLightVisible(intersectionPoint, octree, light,
							offsetX, offsetY)) {
						visibility += 1.0;
					}
				}
			}
		}
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

