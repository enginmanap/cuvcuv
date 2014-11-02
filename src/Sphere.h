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

#define EPSILON 0.0001F

class Sphere {
	Vec3f position;
	Vec3f ambientLight;
	float radius;
	Vec3f calculateColorPerLight(const Vec3f, const Vec3f, const Vec3f,
			const Vec3f, const Vec3f, const Vec3f, const float);
public:
	Sphere(float, float, float, float, Vec3f);
	virtual ~Sphere();
	bool intersectiontest(Ray, float&);
	Vec3f getColorForRay(Ray, float);
	bool setAmbientLight(Vec3f);

};
#endif /* SRC_SPHERE_H_ */
