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
	this->vertexCount = 0;
	this->ObjectCount = 0;
	this->lightCount = 0;

	this->pixels = new Uint32[height * width];
	memset(this->pixels, 255, height * width * sizeof(Uint32));

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

Scene::~Scene() {
	delete this->sampler;
	if (camera != NULL)
		delete camera;
	delete pixels;
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

bool Scene::addSphere(float x, float y, float z, float radius) {
	Sphere sphere(x, y, z, radius, currentAmbientLight);
	spheres.push_back(sphere);
	ObjectCount++;
	return true;
}

void Scene::renderScene() {
	unsigned int x = 0, y = 0;
	while (this->sampler->getPoint(x, y)) {
		Ray ray = this->camera->getRay(x, y);
		Vec3f color = rayTracer.trace(ray, spheres);
		Uint32 color32 = (int)color.x << 16;
		color32 += (int)color.y << 8;
		color32 += (int)color.z;

		pixels[this->sampler->getHeight() * y + x ] = color32;
	}
}

Uint32* Scene::getPixels(int& height, int& width) {
	height = sampler->getHeight();
	width = sampler->getWidht();
	return this->pixels;
}

