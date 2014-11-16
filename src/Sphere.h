/*
 * Sphere.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_SPHERE_H_
#define SRC_SPHERE_H_

#include <vector>

#include "Light.h"
#include "Vec3f.h"
#include "Ray.h"
#include "Primitive.h"

class Sphere: public Primitive {
	Vec3f position;
	float radius;
	Vec3f calculateNormal(const Vec4f&) const;
public:
	Sphere(float, float, float, float);
	virtual ~Sphere();
	bool intersectiontest(Ray, float&) const;
	Vec3f getColorForRay(const Ray,  float, const std::vector<Primitive*>&, const std::vector<Light>&, const unsigned int) const;
	unsigned char isInBoundingBox(const Vec3f&, const Vec3f&) const;
};
#endif /* SRC_SPHERE_H_ */
