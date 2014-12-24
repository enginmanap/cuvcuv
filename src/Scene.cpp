/*
 * Scene.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Scene.h"

int Scene::materialCount = 0;

/**
 * A scene starts definition starts with the sample size,
 * so we are setting height width, they will be passed to camera;
 */
Scene::Scene(unsigned int height, unsigned int width): height(height), width(width), currentAttenuation(Vec3f(1,0,0)), maxVertexCount(2000), currentVertex(0), SphereCount(0), triangleCount(0), lightCount(0) {
	this->camera = NULL;
	this->vertexVector.reserve(maxVertexCount);

	currentMaterial = new Material(DEFAULT_MATERIAL_NAME);
	materialMap[currentMaterial->getName()] = currentMaterial;
	//materialNames.push(currentMaterial->getName());


	//mat->setShininess = 0.0f;
	//mat->setAttenuation = Vec3f(1, 0, 0); these are default

	this->saveFilename = "output.png";//TODO: this should be same as input

	transformStack.push(Mat4f()); //since default constructor generates identity matrix.

	this->maxDepth = 5;

	this->spatialTree = NULL;


	this->sampleRate = 1;
	film = new Film(height,width,COLOR_DEPTH,sampleRate);

}

Scene::~Scene() {
	if (camera != NULL)
		delete camera;

	for (std::vector<Primitive*>::iterator it = primitives.begin();
			it != primitives.end(); ++it) {
		delete (*it);
	}

	std::map<std::string, Material*>::iterator itr = materialMap.begin();
	while (itr != materialMap.end()) {
		   std::map<std::string, Material*>::iterator toErase = itr;
		   ++itr;
		   delete toErase->second;
		   materialMap.erase(toErase);
	}

	delete spatialTree;
	delete film;
}


void Scene::setSampleRate(unsigned char samplingRate){
	 this->sampleRate = samplingRate;
	 if(film != NULL)
		 delete film;

	 film = new Film(height,width,COLOR_DEPTH,sampleRate);
}

bool Scene::setSaveFilename(std::string filename) {
	this->saveFilename = filename;
	return true;
}

std::string Scene::getSaveFilename() {
	return saveFilename;
}

bool Scene::pushTransform() {
	transformStack.push(transformStack.top());

	materialNames.push(currentMaterial->getName());

	std::ostringstream outputStringStream;
	outputStringStream << DEFAULT_MATERIAL_NAME << "_"<< ++materialCount;
	currentMaterial = new Material(outputStringStream.str(),currentMaterial->getAmbient(), currentMaterial->getDiffuse(), currentMaterial->getSpecular(), currentMaterial->getEmission(), currentMaterial->getShininess());
	materialMap[currentMaterial->getName()] = currentMaterial;

	std::cout << "adding material with name" << currentMaterial->getName()<<std::endl;


	return true;
}

Mat4f Scene::popTransform() {
	if (transformStack.size() <= 1) {
		std::cerr << "no transform to pop" << std::endl;
		return Mat4f();
	}
	Mat4f temp = transformStack.top();
	transformStack.pop();

	std::cout << "deleting material with name" << currentMaterial->getName() << std::endl;
	currentMaterial = materialMap[materialNames.top()];
	materialNames.pop();

	return temp;
}

Mat4f Scene::addTransform(Mat4f& transform) {
	transformStack.top() = transform * transformStack.top();
	return transformStack.top();
}

bool Scene::setCamera(float lookfromx, float lookfromy, float lookfromz,
		float lookatx, float lookaty, float lookatz, float upx, float upy,
		float upz, float fovy) {
	if (camera != NULL) {
		delete camera;
	}
	camera = new Camera(lookfromx, lookfromy, lookfromz, lookatx, lookaty,
			lookatz, upx, upy, upz, fovy, this->height,
			this->width);
	return true;
}

bool Scene::createVertexSpace(int maxVertexCount) {
	vertexVector.clear();
	this->maxVertexCount = maxVertexCount;
	vertexVector.reserve(this->maxVertexCount);

	currentVertex = 0;
	return true;
}


bool Scene::getSamplingSize(unsigned int& height, unsigned int& width) {
	height = this->height;
	width = this->width;
	return true;
}

bool Scene::setCurrentEmission(float x, float y, float z) {
	temproryVector.x = x;
	temproryVector.y = y;
	temproryVector.z = z;
	currentMaterial->setEmission(temproryVector);
	return true;
}

bool Scene::setCurrentAmbient(float x, float y, float z) {
	temproryVector.x = x;
	temproryVector.y = y;
	temproryVector.z = z;
	currentMaterial->setAmbient(temproryVector);

	return true;
}

bool Scene::setCurrentDiffuse(float x, float y, float z) {
	temproryVector.x = x;
	temproryVector.y = y;
	temproryVector.z = z;
	currentMaterial->setDiffuse(temproryVector);
	return true;
}

bool Scene::setCurrentSpecular(float x, float y, float z) {
	temproryVector.x = x;
	temproryVector.y = y;
	temproryVector.z = z;
	currentMaterial->setSpecular(temproryVector);
	return true;
}

bool Scene::setCurrentShininess(float shininess) {
	currentMaterial->setShininess(shininess);
	return true;
}

bool Scene::setCurrentAttenuation(float constant, float lineer,
		float quadratic) {
	this->currentAttenuation.x = constant;
	this->currentAttenuation.y = lineer;
	this->currentAttenuation.z = quadratic;
	return true;
}

bool Scene::addVertex(float x, float y, float z) {
	if (currentVertex == maxVertexCount)
		return false;
	this->vertexVector[currentVertex] = Vec3f(x, y, z);
	currentVertex++;
	return true;
}

//FIXME maxVertexCount does not mean we have that much vertex
void Scene::printVertexes() {
	for (int i = 0; i < this->maxVertexCount; ++i) {
		std::cout << "vertex[" << i << "]=" << this->vertexVector[i]
				<< std::endl;
	}
}

bool Scene::addTriangle(int vertice1, int vertice2, int vertice3) {
	if (vertice1 >= 0 && vertice2 >= 0 && vertice3 >= 0
			&& vertice1 < currentVertex && vertice2 < currentVertex
			&& vertice3 < currentVertex) {
		Triangle* triangle = new Triangle(this->vertexVector[vertice1],
				this->vertexVector[vertice2], this->vertexVector[vertice3],transformStack.top());
		triangle->setMaterial(currentMaterial);
		std::cout << "new triangle with material: " << currentMaterial->getName() << std::endl;
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

bool Scene::addSphere(float x, float y, float z, float radius) {
	Sphere* sphere = new Sphere(x, y, z, radius,transformStack.top());
	sphere->setMaterial(currentMaterial);
	primitives.push_back(sphere);
	//sphere->setTransformation(transformStack.top());
	SphereCount++;
	std::cout << "add sphere " << sphere->id << " with the values (" << x
			<< ", " << y << ", " << z << ") and radius: " << radius
			<< std::endl;
	return true;
}

void Scene::buildOctree() {
	std::cout << "generating spatial tree.." << std::endl;
	//calculate the size we need.
	Vec3f maxbb, minbb;
	Vec3f currentBBUpper, currentBBLower;
	for (std::vector<Primitive*>::iterator primitiveP = primitives.begin();
			primitiveP != primitives.end(); ++primitiveP) {
		currentBBUpper = (*primitiveP)->getBBUpper();
		currentBBLower = (*primitiveP)->getBBLower();
		if (currentBBUpper.x > maxbb.x)
			maxbb.x = currentBBUpper.x;
		if (currentBBUpper.y > maxbb.y)
			maxbb.z = currentBBUpper.y;
		if (currentBBUpper.z > maxbb.z)
			maxbb.y = currentBBUpper.z;

		if (currentBBLower.x < minbb.x)
			minbb.x = currentBBLower.x;
		if (currentBBLower.y < minbb.y)
			minbb.y = currentBBLower.y;
		if (currentBBLower.z < minbb.z)
			minbb.z = currentBBLower.z;
	}
	/*
	 * now we have the min and max values, we should buid a Octree to
	 * contain all of them. I want the size to be a power of 2, and
	 * it should be a cube
	 */
	Vec3f lengths = maxbb - minbb;
	float sceneSize = std::max(lengths.x, std::max(lengths.y, lengths.z));
	int treeRootSize = pow(2, std::ceil(log(sceneSize) / log(2))); //this generates smallest power of 2 that is big equal to size

	//now we have the size and min will be floored, so we can calculate max
	float treeRootMaxX = treeRootSize + std::floor(minbb.x);
	float treeRootMaxY = treeRootSize + std::floor(minbb.y);
	float treeRootMaxZ = treeRootSize + std::floor(minbb.z);
	Vec3f treeMax(treeRootMaxX, treeRootMaxY, treeRootMaxZ);
	Vec3f treeMin(std::floor(minbb.x), std::floor(minbb.y),
			std::floor(minbb.z));
	//now request a octree with this dimentions.
	this->spatialTree = new Octree(NULL, treeMax, treeMin, primitives);
	std::cout << "spatial tree generated with dimentions: " << treeMax << ","
			<< treeMin << std::endl;
	//this->spatialTree->print();
}

bool Scene::renderScene() {
	if (this->camera == NULL) {
		std::cerr << "Can't render without a camera set." << std::endl;
	}
	unsigned int x = 0, y = 0;
	Vec3f color;
	bool morePixels;
	Ray ray[sampleRate];
#pragma omp parallel private(color,x,y,ray,morePixels)
	{
		morePixels = this->camera->getRays(x,y, sampleRate, ray);
		while (morePixels) {
			for(unsigned int i=0; i< sampleRate; ++i){
				color = rayTracer.trace(ray[i], *spatialTree, lights, this->maxDepth);
				this->film->setPixel(x,y,color);
			}
#pragma omp critical
			morePixels = this->camera->getRays(x,y, sampleRate, ray);
		}
	}
	return true;
}

bool Scene::addLight(float p1, float p2, float p3, float p4, float c1, float c2,
		float c3) {
	Vec4f lightPos(p1, p2, p3, p4);
	Vec3f lightColor(c1, c2, c3);
	lights.push_back(Light(lightPos,lightColor));

	lights.back().setAttenuation(this->currentAttenuation);

	std::cout << "add light with values (" << p1 << ", " << p2 << ", " << p3
			<< "," << p4 << ") (" << c1 << ", " << c2 << ", " << c3 << ")"
			<< std::endl;
	lightCount++;
	return true;
}

bool Scene::setMaxDepth(unsigned int depth) {
	this->maxDepth = depth;
	return 0;
}
