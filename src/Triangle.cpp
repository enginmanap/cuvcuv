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
	Vec3f v0v1 = b - a;
	Vec3f v0v2 = c - a;
	Vec3f N = Vec3f::cross(v0v1, v0v2);
	float nDotRay = Vec3f::dot(N, ray.getDirection());
	/*
	 if (nDotRay == 0 || (nDotRay > 0 ) && false){
	 //std::cout << "0" << std::endl;
	 return false; // ray parallel to triangle
	 } */

	float d = Vec3f::dot(N, a);
	float t = -(Vec3f::dot(N, ray.getPosition()) + d) / nDotRay;
	if (t < 0) {
		//std::cout << "1" << std::endl;
		return false; // ray behind triangle
	}
	// inside-out test
	Vec3f Phit = ray.getPosition() + t * ray.getDirection();
	// inside-out test edge0
	Vec3f v0p = Phit - a;
	float v = Vec3f::dot(N, Vec3f::cross(v0v1, v0p));
	if (v < 0) {
		//std::cout << "2" << std::endl;
		return false; // P outside triangle
	}
	// inside-out test edge1
	Vec3f v1p = Phit - b;
	Vec3f v1v2 = c - b;
	float w = Vec3f::dot(N, Vec3f::cross(v1v2, v1p));
	if (w < 0) {
		//std::cout << "3" << std::endl;
		return false; // P outside triangle
	}
	// inside-out test edge2
	Vec3f v2p = Phit - c;
	Vec3f v2v0 = a - c;
	float u = Vec3f::dot(N, Vec3f::cross(v2v0, v2p));
	if (u < 0) {
		//std::cout << "4" << std::endl;
		return false; // P outside triangle
	}
	/*
	 float nlen2 = Vec3f::dot(N, N);
	 isectData.t = t;
	 isectData.u = u / nlen2;
	 isectData.v = v / nlen2;
	 */
	distance = t;
	return true;
}

Vec3f Triangle::getColorForRay(Ray ray, float distance) const {
	return this->ambientLight;
}
