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


class RayTracer {
public:
	RayTracer();
	virtual ~RayTracer();

	Vec3f trace(const Ray, const std::vector<Primitive*>&,const std::vector<Light>&, const unsigned int) const;
	bool traceToLight(const Ray ray, const std::vector<Primitive*> &Primitives, const Light light) const;
};

#endif /* SRC_RAYTRACER_H_ */
