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

#ifndef INITIAL_VERTEX_COUNT
#define INITIAL_VERTEX_COUNT 1000
#endif //INITIAL_VERTEX_COUNT

class Model : public Primitive {
	std::vector<Vec3f> vertexVector;

	int triangleCount;
	std::vector<Primitive*> primitives;

	Octree *spatialTree;
	void generateBoundingBox();
	Vec3f calculateNormal(const Vec4f& intersectionPoint) const {return Vec3f();};//FIXME this is a placeholder

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
