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
#include "TriangleBase.h"
#include "Triangle.h"

#ifndef INITIAL_VERTEX_COUNT
#define INITIAL_VERTEX_COUNT 1000
#endif //INITIAL_VERTEX_COUNT

class Model: public Primitive {
	std::vector<Vec3f> vertexVector;
	std::vector<Vec3f> vertexNormalVector;
	std::vector<Vec3f> vertexTextureCoordinateVector;

	int triangleCount;
	std::vector<Primitive*> primitives;

	Octree *spatialTree;
	void generateBoundingBox();
	Vec3f calculateNormal(const Vec4f& intersectionPoint) const {
		return Vec3f();
	}
	; //this is a placeholder, it should newer called
	bool verifyTriangleIndexes(const std::vector<Vec3f>&,int&, int&, int&);
public:
	Model(const Mat4f&);

	virtual ~Model();

	bool createVertexSpace(int);
	void printVertexes();

	void addVertex(double x, double y, double z) {
		this->vertexVector.push_back(Vec3f(x, y, z));
	}
	;
	void addVertexNormal(double x, double y, double z) {
		this->vertexNormalVector.push_back(Vec3f(x, y, z));
	}
	;
	void addVertexTextureCoordinate(double x, double y) {
		this->vertexTextureCoordinateVector.push_back(Vec3f(x, y, 0));
	}
	;
	bool addTriangleBase(int, int, int);
	bool addTriangle(int, int, int, int, int, int, int, int, int);

	void buildOctree();

	bool intersectiontest(Ray, double&, Primitive**) const;

};

#endif /* SRC_MODEL_H_ */
