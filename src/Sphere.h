/*
 * Sphere.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_SPHERE_H_
#define SRC_SPHERE_H_

#include "Vec3f.h"
#include "Ray.h"
#include "Primitive.h"

#define EPSILON 0.0001F

class Sphere: public Primitive {
	Vec3f position;
	float radius;

public:
	Sphere(float, float, float, float);
	virtual ~Sphere();
	bool intersectiontest(Ray, float&) const;
	Vec3f getColorForRay(Ray, float) const;

};
#endif /* SRC_SPHERE_H_ */
