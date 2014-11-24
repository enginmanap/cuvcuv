/*
 * Primitive.h
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include <vector>

#include "Octree.h"
#include "Light.h"
#include "Vec3f.h"
#include "Vec4f.h"
#include "Ray.h"
#include "Mat4f.h"

#ifndef EPSILON
#define EPSILON 0.001F
#endif

class Octree;

class Primitive {

protected:
	Vec3f ambientLight, emissionLight, specular, diffuse;
	float shininess;
	Mat4f inverseTransformMat, transformMatrix;
	Ray transformedRay;
	Vec3f bbUpper, bbLower, bbCenter, bbwidths;

	static unsigned int lastID;
	Vec3f calculateColorPerLight(const Vec3f&, const Vec3f&, const Vec3f&,
			const Vec3f&, const Vec3f&, const Vec3f&, const float) const;
	virtual Vec3f calculateNormal(const Vec4f&) const = 0;
	virtual void generateBoundingBox() = 0;
public:
	Vec3f getBBUpper(){ return bbUpper;}
	Vec3f getBBLower(){ return bbLower;}
	unsigned int id;
	Primitive();
	virtual ~Primitive();
	virtual bool setTransformation(const Mat4f&);
	virtual bool intersectiontest(Ray, float&) const= 0;
	Vec3f getColorForRay(const Ray&, float,
			const Octree&, const std::vector<Light>&,
			const unsigned int) const;
	bool setLightValues(Vec3f&, Vec3f&, Vec3f&, Vec3f&, float);

	Ray generateTransformedRay(const Ray&) const;
	/**
	 * return 0 means not in, 1 means partially in, 2 mean contained in.
	 */
	unsigned char isInBoundingBox(const Vec3f&, const Vec3f&) const;
};

#endif /* PRIMITIVE_H_ */
