/*
 * RayTracer.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_RAYTRACER_H_
#define SRC_RAYTRACER_H_

#include <vector>
#include "Ray.h"
#include "Sphere.h"
#include "Vec3f.h"
#include <limits>

class RayTracer {
public:
	RayTracer();
	virtual ~RayTracer();

	Vec3f trace(Ray, std::vector<Sphere>&);
};

#endif /* SRC_RAYTRACER_H_ */
