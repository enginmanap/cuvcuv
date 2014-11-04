/*
 * Triange.cpp
 *
 *  Created on: 2 Kas 2014
 *      Author: engin
 */

#include "Triangle.h"

Triangle::Triangle(Vec3f vertice1, Vec3f vertice2, Vec3f vertice3) {
	a = vertice1;
	b = vertice2;
	c = vertice3;

}

Triangle::~Triangle() {

}

/*
 bool Triangle::intersectiontest(Ray ray, float& distance) {
 //calculate the normal
 Vec3f normal = Vec3f::cross((c - a), (b - a));
 normal = Vec3f::normalize(normal);
 // plane calculation. in any point P in the plane, (P - anyVertice) dot Normal == 0
 // if ray is passing trought the P point, Ray equation is P = ray.position + t * ray.direction
 // t = ( anyVertice dot normal - ray.position dot normal ) / ray.direction dot normal.
 // first of all, if the ray.direction dot normal is 0, than intersection is not possible, so return false
 float directionDotNormal = Vec3f::dot(ray.getDirection(), normal);
 if (fabs(directionDotNormal) < EPSILON) {
 return false;
 }
 //if it is not zero, than calculate the t
 distance = (Vec3f::dot(a, normal) - Vec3f::dot(ray.getPosition(), normal))
 / directionDotNormal;
 //if distance is negative, it means the intersection is behind the ray, so it does not actual
 if (distance < 0.0f) {
 return false;
 }

 //now we should check if ray hits triangle after going the distance
 //for a P in the triangle of A,B,C, this can be defined:
 //P=alpha*A + beta*B + gamma*C
 //alpha>=0, beta>=0, gamma>=0 and alpha + beta + gamma = 1
 // P - A = beta(B - A) + gamma(C - A) && beta + gamma <=1
 Vec3f p = ray.getPosition() + distance * ray.getDirection();
 //Vec3f pMinusA = p - a;
 Vec3f bMinusA = b - a;
 Vec3f cMinusA = c - a;

 return false;
 }
 */

bool Triangle::intersectiontest(Ray ray, float& distance) const {
	Vec3f edge1 = b - a;
	Vec3f edge2 = c - a;
	Vec3f pvec = vec3fNS::cross(ray.getDirection(), edge2);
	float det = vec3fNS::dot(edge1, pvec);
	if (det == 0)
		return false;
	float invDet = 1 / det;
	Vec3f tvec = ray.getPosition() - a;
	float u = vec3fNS::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1)
		return false;
	Vec3f qvec = vec3fNS::cross(tvec, edge1);
	float v = vec3fNS::dot(ray.getDirection(), qvec) * invDet;
	if (v < 0 || u + v > 1)
		return false;

	distance = vec3fNS::dot(edge2, qvec) * invDet;
	return true;
}

Vec3f Triangle::getColorForRay(Ray ray, float distance) const {
	return this->ambientLight;
}
