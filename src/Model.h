/*
 * Model.h
 *
 *  Created on: 24 Ara 2014
 *      Author: Engin Manap
 */

#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_


#include <string>
#include "Primitive.h"
#include "Triangle.h"



class Model : public Primitive {
	int maxVertexCount, currentVertex;
	std::vector<Vec3f> vertexVector;

	int triangleCount;
	std::vector<Primitive*> primitives;

	Octree *spatialTree;
	void generateBoundingBox();

public:
	Model(const Mat4f&);

	virtual ~Model();

	bool createVertexSpace(int);
	void printVertexes();

	bool addVertex(float, float, float);
	bool addTriangle(int, int, int);

	void buildOctree();

	bool intersectiontest(Ray, float&) const;
};

#endif /* SRC_MODEL_H_ */
