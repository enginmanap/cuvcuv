/*
 * RayTracer.h
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#ifndef SRC_RAYTRACER_H_
#define SRC_RAYTRACER_H_

#include <vector>
#include <limits>
#include <cstdlib>
#include <time.h>       /* time */

#include "Ray.h"
#include "Primitive.h"
#include "Light.h"
#include "Vec3f.h"
#include "Octree.h"

class Octree;

class RayTracer {
private:
	unsigned char shadowGridSize;
	double corners[4][2];
	bool isLightVisible(const Ray&, const double, const Octree&) const;

public:
	RayTracer(char);
	Vec3f trace(const Ray&, const Octree&, const std::vector<Light>&,
			const unsigned int) const;
	double traceToLight(const Vec4f&, const Light&, double refractionIndex, const Octree&) const;
};

#endif /* SRC_RAYTRACER_H_ */
