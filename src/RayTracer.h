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

#ifndef SHADOW_RATE
#define SHADOW_RATE 50
#endif //SHADOW_RATE

class RayTracer {
private:
	float deviations[SHADOW_RATE][2];
public:
	RayTracer();
	Vec3f trace(const Ray&, const Octree&, const std::vector<Light>&, const unsigned int) const;
	float traceToLight(const Vec4f&, const Octree&, const Light&) const;
};

#endif /* SRC_RAYTRACER_H_ */
