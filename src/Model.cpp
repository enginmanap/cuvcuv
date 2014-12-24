/*
 * Model.cpp
 *
 *  Created on: 24 Ara 2014
 *      Author: Engin Manap
 */

#include "Model.h"

Model::Model(const Mat4f& transformMatrix) : maxVertexCount(1000), currentVertex(0), triangleCount(0){
			this->setTransformation(transformMatrix);
			this->vertexVector.reserve(maxVertexCount);
			this->spatialTree=NULL;
}

Model::~Model() {
	// TODO Auto-generated destructor stub
}

bool Model::createVertexSpace(int maxVertexCount) {
	vertexVector.clear();
	this->maxVertexCount = maxVertexCount;
	vertexVector.reserve(this->maxVertexCount);

	currentVertex = 0;
	return true;
}

bool Model::addVertex(float x, float y, float z) {
	if (currentVertex == maxVertexCount)
		return false;
	this->vertexVector[currentVertex] = Vec3f(x, y, z);
	currentVertex++;
	return true;
}

void Model::printVertexes() {
	for (int i = 0; i < this->maxVertexCount; ++i) {
		std::cout << "vertex[" << i << "]=" << this->vertexVector[i]
				<< std::endl;
	}
}

bool Model::addTriangle(int vertice1, int vertice2, int vertice3) {
	if (vertice1 >= 0 && vertice2 >= 0 && vertice3 >= 0
			&& vertice1 < currentVertex && vertice2 < currentVertex
			&& vertice3 < currentVertex) {
		Triangle* triangle = new Triangle(this->vertexVector[vertice1],
				this->vertexVector[vertice2], this->vertexVector[vertice3],this->transformMatrix);
		triangle->setMaterial(this->material);
		std::cout << "new triangle with material: " << this->material->getName() << std::endl;
		//triangle->setTransformation(transformStack.top());
		primitives.push_back(triangle);
		triangleCount++;
		return true;
	} else {
		std::cerr << "one of the vertices used is not defined ";
		if(vertice1 < 0 ) std::cerr << "vertex 1 "<< vertice1;
		if(vertice2 < 0 ) std::cerr << "vertex 2 "<< vertice2;
		if(vertice3 < 0 ) std::cerr << "vertex 3 "<< vertice3;
		if(vertice1 >= currentVertex) std::cerr << " vertex 1 bigger " << vertice1;
		if(vertice2 >= currentVertex) std::cerr << " vertex 2 bigger " << vertice2;
		if(vertice3 >= currentVertex) std::cerr << " vertex 3 bigger " << vertice3;

		std::cerr << std::endl;
		return false;
	}
}

void Model::buildOctree() {
	std::cout << "generating spatial tree.." << std::endl;

	/*
	 * since we have the bbUpper and bbLower values, we should buid a Octree to
	 * contain all of them. I want the size to be a power of 2, and
	 * it should be a cube
	 */
	Vec3f lengths = bbUpper - bbLower;
	float sceneSize = std::max(lengths.x, std::max(lengths.y, lengths.z));
	int treeRootSize = pow(2, std::ceil(log(sceneSize) / log(2))); //this generates smallest power of 2 that is big equal to size

	//now we have the size and min will be floored, so we can calculate max
	float treeRootMaxX = treeRootSize + std::floor(bbLower.x);
	float treeRootMaxY = treeRootSize + std::floor(bbLower.y);
	float treeRootMaxZ = treeRootSize + std::floor(bbLower.z);
	Vec3f treeMax(treeRootMaxX, treeRootMaxY, treeRootMaxZ);
	Vec3f treeMin(std::floor(bbLower.x), std::floor(bbLower.y),
			std::floor(bbLower.z));
	//now request a octree with this dimentions.
	this->spatialTree = new Octree(NULL, treeMax, treeMin, primitives);
	std::cout << "spatial tree for model generated with dimentions: " << treeMax << ","
			<< treeMin << std::endl;
	//this->spatialTree->print();
}

void Model::generateBoundingBox(){

	Vec3f currentBBUpper, currentBBLower;
	for (std::vector<Primitive*>::iterator primitiveP = primitives.begin();
			primitiveP != primitives.end(); ++primitiveP) {
		currentBBUpper = (*primitiveP)->getBBUpper();
		currentBBLower = (*primitiveP)->getBBLower();
		if (currentBBUpper.x > bbUpper.x)
			bbUpper.x = currentBBUpper.x;
		if (currentBBUpper.y > bbUpper.y)
			bbUpper.z = currentBBUpper.y;
		if (currentBBUpper.z > bbUpper.z)
			bbUpper.y = currentBBUpper.z;

		if (currentBBLower.x < bbLower.x)
			bbLower.x = currentBBLower.x;
		if (currentBBLower.y < bbLower.y)
			bbLower.y = currentBBLower.y;
		if (currentBBLower.z < bbLower.z)
			bbLower.z = currentBBLower.z;
	}
	bbCenter.x = (bbUpper.x + bbLower.x)/2;
	bbCenter.y = (bbUpper.y + bbLower.y)/2;
	bbCenter.z = (bbUpper.z + bbLower.z)/2;
}

bool Model::intersectiontest(Ray ray, float& distance) const{
	float closestdistance = std::numeric_limits<float>::max(); // this is the maximum value float can have, min() returns min positive value.
	float intersectionDistance;
	std::set<Primitive*> primitives;
	bool isIntersecting=false;

	spatialTree->getIntersectingPrimitives(ray,primitives);

	if (!primitives.empty()) {
		for (std::set<Primitive*>::const_iterator it = primitives.begin();
				it != primitives.end(); ++it) {
			if ((*it)->intersectiontest(ray, intersectionDistance)) {
				//found intersection
				if (closestdistance > intersectionDistance) {
					closestdistance = intersectionDistance;//this is to keep the closest one
					distance=intersectionDistance;//this is to return
					isIntersecting=true;
				}

			}
		}
	}
	return isIntersecting;
}