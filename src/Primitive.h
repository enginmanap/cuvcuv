/*
 * Primitive.h
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <vector>

#include "Light.h"
#include "Vec3f.h"
#include "Ray.h"
#include "Mat4f.h"

#ifndef EPSILON
#define EPSILON 0.00001F
#endif

#ifndef MAX_DEPTH
#define MAX_DEPTH 5
#endif

class Primitive {

protected:
	Vec3f ambientLight, emissionLight, specular, diffuse;
	float shininess;
	Mat4f inverseTransformMat, transformMatrix;
	Ray transformedRay;
	Vec3f calculateColorPerLight(const Vec3f, const Vec3f, const Vec3f,
			const Vec3f, const Vec3f, const Vec3f, const float) const;
public:
	virtual ~Primitive();
	bool setTransformation(Mat4f&);
	virtual bool intersectiontest(Ray, float&) const= 0;
	virtual Vec3f getColorForRay(const Ray, float,
			const std::vector<Primitive*>&, const std::vector<Light>&,
			const unsigned int) const = 0;
	bool setLightValues(Vec3f, Vec3f, Vec3f, Vec3f, float);

	Ray generateTransformedRay(const Ray) const;

};

#endif /* PRIMITIVE_H_ */
