/*
 * Triangle.h
 *
 *  Created on: 2 Oca 2015
 *      Author: engin
 */

#ifndef SRC_TRIANGLE_H_
#define SRC_TRIANGLE_H_

#include "TriangleBase.h"
#include <math.h>

class Triangle: public TriangleBase {
private:
	Vec3f n1,n2,n3;
	bool smoothShading;
	Mat4f inverseMatrixTranspose;

	Vec3f vectorBA, vectorCA;
	float dotProductBABA, dotProductBACA, dotProductCACA;

public:
	Triangle(const Vec3f& vertice1, const Vec3f& vertice2, const Vec3f& vertice3,
			const Vec3f& normal1, const Vec3f& normal2, const Vec3f& normal3,
			const Mat4f& transformMat, bool smoothing):TriangleBase(vertice1,vertice2,vertice3,transformMat),
			n1(normal1), n2(normal2),n3(normal3), smoothShading(smoothing){
		this->inverseMatrixTranspose = this->inverseTransformMat.transpose();
		vectorBA = b - a;
		vectorCA = c - a;
		dotProductBABA = vec3fNS::dot(vectorBA, vectorBA);
		dotProductBACA = vec3fNS::dot(vectorBA, vectorCA);
		dotProductCACA = vec3fNS::dot(vectorCA, vectorCA);
		//TriangleBase processes transform
		if(!smoothing){
			triangleNormal = vec3fNS::normalize(n1 + n2 + n3);
		}
	};

	Vec3f calculateNormal(const Vec4f& intersectionPoint) const;
};

#endif /* SRC_TRIANGLE_H_ */
