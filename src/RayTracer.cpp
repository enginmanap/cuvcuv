/*
 * RayTracer.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "RayTracer.h"

RayTracer::RayTracer() {
	// TODO Auto-generated constructor stub

}

RayTracer::~RayTracer() {
	// TODO Auto-generated destructor stub
}

Vec3f RayTracer::trace(Ray ray, std::vector<Sphere> &spheres) {
	float distance = std::numeric_limits<float>::max(); // this is the maximum value float can have, min() returns min positive value.
	float intersectionDistance;
	Sphere* intersectingSphere = NULL;

	for (std::vector<Sphere>::iterator it = spheres.begin();
			it != spheres.end(); it++) {
		if (it->intersectiontest(ray, intersectionDistance)) {
			//found intersection
			if (distance > intersectionDistance){
				distance = intersectionDistance;
				intersectingSphere = &(*it);
			}

		}
	}

	if(intersectingSphere == NULL){
		//if no intersection is found
		return Vec3f(255.0f, 255.0f, 255.0f);
	} else {
		return intersectingSphere->getColorForRay(ray, distance);
	}

}

