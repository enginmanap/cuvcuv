/*
 * TriangeBase.h
 *
 *  Created on: 2 Kas 2014
 *      Author: Engin Manap
 */

#ifndef SRC_TRIANGLE_H_
#define SRC_TRIANGLE_H_

#include <vector>
#include "Light.h"

#include "Vec3f.h"
#include "Ray.h"
#include "Primitive.h"

class TriangleBase: public Primitive {

private:
	Vec3f a, b, c;
	Vec3f triangleNormal;
	Vec3f calculateNormal(const Vec4f&) const;
	void generateBoundingBox();
public:
	bool setTransformation(const Mat4f&);
	TriangleBase(const Vec3f& vertice1, const Vec3f& vertice2, const Vec3f& vertice3, const Mat4f& transformMat):a(vertice1), b(vertice2), c(vertice3){
		this->setTransformation(transformMat); //this generates normal and bounding box.
	};
	virtual ~TriangleBase();

	bool intersectiontest(Ray, float&, Primitive**) const;
};

#endif /* SRC_TRIANGLE_H_ */
