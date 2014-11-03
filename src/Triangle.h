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
#include "Primitive.h"

class Triangle : public Primitive {

private:
	Vec3f a, b, c;

public:
	Triangle(Vec3f, Vec3f, Vec3f);
	virtual ~Triangle();

	bool intersectiontest(Ray, float&);
	Vec3f getColorForRay(Ray, float);

};

#endif /* SRC_TRIANGLE_H_ */
