/*
 * Triange.h
 *
 *  Created on: 2 Kas 2014
 *      Author: engin
 */

#ifndef SRC_TRIANGLE_H_
#define SRC_TRIANGLE_H_

#include <vector>
#include "Light.h"

#include "Vec3f.h"
#include "Ray.h"
#include "Primitive.h"

class Triangle: public Primitive {

private:
	Vec3f a, b, c;
	Vec3f calculateNormal(const Vec4f&) const;
public:
	Triangle(Vec3f, Vec3f, Vec3f);
	virtual ~Triangle();

	bool intersectiontest(Ray, float&) const;
	Vec3f getColorForRay(const Ray,  float, const std::vector<Primitive*>&, const std::vector<Light>&, const unsigned int) const;
	unsigned char isInBoundingBox(const Vec3f&, const Vec3f&) const;
};

#endif /* SRC_TRIANGLE_H_ */
