/*
 * octree.h
 *
 *  Created on: 16 Kas 2014
 *      Author: engin
 */

#ifndef SRC_OCTREE_H_
#define SRC_OCTREE_H_

#include <vector>
#include "Vec3f.h"
#include "Primitive.h"

class Octree {
public:
	Octree(Octree*, Vec3f, Vec3f, std::vector<Primitive*>);
	virtual ~Octree();
	std::vector<Primitive*> primitives;
	Octree* parent;
	Octree* children[8];
	Vec3f upperEnd, lowerEnd, center;
};

#endif /* SRC_OCTREE_H_ */
