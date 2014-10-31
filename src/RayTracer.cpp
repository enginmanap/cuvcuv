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

Vec3f RayTracer::trace(Ray ray, std::vector<Sphere> &spheres){
	std::vector<float> intersections;
	for(std::vector<Sphere>::iterator it= spheres.begin(); it != spheres.end(); it++){
		if(it->intersectiontest(ray)){
			//found intersection
			return Vec3f(255.0f,0.1f,0.1f);
		}
	}

	return Vec3f(255.0f,255.0f,255.0f);
}

