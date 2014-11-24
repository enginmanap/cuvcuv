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
#include <iostream>
#include <sstream>
#include "Vec3f.h"
#include "Primitive.h"
#include "Ray.h"

class Primitive;

class Octree {
	std::set<Primitive *> primitives;
	Octree* parent;
	Octree* children[8];
	Vec3f upperEnd, lowerEnd, center;
	unsigned char isSplittingRedudant;
public:
	Octree(Octree*, Vec3f&, Vec3f&, std::vector<Primitive*>&);
	virtual ~Octree();
	unsigned char getIsSplittingRedudant(){ return isSplittingRedudant;}
	bool isRayIntersects(const Ray&) const;
	void getIntersectingPrimitives(const Ray&, std::set<Primitive*>&) const;
};

#endif /* SRC_OCTREE_H_ */
