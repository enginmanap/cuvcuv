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

}

bool Scene::setCamera(float lookfromx, float lookfromy, float lookfromz,
		float lookatx, float lookaty, float lookatz, float upx, float upy,
		float upz, int fovy) {
	return false;
}

Scene::~Scene() {
	delete this->sampler;
}

bool Scene::getSamplingSize(int& height,int& width){
	height = sampler->getHeight();
	width = sampler->getWidht();
	return true;
}

