/*
 * Primitive.cpp
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#include "Primitive.h"

Vec3f Primitive::calculateColorPerLight(const Vec3f direction,
		const Vec3f color, const Vec3f normal, const Vec3f halfVec,
		const Vec3f diffuse, const Vec3f specular,
		const float shininess) const {

	Vec3f lambert, phong;

	float nDotl = vec3fNS::dot(normal, direction);
	float nDotH = vec3fNS::dot(normal, halfVec);
	/*
	 if (nDotl > 0.0f) {
	 lambert = diffuse * color;
	 //lambert = Vec3f(diffuse.x * color.x,diffuse.y * color.y,diffuse.z * color.z);
	 lambert = nDotl * lambert;

	 }


	 if (nDotH > 0.0f) {
	 phong = specular * color;
	 //phong = Vec3f(specular.x * color.x,specular.y * color.y,specular.z * color.z);
	 float temp = (float) pow(nDotH, shininess);
	 phong = temp * phong;
	 }
	 */
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

Primitive::~Primitive() {

}
