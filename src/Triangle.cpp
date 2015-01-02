/*
 * Triangle.cpp
 *
 *  Created on: 2 Oca 2015
 *      Author: engin
 */

#include "Triangle.h"

Vec3f Triangle::calculateNormal(const Vec4f& intersectionPoint) const{
	if(!smoothShading){
		return triangleNormal;
	} else {
		/**
		 * since we have the intersection point, we will assume it is
		 * in  the triangle plane, and interpolate based on distance
		 */
		Vec3f intersection = intersectionPoint* this->inverseTransformMat;

		Vec3f vectorIntersectionA = intersection - a;
		float dotProductIABA = vec3fNS::dot(vectorIntersectionA, vectorBA);
		float dotProductIACA = vec3fNS::dot(vectorIntersectionA, vectorCA);
		float denom = dotProductBABA * dotProductCACA - dotProductBACA * dotProductBACA;
		float v = (dotProductCACA * dotProductIABA - dotProductBACA * dotProductIACA) / denom;
		float w = (dotProductBABA * dotProductIACA - dotProductBACA * dotProductIABA) / denom;
		float u = 1.0f - v - w;
		//order is u->a, v->b, w->c
		Vec3f normal = u*n1 + v*n2 + w*n3;

		return vec3fNS::normalize(Vec4f(normal, 0.0f) * this->inverseMatrixTranspose);
	}

}

