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
#include "Material.h"

#ifndef EPSILON
#define EPSILON 0.001F
#endif

class Octree;

class Primitive {
public:
	unsigned int id;
protected:
	static unsigned int lastID;
	Material* material;
	Mat4f inverseTransformMat, transformMatrix;
	Vec3f bbUpper, bbLower, bbCenter, bbwidths;

	Vec3f calculateColorPerLight(const Vec3f&, const Vec3f&, const Vec3f&,
			const Vec3f&, const Vec3f&, const Vec3f&, const float) const;
	virtual Vec3f calculateNormal(const Vec4f&) const = 0;
	virtual Vec3f getDiffuse(const Vec3f&) const { return this->material->getDiffuse();};
	virtual void generateBoundingBox() = 0;
public:
	Vec3f getBBUpper(){ return bbUpper;}
	Vec3f getBBLower(){ return bbLower;}

	Primitive():id(++lastID), material(0) {};
	virtual ~Primitive();
	virtual bool setTransformation(const Mat4f&);
	virtual bool intersectiontest(Ray, float&, Primitive**) const= 0;
	Vec3f getColorForRay(const Ray&, float,
			const Octree&, const std::vector<Light>&,
			const unsigned int) const;
	bool setMaterial(Material*);

	Ray generateTransformedRay(const Ray&) const;
	/**
	 * return 0 means not in, 1 means partial or fully in.
	 */
	unsigned char isInBoundingBox(const Vec3f&, const Vec3f&) const;
};

#endif /* PRIMITIVE_H_ */
