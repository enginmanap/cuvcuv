/*
 * Scene.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Scene.h"

/**
 * A scene starts definition starts with the sample size,
 * so we are setting the sampler in constructor;
 */
Scene::Scene(int height, int width) {
	this->sampler = new Sampler(height, width);
	this->camera = NULL;
	this->vertexArray = NULL;
	this->maxVertexCount = 0;
	this->currentVertex = 0;
	this->SphereCount = 0;
	this->triangleCount = 0;
	this->lightCount = 0;
	this->colorRange = pow(2, COLOR_DEPTH) - 1; //for 8 bits, this means 255
	this->pixels = new unsigned char[height * width * COLOR_DEPTH];
	memset(this->pixels, 255, height * width * COLOR_DEPTH); //standart says char is 1 byte

	this->currentShininess = 0.0f;
	this->currentAttenuation = Vec3f(1, 0, 0);

	this->saveFilename = "output.png";

	transformStack.push(Mat4f()); //since default constructor generates identity matrix.

	this->maxDepth = 5;

	this->spatialTree = NULL;

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
	return true;
}

Mat4f Scene::popTransform() {
	if (transformStack.size() <= 1) {
		std::cerr << "no transform to pop" << std::endl;
		return Mat4f();
	}
	Mat4f temp = transformStack.top();
	transformStack.pop();
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
			lookatz, upx, upy, upz, fovy, sampler->getHeight(),
			sampler->getWidht());
	return true;
}

bool Scene::createVertexSpace(int maxVertexCount) {
	if (this->vertexArray != NULL) {
		delete vertexArray;
	}
	this->maxVertexCount = maxVertexCount;
	vertexArray = new Vec3f[this->maxVertexCount];
	//memset(this->vertexArray, 0, this->maxVertexCount * sizeof(Vec3f)); //this does not work, because somehow Vec3f is 16 bytes, instead of 12.

	currentVertex = 0;
	return true;
}

Scene::~Scene() {
	delete this->sampler;
	if (camera != NULL)
		delete camera;
	if (this->vertexArray != NULL) {
		delete[] this->vertexArray;
	}
	for (std::vector<Primitive*>::iterator it = primitives.begin();
			it != primitives.end(); ++it) {
		delete (*it);
	}
	delete[] pixels;

	delete spatialTree;
}

bool Scene::getSamplingSize(int& height, int& width) {
	height = sampler->getHeight();
	width = sampler->getWidht();
	return true;
}

bool Scene::setCurrentEmission(float x, float y, float z) {
	currentEmissionLight.x = x;
	currentEmissionLight.y = y;
	currentEmissionLight.z = z;
	return true;
}

bool Scene::setCurrentAmbient(float x, float y, float z) {
	currentAmbientLight.x = x;
	currentAmbientLight.y = y;
	currentAmbientLight.z = z;
	return true;
}

bool Scene::setCurrentDiffuse(float x, float y, float z) {
	this->currentDiffuse.x = x;
	this->currentDiffuse.y = y;
	this->currentDiffuse.z = z;
	return true;
}

bool Scene::setCurrentSpecular(float x, float y, float z) {
	this->currentSpecular.x = x;
	this->currentSpecular.y = y;
	this->currentSpecular.z = z;
	return true;
}

bool Scene::setCurrentShininess(float shininess) {
	this->currentShininess = shininess;
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
	this->vertexArray[currentVertex] = Vec3f(x, y, z);
	currentVertex++;
	return true;
}

void Scene::printVertexes() {
	for (int i = 0; i < this->maxVertexCount; ++i) {
		std::cout << "vertex[" << i << "]=" << this->vertexArray[i]
				<< std::endl;
	}
}

bool Scene::addTriangle(int vertice1, int vertice2, int vertice3) {
	if (vertice1 >= 0 && vertice2 >= 0 && vertice3 >= 0
			&& vertice1 < currentVertex && vertice2 < currentVertex
			&& vertice3 < currentVertex) {
		Triangle* triangle = new Triangle(this->vertexArray[vertice1],
				this->vertexArray[vertice2], this->vertexArray[vertice3],transformStack.top());
		triangle->setLightValues(currentAmbientLight, currentEmissionLight,
				currentDiffuse, currentSpecular, currentShininess);
		//triangle->setTransformation(transformStack.top());
		primitives.push_back(triangle);
		triangleCount++;
		return true;
	} else {
		std::cerr << "one of the vertices used is not defined";
		return false;
	}
}

bool Scene::addSphere(float x, float y, float z, float radius) {
	Sphere* sphere = new Sphere(x, y, z, radius,transformStack.top());
	sphere->setLightValues(currentAmbientLight, currentEmissionLight,
			currentDiffuse, currentSpecular, currentShininess);
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
	this->spatialTree = new Octree(NULL, treeMax, treeMin, primitives); //FIXME this values should be determined by the objects.
	std::cout << "spatial tree generated with dimentions: " << treeMax << ","
			<< treeMin << std::endl;
	//this->spatialTree->print();
}

bool Scene::renderScene() {
	unsigned int x = 0, y = 0;
	Vec3f color;
	bool morePixels;
	Ray ray;

#pragma omp parallel private(color,x,y,ray) shared (morePixels)
	{
#pragma omp critical
		morePixels = this->sampler->getPoint(x, y);
		while (morePixels) {
			if (this->camera == NULL) {
				std::cerr << "Can't render without a camera set." << std::endl;
			}
			ray = this->camera->getRay(x, y);
			color = rayTracer.trace(ray, *spatialTree, lights, this->maxDepth);
			color = colorRange * color;
			unsigned int index = 4 * (this->sampler->getWidht() * y + x);
#ifdef USE_FREEIMAGE_PNG
			pixels[index + 0] = (unsigned char) color.z;
			pixels[index + 1] = (unsigned char) color.y;
			pixels[index + 2] = (unsigned char) color.x;
			pixels[index + 3] = 255;
#else
			pixels[index + 0] = (unsigned char) color.x;
			pixels[index + 1] = (unsigned char) color.y;
			pixels[index + 2] = (unsigned char) color.z;
			pixels[index + 3] = 255;
#endif /*USE_FREEIMAGE_PNG*/
#pragma omp critical
			morePixels = this->sampler->getPoint(x, y);
		}
	}
	return true;
}

unsigned char* Scene::getPixels(int& height, int& width) {
	height = sampler->getHeight();
	width = sampler->getWidht();
	return this->pixels;
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
