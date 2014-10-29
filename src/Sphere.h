/*
 * Sphere.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_SPHERE_H_
#define SRC_SPHERE_H_

#include "Vec3f.h"

class Sphere {
	Vec3f position;
	float radius;
public:
	Sphere(float, float, float, float);
	virtual ~Sphere();
};

#endif /* SRC_SPHERE_H_ */
