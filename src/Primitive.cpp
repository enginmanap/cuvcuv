/*
 * Primitive.cpp
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#include "Primitive.h"
#include "RayTracer.h"

Vec3f Primitive::calculateColorPerLight(const Vec3f direction,
		const Vec3f color, const Vec3f normal, const Vec3f halfVec,
		const Vec3f diffuse, const Vec3f specular,
		const float shininess) const {

	Vec3f lambert, phong;

	float nDotl = vec3fNS::dot(normal, direction);
	float nDotH = vec3fNS::dot(normal, halfVec);

	lambert = diffuse * color;
	lambert = std::max(nDotl, 0.0f) * lambert;

	phong = specular * color;
	phong = pow(std::max(nDotH, 0.0f), shininess) * phong;

	return lambert + phong;

}

Ray Primitive::generateTransformedRay(const Ray ray) const {
	//since direction has 0 as last element, translate became 0 too
	return Ray(ray.getPosition() * inverseTransformMat,
			ray.getDirection() * inverseTransformMat, 0, 100);
}

bool Primitive::setTransformation(Mat4f& matrix) {
	this->transformMatrix = matrix;
	this->inverseTransformMat = Mat4f::inverse(matrix);
	return true;
}

bool Primitive::setLightValues(Vec3f ambientLight, Vec3f emissionLight,
		Vec3f diffuse, Vec3f specular, float shininess) {
	this->ambientLight = ambientLight;
	this->emissionLight = emissionLight;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	return true;
}

Vec3f Primitive::getColorForRay(const Ray ray, float distance,
		const std::vector<Primitive*>& primitives,
		const std::vector<Light>& lights, const unsigned int depth) const {
	Vec3f color;

	RayTracer tracer;

	Vec4f intersectionPoint = distance * ray.getDirection();
	intersectionPoint = intersectionPoint + ray.getPosition();
	Vec3f normal = this->calculateNormal(intersectionPoint);
	Vec4f normal4(normal, 0.0f);
	Vec3f eyeDirn = vec3fNS::normalize(
			((Vec3f) ray.getPosition()) - intersectionPoint);

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
		}
		//check if light is blocked or not
		Vec4f direction4 = Vec4f(direction, 0.0f);
		Ray rayToLight(intersectionPoint + EPSILON * 10.0f * direction4,
				direction, 0, 100);
		//if(rayToLight.getPosition().getElement(0) * rayToLight.getPosition().getElement(0) +rayToLight.getPosition().getElement(1) * rayToLight.getPosition().getElement(1) + rayToLight.getPosition().getElement(2) * rayToLight.getPosition().getElement(2) < 1)
		//std::cout << "ray origin (" << rayToLight.getPosition() << ") direction (" << rayToLight.getDirection()<< ")" <<std::endl;

		if (tracer.traceToLight(rayToLight, primitives, *(&it))) {
			float lightDistance =
					(it.getPosition() - rayToLight.getPosition()).length();
			Vec3f halfVec = vec3fNS::normalize(direction + eyeDirn);

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
		if (depth > 0) {
			Ray reflectionRay(intersectionPoint + EPSILON * 10.0f * normal4,
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

Primitive::~Primitive() {

}
