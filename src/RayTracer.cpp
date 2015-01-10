/*
 * RayTracer.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "RayTracer.h"

RayTracer::RayTracer()
{
	deviations[0][0] = 0;
	deviations[0][1] = 0;
	if(SHADOW_RATE >=4){
		deviations[1][0] = -0.5f;deviations[1][1] = -0.5f;//check lower left
		deviations[2][0] =  0.5f;deviations[2][1] =  0.5f;//check upper right
		deviations[3][0] = -0.5f;deviations[3][1] =  0.5f;//upper left
		deviations[4][0] =  0.5f;deviations[4][1] = -0.5f;//lower right

		//srand(time(NULL));
		for (int i = 5; i < SHADOW_RATE; ++i) {
			deviations[i][0]= rand()/float(RAND_MAX+1)- 0.5;
			deviations[i][1]= rand()/float(RAND_MAX+1)- 0.5;
		}
	}
}


/**
 * returns true if the ray can reach light with out hitting
 * any objects. false if there are any obstacles.
 *
 */
float RayTracer::traceToLight(const Vec4f& intersectionPoint, const Octree& octree,
		const Light& light) const {
	//calculate the mo
	//we should cast number of rays, and average the visibility results
	float visibility = 0.0f;
	bool isBlocked;

	for(unsigned int i = 0; i < SHADOW_RATE; ++i){
		Vec3f direction;
		Vec4f lightPosition = light.getPosition();
        float distanceToLight = ((Vec3f)(lightPosition - intersectionPoint)).length();
		lightPosition.x += deviations[i][0] * 0.025 * distanceToLight;
		lightPosition.y += deviations[i][1] * 0.025 * distanceToLight;
		if (fabs(lightPosition.w) < EPSILON) {
			direction = lightPosition;
		} else {
			Vec4f lightPos = lightPosition;
			lightPos = (1 / lightPosition.w) * lightPos;
			direction = lightPos - intersectionPoint;
		}

		Ray rayToLight(intersectionPoint,
				direction, 0, 100);



		float intersectionDistance;
		Primitive* placeHolder = NULL; //this primitive will not be used, but it is required
		std::set<Primitive*> primitives;
		octree.getIntersectingPrimitives(rayToLight,primitives);
		isBlocked = false;
		for (std::set<Primitive*>::const_iterator it = primitives.begin();
				it != primitives.end(); ++it) {
			if ((*it)->intersectiontest(rayToLight, intersectionDistance,&placeHolder)) {
				//found intersection, check if it is before the closest one
				if (distanceToLight > intersectionDistance) {
					isBlocked = true; //if we have one blocking object, it is enough
					break;
				}

			}
		}
		if(!isBlocked){
			visibility += 1.0/SHADOW_RATE;
		}
		if(i == 5 ) { //after first 5
			if( visibility == 5.0/SHADOW_RATE) { //if all of them see light, no one will be blocked
				//std::cout << "all visible " << std::endl;
				return 1.0f;
			} else if ( visibility == 0) { // if none of them see light, no one will see either
				//std::cout << "all blocked" << std::endl;
				return 0.0f;
			}
		}
	}
	return visibility;
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
				lights, depth - 1, this);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

