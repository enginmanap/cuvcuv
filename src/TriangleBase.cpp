/*
 * TriangeBase.cpp
 *
 *  Created on: 2 Kas 2014
 *      Author: Engin Manap
 */

#include "TriangleBase.h"

bool TriangleBase::setTransformation(const Mat4f& transformMatrix) {
	Primitive::setTransformation(transformMatrix); //call to super, so inverse will be calculated
	Vec3f normal = Vec3fNS::cross((b - a), (c - a));
	normal = Vec4f(normal, 0) * this->inverseTransformMat.transpose();
	triangleNormal = Vec3fNS::normalize(normal);
	return true;
}

void TriangleBase::generateBoundingBox() {
	//std::cout<<"generate box" << std::endl;
	//first calculate values of the vertex points, after transformations
	Vec3f transformedA = Vec4f(a, 1.0) * this->transformMatrix; //1.0 since this is not a direction, but position
	Vec3f transformedB = Vec4f(b, 1.0) * this->transformMatrix;
	Vec3f transformedC = Vec4f(c, 1.0) * this->transformMatrix;

	//now calculate the bounding box
	double xMax = std::max(transformedA.x,
			std::max(transformedB.x, transformedC.x));
	double yMax = std::max(transformedA.y,
			std::max(transformedB.y, transformedC.y));
	double zMax = std::max(transformedA.z,
			std::max(transformedB.z, transformedC.z));
	this->bbUpper = Vec3f(xMax, yMax, zMax);

	double xMin = std::min(transformedA.x,
			std::min(transformedB.x, transformedC.x));
	double yMin = std::min(transformedA.y,
			std::min(transformedB.y, transformedC.y));
	double zMin = std::min(transformedA.z,
			std::min(transformedB.z, transformedC.z));
	this->bbLower = Vec3f(xMin, yMin, zMin);

	double xCenter = (xMax + xMin) / 2;
	double yCenter = (yMax + yMin) / 2;
	double zCenter = (zMax + zMin) / 2;
	this->bbCenter = Vec3f(xCenter, yCenter, zCenter);

}

bool TriangleBase::intersectiontest(Ray ray, double& distance,
		Primitive** intersectingPrimitive) const {
	Ray transformedRay = generateTransformedRay(ray);
	Vec3f rayPosition = transformedRay.getPosition();
	Vec3f rayDirection = transformedRay.getDirection();
	Vec3f edge1 = b - a;
	Vec3f edge2 = c - a;
	Vec3f pvec = Vec3fNS::cross(rayDirection, edge2);
	double det = Vec3fNS::dot(edge1, pvec);
	if (fabs(det) < EPSILON)
		return false;
	double invDet = 1 / det;
	Vec3f tvec = rayPosition - a;
	double u = Vec3fNS::dot(tvec, pvec) * invDet;
	if (u < -EPSILON || u > 1)
		return false;
	Vec3f qvec = Vec3fNS::cross(tvec, edge1);
	double v = Vec3fNS::dot(rayDirection, qvec) * invDet;
	if (v < -EPSILON || u + v > 1)
		return false;

	distance = Vec3fNS::dot(edge2, qvec) * invDet;
	if (distance < 0) { //object is behind the triangle
		return false;
	}
	return true;
}

Vec3f TriangleBase::calculateNormal(const Vec4f& position) const {
	return triangleNormal;
}
