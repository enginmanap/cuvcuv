/*
 * Triange.cpp
 *
 *  Created on: 2 Kas 2014
 *      Author: engin
 */

#include "Triangle.h"

Triangle::Triangle(Vec3f& vertice1, Vec3f& vertice2, Vec3f& vertice3, Mat4f& transformMat) {
	a = vertice1;
	b = vertice2;
	c = vertice3;
	this->setTransformation(transformMat); //this generates normal and bounding box.

}

Triangle::~Triangle() {

}

bool Triangle::setTransformation(Mat4f& transformMatrix){
	Primitive::setTransformation(transformMatrix);//call to super, so inverse will be calculated
	Vec3f normal = vec3fNS::cross((b - a), (c - a));
	normal = Vec4f(normal, 0.0f) * this->inverseTransformMat.transpose();
	triangleNormal = vec3fNS::normalize(normal);
	return true;
}


void Triangle::generateBoundingBox(){
	//std::cout<<"generate box" << std::endl;
	//first calculate values of the vertex points, after transformations
	Vec3f transformedA = Vec4f(a,1.0f) * this->transformMatrix; //1.0f since this is not a direction, but position
	Vec3f transformedB = Vec4f(b,1.0f) * this->transformMatrix;
	Vec3f transformedC = Vec4f(c,1.0f) * this->transformMatrix;

	//now calculate the bounding box
	float xMax = std::max(transformedA.x, std::max(transformedB.x, transformedC.x));
	float yMax = std::max(transformedA.y, std::max(transformedB.y, transformedC.y));
	float zMax = std::max(transformedA.z, std::max(transformedB.z, transformedC.z));
	this->bbUpper = Vec3f(xMax,yMax,zMax);

	float xMin = std::min(transformedA.x, std::min(transformedB.x, transformedC.x));
	float yMin = std::min(transformedA.y, std::min(transformedB.y, transformedC.y));
	float zMin = std::min(transformedA.z, std::min(transformedB.z, transformedC.z));
	this->bbLower=Vec3f(xMin,yMin,zMin);

	float xCenter = (xMax + xMin) /2;
	float yCenter = (yMax + yMin) /2;
	float zCenter = (zMax + zMin) /2;
	this->bbCenter=Vec3f(xCenter,yCenter,zCenter);


}

bool Triangle::intersectiontest(Ray ray, float& distance) const {
	Ray transformedRay = generateTransformedRay(ray);
	Vec3f rayPosition = transformedRay.getPosition();
	Vec3f rayDirection = transformedRay.getDirection();
	Vec3f edge1 = b - a;
	Vec3f edge2 = c - a;
	Vec3f pvec = vec3fNS::cross(rayDirection, edge2);
	float det = vec3fNS::dot(edge1, pvec);
	if (fabs(det) < EPSILON)
		return false;
	float invDet = 1 / det;
	Vec3f tvec = rayPosition - a;
	float u = vec3fNS::dot(tvec, pvec) * invDet;
	if (u < -EPSILON || u > 1)
		return false;
	Vec3f qvec = vec3fNS::cross(tvec, edge1);
	float v = vec3fNS::dot(rayDirection, qvec) * invDet;
	if (v < -EPSILON || u + v > 1)
		return false;

	distance = vec3fNS::dot(edge2, qvec) * invDet;
	if (distance < 0.0f) { //object is behind the triangle
		return false;
	}
	return true;
}

Vec3f Triangle::calculateNormal(const Vec4f& position) const {
	return triangleNormal;
}
