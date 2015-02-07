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
	return Ray(newPos, newDir, ray.getRefractionIndex(), ray.getDistance());
}

Vec3f Primitive::getColorForReflection(const Ray& ray, Vec4f& normal, Vec4f& intersectionPoint,
		const Octree& octree, const std::vector<Light>& lights,
		const unsigned int depth, const RayTracer* tracer) const {
		Vec4f reflectionDir = ray.getDirection()
				- 2 * Vec4fNS::dot(ray.getDirection(), normal) * normal;
		Ray reflectionRay(intersectionPoint, reflectionDir, ray.getRefractionIndex(), ray.getDistance());
		Vec3f reflectedColor = tracer->trace(reflectionRay, octree,
				lights, depth);
		return reflectedColor;
}

bool Primitive::setTransformation(const Mat4f& matrix) {
	this->transformMatrix = matrix;
	this->inverseTransformMat = Mat4f::inverse(matrix);
	this->generateBoundingBox(); //since transformations would change it.
	return true;
}

bool Primitive::setMaterial(Material* mat) {
	this->material = mat;
	return true;
}

Vec3f Primitive::getColorForRay(const Ray& ray, float distance,
		const Octree& octree, const std::vector<Light>& lights,
		const unsigned int depth, const RayTracer* tracer) const {
	Vec3f color;

	Vec4f intersectionPoint = distance * ray.getDirection() + ray.getPosition();
	Vec4f normal(this->calculateNormal(intersectionPoint), 0);
	Vec4f coloringNormal = normal;
	Vec4f raySideIntersectionPoint, transparencyIntersectionPoint;
	bool raySide = Vec4fNS::dot(normal, ray.getDirection()) < 0; //is ray opposite side as normal, so it can reflect/get light
	if(!raySide &&  material->getRefractionIndex()!=1.0){
		coloringNormal = -1 * coloringNormal;
	}
	if (raySide || material->getRefractionIndex()!=1.0) { //We assume objects are one sided, if we hit the opposide side, we won't calculate light/diffuse.
	//if (raySide) { //We assume objects are one sided, if we hit the opposide side, we won't calculate light/diffuse.
		//check if light is blocked or not
		raySideIntersectionPoint = intersectionPoint + EPSILON * 10.0f * coloringNormal;
		//the 10.0f is to make epsilon bigger, or it might still be in Spheres.

		for (unsigned int i = 0; i < lights.size(); i++) {
			Light it = lights[i];

			Vec4f direction;
			if (fabs(it.getPosition().w) < EPSILON) {
				direction = Vec4fNS::normalize(it.getPosition());
			} else {
				Vec4f lightPos = it.getPosition();
				lightPos = (1 / it.getPosition().w) * lightPos;
				direction = Vec4fNS::normalize(lightPos - raySideIntersectionPoint);
			}
			if (Vec4fNS::dot(coloringNormal, direction) > 0) {//is light on the same side as normal
				//std::cout << "entered" << std::endl;
				float ligthVisibility = tracer->traceToLight(raySideIntersectionPoint,
						*(&it), ray.getRefractionIndex(),octree);
//#pragma omp critical
//				std::cout << "light visibility is " << ligthVisibility << std::endl;
				if (ligthVisibility) {
					float lightDistance = ((Vec3f) (it.getPosition()
							- raySideIntersectionPoint)).length();	//casting to vec3 because w is 0
					Vec4f eyeDirn = Vec4fNS::normalize(
							ray.getPosition() - raySideIntersectionPoint);
					Vec4f halfVec = Vec4fNS::normalize(direction + eyeDirn);

					color = color
							+ ligthVisibility
									* it.getAttenuationFactor(lightDistance)
									* calculateColorPerLight(direction,
											it.getColor(), coloringNormal, halfVec,
											this->getDiffuse(raySideIntersectionPoint),
											material->getSpecular(),
											material->getShininess());
				}

			}
		}
	}


	//at this point, we are going to calculate transparency, so we should reverse the intersection point
	//if we are passing thru the wrong way, we need to remove the added normal
	if(raySide) {
		raySideIntersectionPoint = intersectionPoint + EPSILON * 10.0f * normal;
		transparencyIntersectionPoint = intersectionPoint - EPSILON * 10.0f *normal;
	} else {
		transparencyIntersectionPoint = intersectionPoint + EPSILON * 10.0f * normal;
		raySideIntersectionPoint = intersectionPoint - EPSILON * 10.0f *normal;
	}

	//these calculations are here because they are side free, but they are after refraction
	color = color + material->getAmbient() + material->getEmission();
	Vec3f reflectedColor;
	//if we have the depth to calculate reflection/refraction
	if (depth > 0) {


		//if refraction is defined
		if(material->getRefractionIndex() == 1.0f){
			//since object is not refractive, we can apply reflection directly
			//if there is a reflection
			if(material->getSpecular().length() > EPSILON){
				reflectedColor = material->getSpecular() * getColorForReflection(ray, normal, raySideIntersectionPoint,
					octree, lights,	depth, tracer);
				color = color + reflectedColor;
			}
		} else {
			//we need to calculate refraction, and add it according to fresnel law
			float cosineIntersection, refractionFrom, refractionTo;
			cosineIntersection = Vec3fNS::dot(ray.getDirection(),normal);
			if(cosineIntersection > 0){
				refractionFrom = material->getRefractionIndex();
				//FIXME this assumes objects wont be in each other
				refractionTo = 1.0f;
				normal = -1 * normal;
			} else {
				refractionFrom = ray.getRefractionIndex();
				refractionTo = material->getRefractionIndex();
				cosineIntersection = cosineIntersection * -1;
			}
			float cosineRefractionsq = 1.0f - ((refractionFrom/refractionTo)*(refractionFrom/refractionTo))*(1.0f - cosineIntersection * cosineIntersection);
			//if the angle is bigger than critical, it will reflect only.
			if(cosineRefractionsq < 0.0f){//total internal reflection
				color = color + material->getSpecular() * getColorForReflection(ray, normal, raySideIntersectionPoint,
						octree, lights,	depth, tracer);
			} else {
				float cosineRefraction = sqrtf(cosineRefractionsq);
				//fresnel  if there will be a reflection.
				float fresnelCoef = (pow((refractionFrom * cosineIntersection - refractionTo * cosineRefraction) / (refractionFrom * cosineIntersection + refractionTo * cosineRefraction), 2.0f) +
						pow((refractionTo * cosineIntersection - refractionFrom * cosineRefraction) / (refractionFrom * cosineRefraction + refractionTo * cosineRefraction), 2.0f)) * 0.5f;
				//fresnels transparency factor
				float fresnelTrans = (((2*refractionFrom*cosineIntersection)/(refractionFrom*cosineIntersection + refractionTo*cosineRefraction)) +
						((2*refractionFrom*cosineIntersection)/(refractionFrom*cosineRefraction + refractionTo*cosineIntersection))) * 0.5;
				//FIXME there is a reflection factor too, I did not implement it yet.
				Vec3f refractedColor;
				reflectedColor = Vec3fNS::clamp(reflectedColor,0,1);
				if(fresnelCoef <= 1.0f){
					Vec3f refraction= (refractionFrom/refractionTo) * ray.getDirection() + ((refractionFrom/refractionTo)*cosineIntersection - cosineRefraction) * normal;
					//we are updating refraction index of ray, and reseting distance
					Ray refractionRay(transparencyIntersectionPoint, refraction.normalize(), material->getRefractionIndex(), 0);
					refractedColor = tracer->trace(refractionRay, octree,
							lights, depth);
					refractedColor = Vec3fNS::clamp(refractedColor, 0, 1);
				}
				//FIXME we need to color for the distance that ray was in the object
				//known as Beers law
					color = (1.0 -fresnelTrans)*(color +  material->getSpecular() * getColorForReflection(ray, normal, raySideIntersectionPoint,
							octree, lights,	depth, tracer));
					color = color + (fresnelTrans)*refractedColor;
			}

		}
	}

	//we calculate dissolve after ambient/emission because they are effected by it.
	//if a material is dissolved, it is transparent, but not refractive, so direction is same
	if(material->getDissolve() < 1.0f){
		if (depth > 0) {
			Ray dissolveRay(raySideIntersectionPoint, ray.getDirection(), 0, 100);
			Vec3f dissolveColor = tracer->trace(dissolveRay, octree,
					lights, depth);
			dissolveColor = Vec3fNS::clamp(dissolveColor, 0, 1);
			//std::cout << "reflection " << reflectedColor << std::endl;
			color = (material->getDissolve() * color) + ((1.0f - material->getDissolve()) * dissolveColor);

		}
	}

	return color;
}

Primitive::~Primitive() {

}

unsigned char Primitive::isInBoundingBox(const Vec3f& upperEnd,
		const Vec3f& lowerEnd) const {

	//if the bounding box is completely in the octree box or in contact
	/**
	 * this check is the most probably fastest possible check
	 * If no contact, given upper must be lower than this lower,
	 * or given lower must be bigger than this upper.
	 */
	if (upperEnd.x > this->bbLower.x && lowerEnd.x < this->bbUpper.x
			&& upperEnd.y > this->bbLower.y && lowerEnd.y < this->bbUpper.y
			&& upperEnd.z > this->bbLower.z && lowerEnd.z < this->bbUpper.z) {
		return 1;
	}
	//intersection or inclusion is not found
	return 0;

}
