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
	Ray transformedRay = generateTransformedRay(ray);
	Vec3f rayPosition = transformedRay.getPosition();
	Vec3f rayDirection = transformedRay.getDirection();
	Vec3f edge1 = b - a;
	Vec3f edge2 = c - a;
	Vec3f pvec = vec3fNS::cross(rayDirection, edge2);
	float det = vec3fNS::dot(edge1, pvec);
	if (det == 0)
		return false;
	float invDet = 1 / det;
	Vec3f tvec = rayPosition - a;
	float u = vec3fNS::dot(tvec, pvec) * invDet;
	if (u < 0 || u > 1)
		return false;
	Vec3f qvec = vec3fNS::cross(tvec, edge1);
	float v = vec3fNS::dot(rayDirection, qvec) * invDet;
	if (v < 0 || u + v > 1)
		return false;

	distance = vec3fNS::dot(edge2, qvec) * invDet;
	if (distance < 0.0f) { //object is behind the triangle
		return false;
	}
	return true;
}

Vec3f Triangle::calculateNormal(const Vec4f& position) const {
	Vec3f normal = vec3fNS::cross((b - a), (c - a));
	normal = Vec4f(normal, 0.0f) * this->inverseTransformMat.transpose();
	return vec3fNS::normalize(normal);
}

unsigned char Triangle::isInBoundingBox(const Vec3f& upperEnd, const Vec3f& lowerEnd) const {
	//0 not in, 1 partially in 2 contained in.
	unsigned char isIn = 0;
	//check each vertex
	if(this->a.x < upperEnd.x && this->a.y < upperEnd.y && this->a.z < upperEnd.z ){
		if(this->a.x > lowerEnd.x && this->a.y > lowerEnd.y && this->a.z > lowerEnd.z ){
			//a is in it, so atleast a partial hit.
			isIn += 1;
		}
	}
	if(this->b.x < upperEnd.x && this->b.y < upperEnd.y && this->b.z < upperEnd.z ){
		if(this->b.x > lowerEnd.x && this->b.y > lowerEnd.y && this->b.z > lowerEnd.z ){
			//b is in it, so atleast a partial hit.
			isIn += 1;
		}
	}
	if(this->c.x < upperEnd.x && this->c.y < upperEnd.y && this->c.z < upperEnd.z ){
		if(this->c.x > lowerEnd.x && this->c.y > lowerEnd.y && this->c.z > lowerEnd.z ){
			//c is in it, so atleast a partial hit.
			isIn += 1;
		}
	}
	if(isIn == 3){
		return 2;
	} else if (isIn == 0) {
		return 0;
	}
	return 1;

}
