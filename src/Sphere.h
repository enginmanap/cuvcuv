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
	float radius;

	Vec3f ambientLight, specular, diffuse;
	float shininess;

	Vec3f calculateColorPerLight(const Vec3f, const Vec3f, const Vec3f,
			const Vec3f, const Vec3f, const Vec3f, const float);
public:
	Sphere(float, float, float, float);
	virtual ~Sphere();
	bool intersectiontest(Ray, float&);
	Vec3f getColorForRay(Ray, float);
	bool setLightValues(Vec3f, Vec3f, Vec3f, float);

};
#endif /* SRC_SPHERE_H_ */
