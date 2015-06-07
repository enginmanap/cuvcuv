/*
 * Triangle.cpp
 *
 *  Created on: 2 Oca 2015
 *      Author: engin
 */

#include "Triangle.h"

Vec3f Triangle::calculateBarycentricCoord(const Vec4f& intersectionPoint) const {
	/**
	 * since we have the intersection point, we will assume it is
	 * in  the triangle plane, and interpolate based on distance
	 */
	Vec3f intersection = intersectionPoint * this->inverseTransformMat;

	Vec3f vectorIntersectionA = intersection - a;
	double dotProductIABA = Vec3fNS::dot(vectorIntersectionA, vectorBA);
	double dotProductIACA = Vec3fNS::dot(vectorIntersectionA, vectorCA);
	double denom = dotProductBABA * dotProductCACA
			- dotProductBACA * dotProductBACA;
	double v = (dotProductCACA * dotProductIABA
			- dotProductBACA * dotProductIACA) / denom;
	double w = (dotProductBABA * dotProductIACA
			- dotProductBACA * dotProductIABA) / denom;
	double u = 1.0 - v - w;
	return Vec3f(u,v,w);
}

/**
 * Returns normal for intersection point, and sets barycentric coordinates
 * of intersection to second parameter.
 *
 * @params:
 * intersectionPoint: The points plane is not verified
 * BCCoord: Barycentric coordinates of the intersection
 */
Vec3f Triangle::calculateNormal(const Vec4f& intersectionPoint) const {
	if (!smoothShading) {
		return triangleNormal;
	} else {
		Vec3f bcCorrds = calculateBarycentricCoord(intersectionPoint);
		Vec3f normal = bcCorrds.u * n1 + bcCorrds.v * n2 + bcCorrds.w * n3;

		//if there is a normal map
		if(this->material->getMapBump() != NULL){
				/**
				 * calculating barycentric coordinates
				 * TODO this must be exported as another function
				 */
				Vec3f texelCoord = bcCorrds.w * t1 + bcCorrds.v * t2 + bcCorrds.w * t3;
				Vec3f normalChange = this->material->getMapBump()->getColor(texelCoord.x,
						texelCoord.y);

				//why this values work is a mystery for me.
				normalChange.x = normalChange.x - 0.5;
				normalChange.y = normalChange.y -0.5;
				normalChange.z = normalChange.z- 1;

				// do not normalize the change, it is not normal vector
				//normal = (normal.normalize() + normalChange).normalize();
				//add value from map to the normal
				normal = normal + normalChange;
				normal= normal.normalize();
		}

		return Vec3fNS::normalize(
				Vec4f(normal, 0) * this->inverseMatrixTranspose);
	}

}

Vec3f Triangle::getDiffuse(const Vec4f& intersectionPoint) const {
	if (this->material->getMapKd() == NULL) {
		//there might be no map
		return this->material->getDiffuse();
	}

	//order is u->a, v->b, w->c
	Vec3f bcCorrds = calculateBarycentricCoord(intersectionPoint);
	Vec3f texelCoord = bcCorrds.u * t1 + bcCorrds.v * t2 + bcCorrds.w * t3;

	//std::cout << "u " << u << " v " << v << " w " << w << std::endl;
	//std::cout << "map texel coordinates " << texelCoord.x << ", " <<texelCoord.y << std::endl;
	Vec3f diffuseColor = this->material->getMapKd()->getColor(texelCoord.x,
			texelCoord.y);

	//std::cout << "diffuse result "<< diffuseColor << std::endl;
	return diffuseColor;

}
