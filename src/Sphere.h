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
	void generateBoundingBox();
	Mat4f inverseMatrixTranspose;
public:
	bool setTransformation(Mat4f&);
	Sphere(float, float, float, float, Mat4f&);
	virtual ~Sphere();
	bool intersectiontest(Ray, float&) const;
};
#endif /* SRC_SPHERE_H_ */
