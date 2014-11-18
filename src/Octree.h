/*
 * octree.h
 *
 *  Created on: 16 Kas 2014
 *      Author: engin
 */

#ifndef SRC_OCTREE_H_
#define SRC_OCTREE_H_

#include <vector>
#include <set>
#include <cstring>
#include "Vec3f.h"
#include "Primitive.h"
#include "Ray.h"

class Primitive;

class Octree {
	std::set<Primitive *> primitives;
	Octree* parent;
	Octree* children[8];
	Vec3f upperEnd, lowerEnd, center;
public:
	Octree(Octree*, Vec3f, Vec3f, std::vector<Primitive*>);
	virtual ~Octree();
	bool isRayIntersects(const Ray&) const;
	std::set<Primitive*> getIntersectingPrimitives(const Ray&) const;
};

#endif /* SRC_OCTREE_H_ */
