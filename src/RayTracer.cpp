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

Vec3f RayTracer::trace(Ray ray, std::vector<Sphere> &spheres, std::vector<Triangle> &triangles) {
	float distance = std::numeric_limits<float>::max(); // this is the maximum value float can have, min() returns min positive value.
	float intersectionDistance;
	Sphere* intersectingSphere = NULL;
	Triangle* intersectingTriangle = NULL;

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
	for (std::vector<Triangle>::iterator it = triangles.begin();
			it != triangles.end(); it++) {
		if (it->intersectiontest(ray, intersectionDistance)) {
			//found intersection
			//std::cout << "triangle intersection found" << std::endl;
			if (distance > intersectionDistance){
				distance = intersectionDistance;
				intersectingTriangle = &(*it);
			}

		}
	}

	if(intersectingTriangle != NULL){
		Vec3f color = intersectingTriangle->getColorForRay(ray, distance);
		//std::cout << "triangle color " << color<< std::endl;
		return color;
	} else if (intersectingSphere != NULL){
		return intersectingSphere->getColorForRay(ray, distance);
	} else {
		return Vec3f(0.0f, 0.0f, 0.0f);
	}

}

