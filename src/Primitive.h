/*
 * Primitive.h
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "Vec3f.h"
#include "Ray.h"

#define EPSILON 0.0001F

class Primitive {
protected:
	Vec3f ambientLight, specular, diffuse;
	float shininess;
	Vec3f calculateColorPerLight(const Vec3f, const Vec3f, const Vec3f,
			const Vec3f, const Vec3f, const Vec3f, const float);
public:
	virtual bool intersectiontest(Ray, float&);
	virtual Vec3f getColorForRay(Ray, float);
	bool setLightValues(Vec3f, Vec3f, Vec3f, float);
};

#endif /* PRIMITIVE_H_ */
