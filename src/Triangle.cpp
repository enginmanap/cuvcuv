/*
 * Triange.cpp
 *
 *  Created on: 2 Kas 2014
 *      Author: engin
 */

#include "Triangle.h"
#include "RayTracer.h"

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

Vec3f Triangle::getColorForRay(const Ray ray, float distance,
		const std::vector<Primitive*>& primitives,
		const std::vector<Light>& lights, const unsigned int depth) const {
	Vec3f color;

	RayTracer tracer;

	Vec3f intersectionPoint = distance * ray.getDirection();
	intersectionPoint = intersectionPoint + ray.getPosition();
	Vec3f normal = vec3fNS::cross((b - a), (c - a));
	normal = Vec4f(normal, 0.0f) * this->inverseTransformMat.transpose();
	normal = vec3fNS::normalize(normal);
	Vec4f normal4(normal, 0.0f); //is must be 0, or it would make w bigger than 1
	Vec3f eyeDirn = vec3fNS::normalize(
			((Vec3f) ray.getPosition()) - intersectionPoint);

	//for(std::vector<Light>::const_iterator it= lights.back(); it != lights.end(); it++ ) {
	for (unsigned int i = 0; i < lights.size(); i++) {
		Light it = lights[i];
		Vec3f lightPos;
		Vec3f direction;
		lightPos.x = it.getPosition().x;
		lightPos.y = it.getPosition().y;
		lightPos.z = it.getPosition().z;
		if (fabs(it.getPosition().w) < EPSILON) {
			direction = vec3fNS::normalize(lightPos);
		} else {
			lightPos = (1 / it.getPosition().w) * lightPos;
			direction = vec3fNS::normalize(lightPos - intersectionPoint);
			//Vec3f eyePos = ray.getPosition();
			//direction = vec3fNS::normalize( lightPos);
		}

		Ray rayToLight(intersectionPoint + EPSILON * normal4, direction, 0,
				100);
		if (tracer.traceToLight(rayToLight, primitives, *(&it))) {
			float lightDistance = (it.getPosition() - rayToLight.getPosition()).length();
			Vec3f halfVec = vec3fNS::normalize(eyeDirn + direction);
			//std::cout << "AttenuationFactor " << it.getAttenuationFactor(distance) << " for("<< distance << ")"<< std::endl;
			color = color
					+ it.getAttenuationFactor(lightDistance)
							* calculateColorPerLight(direction, it.getColor(),
									normal, halfVec, diffuse, specular,
									shininess);
		}

	}
	//now we have the color for this object itself, calculate reflections.
	if (fabs(this->specular.x) < EPSILON && fabs(this->specular.y) < EPSILON
			&& fabs(this->specular.z) < EPSILON) {
		//the object is not reflective, so stop here
	} else {
		if (depth < MAX_DEPTH) {
			Ray reflectionRay(intersectionPoint + EPSILON * normal4,
					ray.getDirection()
							- 2 * Vec4fNS::dot(ray.getDirection(), normal4)
									* normal4, 0, 100);
			Vec3f reflectedColor = tracer.trace(reflectionRay, primitives,
					lights, depth);
			reflectedColor = vec3fNS::clamp(reflectedColor, 0, 1);
			//std::cout << "reflection " << reflectedColor << std::endl;
			color = color + specular * reflectedColor;

		}
	}
	color = color + ambientLight + emissionLight;
	//Opengl auto clamps, we should do it manually;
	return vec3fNS::clamp(color, 0, 1); //TODO move clamping to last step, just before writing the pixel.
}
