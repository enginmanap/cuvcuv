/*
 * Triange.h
 *
 *  Created on: 2 Kas 2014
 *      Author: engin
 */

#ifndef SRC_TRIANGLE_H_
#define SRC_TRIANGLE_H_

#include "Vec3f.h"
#include "Ray.h"

//FIXME this is also found on sphere, it should not be repeated
#define EPSILON 0.0001F

class Triangle {

private:
	Vec3f a, b, c;
	Vec3f ambientLight, specular, diffuse;
	float shininess;
	Vec3f calculateColorPerLight(const Vec3f, const Vec3f, const Vec3f,
			const Vec3f, const Vec3f, const Vec3f, const float);
public:
	Triangle(Vec3f, Vec3f, Vec3f);
	virtual ~Triangle();

	bool intersectiontest(Ray, float&);
	Vec3f getColorForRay(Ray, float);
	bool setLightValues(Vec3f, Vec3f, Vec3f, float);

};

#endif /* SRC_TRIANGLE_H_ */
