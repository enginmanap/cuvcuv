/*
 * RayTracer.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "RayTracer.h"

RayTracer::RayTracer(char shadowGridSize) :
		shadowGridSize(shadowGridSize) {
}

bool RayTracer::isLightVisible(const Ray& rayToLight, const double distanceToLight, const Octree& octree) const {
	double intersectionDistance;
	Primitive* placeHolder = NULL; //this primitive will not be used, but it is required
	std::set<Primitive*> primitives;
	octree.getIntersectingPrimitives(rayToLight, primitives);
	for (std::set<Primitive*>::const_iterator it = primitives.begin();
			it != primitives.end(); ++it) {
		if ((*it)->intersectiontest(rayToLight, intersectionDistance,
				&placeHolder)) {
			//found intersection, check if it is before the closest one
			if (distanceToLight > intersectionDistance) {
				//FIXME now we need to update this
				if((*it)->getRefractionIndex() == 1.0)
					return false;
			}

		}
	}
	return true;
}

/**
 * Generates shadowGrid^2 rays, and casts them.
 * returns 1 if all the rays can reach light with out hitting
 * any objects. returns 0 if none of them reaches, all elements
 * between gets equal weight.
 *
 */
double RayTracer::traceToLight(const Vec4f& intersectionPoint, const Light& light,
		double refractionIndex, const Octree& octree) const {

	//we should cast number of rays, and average the visibility results
	Vec3f direction;
	Vec4f lightPosition = light.getPosition();
	double distanceToLight =
			((Vec3f) (lightPosition - intersectionPoint)).length();
	if (fabs(lightPosition.w) < EPSILON) {
		direction = lightPosition;
	} else {
		Vec4f lightPos = lightPosition;
		lightPos = (1 / lightPosition.w) * lightPos;
		direction = lightPos - intersectionPoint;
	}
	std::vector<Ray>shadowRays;
	if(shadowGridSize > 1 && light.getSize() > 0){//0 means point & directional light
		//calculate max derivation
		double maxDerivation = (light.getSize()/2)/distanceToLight;
		shadowRays = Ray::generateDeriveredRays(intersectionPoint,direction,refractionIndex,shadowGridSize,maxDerivation);
	} else {
		shadowRays.push_back(Ray(intersectionPoint,direction.normalize(),refractionIndex,0));
	}
	//at this point we know that
	double visibility = 0.0;

	//we start from 1 because 0 is already tested
	for (unsigned char i = 0; i < shadowRays.size(); ++i) {
		if (isLightVisible(shadowRays[i], distanceToLight, octree)) {
			visibility += 1.0;
		}
		/*
		 //FIXME this is causing errors if model is spiked or has sharp corners
		if(i == 4){
			if (visibility == 5.0) { //if all corners and center see light, no one will be blocked
				//std::cout << "all visible " << std::endl;
				return 1.0;
			} else if (visibility == 0.0) { // if none of them see light, no one will see either
				//std::cout << "all blocked" << std::endl;
				return 0.0;
			}
		}
		*/
	}

	//std::cout << "vis: " << visibility / (shadowGridSize * shadowGridSize) << std::endl;
	return visibility / (shadowRays.size());//since we calculate gridsize^2 rays, and result must be between 0-1
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
	double distance = std::numeric_limits<double>::max(); // this is the maximum value double can have, min() returns min positive value.

	Primitive* intersectingPrimitive = NULL;
	Primitive* closestIntersectingPrimitive = NULL;

	std::set<Primitive*> primitives;
	octree.getIntersectingPrimitives(ray, primitives);
	if (!primitives.empty()) {
		double intersectionDistance;
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
		return Vec3f(0.0, 0.0, 0.0);
	}

}

