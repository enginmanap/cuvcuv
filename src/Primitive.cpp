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

	if (nDotl > 0.0f) {
		lambert = diffuse * color;
		//lambert = Vec3f(diffuse.x * color.x,diffuse.y * color.y,diffuse.z * color.z);
		lambert = nDotl * lambert;
	}

	float nDotH = vec3fNS::dot(normal, halfVec);
	if (nDotH > 0.0f) {
		phong = specular * color;
		//phong = Vec3f(specular.x * color.x,specular.y * color.y,specular.z * color.z);
		float temp = (float) pow(nDotH, shininess);
		phong = temp * phong;
	}
	return lambert + phong;

}

Ray Primitive::generateTransformedRay(const Ray ray) const {
	Mat4f scaleRotate = transformationMatrix, translation;

	translation[0][3] = scaleRotate[0][3];
	translation[1][3] = scaleRotate[1][3];
	translation[2][3] = scaleRotate[2][3];

	scaleRotate[0][3] = 0;
	scaleRotate[1][3] = 0;
	scaleRotate[2][3] = 0;

	Ray temp = Ray(scaleRotate * ray.getPosition() ,scaleRotate * ray.getDirection(), 0,100);

	return Ray(translation * temp.getPosition() , temp.getDirection(), 0, 100);
}

bool Primitive::setTransformation(Mat4f& matrix) {
	this->transformationMatrix = Mat4f::inverse(matrix);
	return true;
}

bool Primitive::setLightValues(Vec3f ambientLight, Vec3f diffuse,
		Vec3f specular, float shininess) {
	this->ambientLight = ambientLight;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	return true;
}

Primitive::~Primitive() {

}
