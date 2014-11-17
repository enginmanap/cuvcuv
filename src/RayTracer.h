/*
 * RayTracer.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_RAYTRACER_H_
#define SRC_RAYTRACER_H_

#include <vector>
#include <limits>

#include "Ray.h"
#include "Primitive.h"
#include "Light.h"
#include "Vec3f.h"
#include "Octree.h"


class RayTracer {
public:
	RayTracer();
	virtual ~RayTracer();

	Vec3f trace(const Ray,  const Octree&,const std::vector<Light>&, const unsigned int) const;
	bool traceToLight(const Ray ray, const Octree&, const Light light) const;
};

#endif /* SRC_RAYTRACER_H_ */
