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
	this->pixels = new Uint32[height * width];
	memset(this->pixels, 0, height * width * sizeof(Uint32));

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
	if (this->vertexArray != NULL){
		delete[] this->vertexArray;
	}
	delete[] pixels;
}

bool Scene::getSamplingSize(int& height, int& width) {
	height = sampler->getHeight();
	width = sampler->getWidht();
	return true;
}

bool Scene::setCurrentAmbient(float x, float y, float z) {
	currentAmbientLight.x = x;
	currentAmbientLight.y = y;
	currentAmbientLight.z = z;
	return true;
}

bool Scene::addVertex(float x, float y, float z) {
	if (currentVertex == maxVertexCount)
		return false;
	this->vertexArray[currentVertex] = Vec3f(x, y, z);
	currentVertex++;
	return true;
}

void Scene::printVertexes(){
	for (int i = 0; i < this->maxVertexCount; ++i) {
		std::cout << "vertex[" << i << "]=" << this->vertexArray[i]<< std::endl;
	}
}

bool Scene::addTriangle(int vertice1,int vertice2,int vertice3){
	Triangle tri(this->vertexArray[vertice1],this->vertexArray[vertice2],this->vertexArray[vertice3],currentAmbientLight);
	triangles.push_back(tri);
	triangleCount++;
	std::cout << "triangle added" << std::endl;
	return true;
}

bool Scene::addSphere(float x, float y, float z, float radius) {
	Sphere sphere(x, y, z, radius, currentAmbientLight);
	spheres.push_back(sphere);
	SphereCount++;
	return true;
}

void Scene::renderScene() {
	unsigned int x = 0, y = 0;
	while (this->sampler->getPoint(x, y)) {
		if(this->camera == NULL){
			std::cerr << "Can't render without a camera set." << std::endl;
		}
		Ray ray = this->camera->getRay(x, y);
		Vec3f color = rayTracer.trace(ray, spheres,triangles);
		color = colorRange * color;
		Uint32 color32 = (int) color.x << 16;
		color32 += (int) color.y << 8;
		color32 += (int) color.z;

		pixels[this->sampler->getWidht() * y + x] = color32;
	}
}

Uint32* Scene::getPixels(int& height, int& width) {
	height = sampler->getHeight();
	width = sampler->getWidht();
	return this->pixels;
}

