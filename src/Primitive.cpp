/*
 * Primitive.cpp
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#include "Primitive.h"

unsigned int Primitive::lastID = 0;

Vec3f Primitive::calculateColorPerLight(const Vec3f& direction,
		const Vec3f& color, const Vec3f& normal, const Vec3f& halfVec,
		const Vec3f& diffuse, const Vec3f& specular,
		const float shininess) const {

	Vec3f lambert, phong;

	float nDotl = Vec3fNS::dot(normal, direction);
	float nDotH = Vec3fNS::dot(normal, halfVec);

	lambert = diffuse * color;
	lambert = std::max(nDotl, 0.0f) * lambert;

	phong = specular * color;
	phong = pow(std::max(nDotH, 0.0f), shininess) * phong;

	return lambert + phong;

}

Ray Primitive::generateTransformedRay(const Ray& ray) const {
	//since direction has 0 as last element, translate became 0 too
	Vec4f newPos = ray.getPosition() * inverseTransformMat;
	Vec4f newDir = ray.getDirection() * inverseTransformMat;
	return Ray(newPos,newDir, 0, 100);
}

bool Primitive::setTransformation(const Mat4f& matrix) {
	this->transformMatrix = matrix;
	this->inverseTransformMat = Mat4f::inverse(matrix);
	this->generateBoundingBox();//since transformations would change it.
	return true;
}

bool Primitive::setMaterial(Material* mat) {
	this->material = mat;
	return true;
}

Vec3f Primitive::getColorForRay(const Ray& ray, float distance,
		const Octree& octree,
		const std::vector<Light>& lights, const unsigned int depth, const RayTracer* tracer) const {
	Vec3f color;

	Vec4f intersectionPoint = distance * ray.getDirection();
	intersectionPoint = intersectionPoint + ray.getPosition();
	Vec4f normal(this->calculateNormal(intersectionPoint),0);
	bool raySide = Vec4fNS::dot(normal,ray.getDirection()) < 0;//is ray opposite side as normal, so it can reflect/get light
	if(raySide){//We assume objects are one sided, if we hit the opposide side, we won't calculate light/diffuse.
		//check if light is blocked or not
		intersectionPoint = intersectionPoint + EPSILON * 10.0f *normal;
		//the 10.0f is to make epsilon bigger, or it might still be in Spheres.

		for (unsigned int i = 0; i < lights.size(); i++) {
			Light it = lights[i];

			Vec4f direction;
			if (fabs(it.getPosition().w) < EPSILON) {
				direction = Vec4fNS::normalize(it.getPosition());
			} else {
				Vec4f lightPos = it.getPosition();
				lightPos = (1 / it.getPosition().w) * lightPos;
				direction = Vec4fNS::normalize(lightPos - intersectionPoint);
			}
			if(Vec4fNS::dot(normal,direction) > 0){//is light on the same side as normal
				//std::cout << "entered" << std::endl;
				float ligthVisibility = tracer->traceToLight(intersectionPoint, octree, *(&it));
//#pragma omp critical
//				std::cout << "light visibility is " << ligthVisibility << std::endl;
				if (ligthVisibility) {
					float lightDistance =
							((Vec3f)(it.getPosition() - intersectionPoint)).length();//casting to vec3 because w is 0
					Vec4f eyeDirn = Vec4fNS::normalize(
							ray.getPosition() - intersectionPoint);
					Vec4f halfVec = Vec4fNS::normalize(direction + eyeDirn);

					color = color + ligthVisibility	* it.getAttenuationFactor(lightDistance) * calculateColorPerLight(direction, it.getColor(),	normal, halfVec, this->getDiffuse(intersectionPoint),  material->getSpecular(), material->getShininess());
				}

			}
		}
		//now we have the color for this object itself, calculate reflections.
		if (fabs( material->getSpecular().x) < EPSILON && fabs( material->getSpecular().y) < EPSILON
				&& fabs( material->getSpecular().z) < EPSILON) {
			//the object is not reflective, so stop here
		} else {
			if (depth > 0) {

				Vec4f reflectionDir = ray.getDirection()- 2 * Vec4fNS::dot(ray.getDirection(), normal) * normal;
				Ray reflectionRay(intersectionPoint,reflectionDir, 0, 100);
				Vec3f reflectedColor = tracer->trace(reflectionRay, octree,
						lights, depth);
				reflectedColor = Vec3fNS::clamp(reflectedColor, 0, 1);
				//std::cout << "reflection " << reflectedColor << std::endl;
				color = color + material->getSpecular() * reflectedColor;


			}
		}
	}
	//these calculations are left because they are side free (also makes debugging easier)
	color = color + material->getAmbient() + material->getEmission();
	return color;
}

Primitive::~Primitive() {

}


unsigned char Primitive::isInBoundingBox(const Vec3f& upperEnd, const Vec3f& lowerEnd) const {

	//if the bounding box is completely in the octree box or in contact
	/**
	 * this check is the most probably fastest possible check
	 * If no contact, given upper must be lower than this lower,
	 * or given lower must be bigger than this upper.
	 */
	if(upperEnd.x > this->bbLower.x &&
		lowerEnd.x < this->bbUpper.x &&
		upperEnd.y > this->bbLower.y &&
		lowerEnd.y < this->bbUpper.y &&
		upperEnd.z > this->bbLower.z &&
		lowerEnd.z < this->bbUpper.z
		) {
		return 1;
	}
	//intersection or inclusion is not found
	return 0;

}
