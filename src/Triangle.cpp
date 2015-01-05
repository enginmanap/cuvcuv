/*
 * Triangle.cpp
 *
 *  Created on: 2 Oca 2015
 *      Author: engin
 */

#include "Triangle.h"

/**
 * Returns normal for intersection point, and sets barycentric coordinates
 * of intersection to second parameter.
 *
 * @params:
 * intersectionPoint: The points plane is not verified
 * BCCoord: Barycentric coordinates of the intersection
 */
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

Vec3f Triangle::getDiffuse(const Vec3f& intersectionPoint) const{
	if(this->material->getMapKd() == NULL){
		//there might be no map
		return this->material->getDiffuse();
	}
	/**
	 * calculating barycentric coordinates
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
	Vec3f texelCoord = u*t1 + v*t2 + w*t3;
	//std::cout << "u " << u << " v " << v << " w " << w << std::endl;
	//std::cout << "map texel coordinates " << texelCoord.x << ", " <<texelCoord.y << std::endl;
	Vec3f diffuseColor = this->material->getMapKd()->getColor(texelCoord.x,texelCoord.y);

	//std::cout << "diffuse result "<< diffuseColor << std::endl;
	return diffuseColor;

}
