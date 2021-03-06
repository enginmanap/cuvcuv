/*
 * Model.cpp
 *
 *  Created on: 24 Ara 2014
 *      Author: Engin Manap
 */

#include "Model.h"

Model::Model(const Mat4f& transformMatrix) :
		triangleCount(0) {
	this->setTransformation(transformMatrix);
	this->vertexVector.reserve(INITIAL_VERTEX_COUNT);
	this->vertexNormalVector.reserve(INITIAL_VERTEX_COUNT);
	this->spatialTree = NULL;
	//this is because obj files start counting from 1, and it is better to add 1 empty vertex than
	//calculate with -1 for all vertexes.
	this->vertexVector.push_back(Vec3f(0, 0, 0));
	this->vertexNormalVector.push_back(Vec3f(0, 0, 0));
	this->vertexTextureCoordinateVector.push_back(Vec3f(0, 0, 0));
}

Model::~Model() {
	if (this->spatialTree != NULL)
		delete this->spatialTree;
}

bool Model::createVertexSpace(int maxVertexCount) {
	vertexVector.clear();
	vertexVector.reserve(maxVertexCount);

	return true;
}

void Model::printVertexes() {
	for (unsigned int i = 0; i < vertexVector.size(); ++i) {
		std::cout << "vertex[" << i << "]=" << this->vertexVector[i]
				<< std::endl;
	}
}

/**
 * if the indexes are negative, it means reverse order,
 * this function updates them to in order. If indexes are bigger than
 * current max, it returns false.
 *
 * @params
 * vertice1,vertice2,vertice3: vertex indexes
 *
 * @returns:
 * true if the indexes are valid, false if they are not.
 */
bool Model::verifyTriangleIndexes(const std::vector<Vec3f> &vector,
		int& value1, int& value2, int& value3) {
	int currentMaxValue = vector.size();
	//negative index means starting from last. last vertice is -1
	if (value1 < 0)
		value1 = currentMaxValue + value1;
	if (value2 < 0)
		value2 = currentMaxValue + value2;
	if (value3 < 0)
		value3 = currentMaxValue + value3;
	if (value1 < currentMaxValue && value2 < currentMaxValue
			&& value3 < currentMaxValue) {
		return true;
	} else {
		std::cerr << "one of the value used for face definition is not defined ("
				<< currentMaxValue << ") ";
		if (value1 >= currentMaxValue)
			std::cerr << " value 1 bigger " << value1;
		if (value2 >= currentMaxValue)
			std::cerr << " value 2 bigger " << value2;
		if (value3 >= currentMaxValue)
			std::cerr << " value 3 bigger " << value3;

		std::cerr << std::endl;
		return false;
	}
}

bool Model::addTriangleBase(int vertice1, int vertice2, int vertice3) {
	if (verifyTriangleIndexes(this->vertexVector, vertice1, vertice2, vertice3)) {
		TriangleBase* triangle = new TriangleBase(this->vertexVector[vertice1],
				this->vertexVector[vertice2], this->vertexVector[vertice3],
				this->transformMatrix);
		triangle->setMaterial(this->material);

		//std::cout << "new triangle with material: " << this->material->getName() << std::endl;
		//triangle->setTransformation(transformStack.top());
		primitives.push_back(triangle);
		triangleCount++;
		return true;
	}
	return false;
}

bool Model::addTriangle(int vertice1, int vertice2, int vertice3, int normal1,
		int normal2, int normal3, int texture1, int texture2, int texture3) {
	//this if also rearranges negative indexes, so it has huge side effect.
	if (verifyTriangleIndexes(this->vertexVector, vertice1, vertice2, vertice3) &&
			verifyTriangleIndexes(this->vertexNormalVector, normal1, normal2, normal3) &&
			verifyTriangleIndexes(this->vertexTextureCoordinateVector, texture1, texture2, texture3)) {
		Triangle* triangle = new Triangle(this->vertexVector[vertice1],
				this->vertexVector[vertice2], this->vertexVector[vertice3],
				this->vertexNormalVector[normal1],
				this->vertexNormalVector[normal2],
				this->vertexNormalVector[normal3], this->transformMatrix, true);
		triangle->setTextureCoordinates(
				vertexTextureCoordinateVector[texture1].x,
				vertexTextureCoordinateVector[texture1].y,
				vertexTextureCoordinateVector[texture2].x,
				vertexTextureCoordinateVector[texture2].y,
				vertexTextureCoordinateVector[texture3].x,
				vertexTextureCoordinateVector[texture3].y);
		triangle->setMaterial(this->material);

		//std::cout << "new triangle with material: " << this->material->getName() << std::endl;
		//triangle->setTransformation(transformStack.top());
		primitives.push_back(triangle);
		triangleCount++;
		return true;
	}
	return false;
}

void Model::buildOctree() {
	std::cout << "generating spatial tree for model" << std::endl;
	generateBoundingBox();
	/*
	 * This requirement is lifted
	 / *
	 * since we have the bbUpper and bbLower values, we should buid a Octree to
	 * contain all of them. I want the size to be a power of 2, and
	 * it should be a cube
	 * /
	 Vec3f lengths = bbUpper - bbLower;
	 double sceneSize = std::max(lengths.x, std::max(lengths.y, lengths.z));
	 int treeRootSize = pow(2, std::ceil(log(sceneSize) / log(2))); //this generates smallest power of 2 that is big equal to size

	 //now we have the size and min will be floored, so we can calculate max
	 double treeRootMaxX = treeRootSize + std::floor(bbLower.x);
	 double treeRootMaxY = treeRootSize + std::floor(bbLower.y);
	 double treeRootMaxZ = treeRootSize + std::floor(bbLower.z);
	 Vec3f treeMax(treeRootMaxX, treeRootMaxY, treeRootMaxZ);
	 Vec3f treeMin(std::floor(bbLower.x), std::floor(bbLower.y),
	 std::floor(bbLower.z));
	 */
	//now request a octree with this dimentions.
	this->spatialTree = new Octree(NULL, bbUpper, bbLower, primitives, 10);	//TODO 10 is hardcoded max depth
	std::cout << "spatial tree for model generated with dimentions: " << bbUpper
			<< "," << bbLower << std::endl;
	//this->spatialTree->print();
}

void Model::generateBoundingBox() {

	Vec3f currentBBUpper, currentBBLower;
	for (std::vector<Primitive*>::iterator primitiveP = primitives.begin();
			primitiveP != primitives.end(); ++primitiveP) {
		currentBBUpper = (*primitiveP)->getBBUpper();
		currentBBLower = (*primitiveP)->getBBLower();
		if (currentBBUpper.x > bbUpper.x)
			bbUpper.x = currentBBUpper.x;
		if (currentBBUpper.y > bbUpper.y)
			bbUpper.y = currentBBUpper.y;
		if (currentBBUpper.z > bbUpper.z)
			bbUpper.z = currentBBUpper.z;

		if (currentBBLower.x < bbLower.x)
			bbLower.x = currentBBLower.x;
		if (currentBBLower.y < bbLower.y)
			bbLower.y = currentBBLower.y;
		if (currentBBLower.z < bbLower.z)
			bbLower.z = currentBBLower.z;
	}
	bbCenter.x = (bbUpper.x + bbLower.x) / 2;
	bbCenter.y = (bbUpper.y + bbLower.y) / 2;
	bbCenter.z = (bbUpper.z + bbLower.z) / 2;
	std::cout << "bounding box for model: " << bbUpper << "," << bbLower
			<< std::endl;
}

bool Model::intersectiontest(Ray ray, double& distance,
		Primitive** intersectingPrimitive) const {
	double closestdistance = std::numeric_limits<double>::max(); // this is the maximum value double can have, min() returns min positive value.
	double intersectionDistance;
	std::set<Primitive*> primitives;
	bool isIntersecting = false;

	spatialTree->getIntersectingPrimitives(ray, primitives);
	if (!primitives.empty()) {
		for (std::set<Primitive*>::const_iterator it = primitives.begin();
				it != primitives.end(); ++it) {
			if ((*it)->intersectiontest(ray, intersectionDistance,
					intersectingPrimitive)) {
				//found intersection
				if (closestdistance > intersectionDistance) {
					closestdistance = intersectionDistance; //this is to keep the closest one
					distance = intersectionDistance; //this is to return
					isIntersecting = true;
					*intersectingPrimitive = *it;
				}

			}
		}
	}
	return isIntersecting;
}
