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

	this->currentShininess = 0.0f;

	this->saveFilename = "output.png";

	transformStack.push(Mat4f());//since default constructor generates identity matrix.

}

bool Scene::setSaveFilename(std::string filename){
	this->saveFilename = filename;
	return true;
}

std::string Scene::getSaveFilename(){
	return saveFilename;
}

bool Scene::pushTransform(){
	transformStack.push(transformStack.top());
	return true;
}

Mat4f Scene::popTransform(){
	if( transformStack.size() <= 1){
		std::cerr << "no transform to pop" << std::endl;
		return Mat4f();
	}
	Mat4f temp = transformStack.top();
	transformStack.pop();
	return temp;
}

Mat4f Scene::addTransform(Mat4f& transform){
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
			it != primitives.end(); it++) {
		delete (*it);
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
				this->vertexArray[vertice2], this->vertexArray[vertice3]);
		triangle->setLightValues(currentAmbientLight, currentDiffuse,
				currentSpecular, currentShininess);
		triangle->setTransformation(transformStack.top());
		primitives.push_back(triangle);
		triangleCount++;
		return true;
	} else {
		std::cerr << "one of the vertices used is not defined";
		return false;
	}
}

bool Scene::addSphere(float x, float y, float z, float radius) {
	std::cout << "add sphere with the values (" << x << ", " << z << ", " << z << ") and radius: " << radius << std::endl;
	Sphere* sphere = new Sphere(x, y, z, radius);
	sphere->setLightValues(currentAmbientLight, currentDiffuse, currentSpecular,
			currentShininess);
	primitives.push_back(sphere);
	sphere->setTransformation(transformStack.top());
	SphereCount++;
	return true;
}

void Scene::renderScene() {
	unsigned int x = 0, y = 0;
	while (this->sampler->getPoint(x, y)) {
		if (this->camera == NULL) {
			std::cerr << "Can't render without a camera set." << std::endl;
		}
		Ray ray = this->camera->getRay(x, y);
		Vec3f color = rayTracer.trace(ray, primitives, lights);
		color = colorRange * color;
		Uint32 color32 = (int) color.x << 16;
		color32 += (int) color.y << 8;
		color32 += (int) color.z;
		color32 = color32 | 0xFF000000;

		pixels[this->sampler->getWidht() * y + x] = color32;
	}
}

Uint32* Scene::getPixels(int& height, int& width) {
	height = sampler->getHeight();
	width = sampler->getWidht();
	return this->pixels;
}

bool Scene::addLight(float p1,float p2 ,float p3 ,float p4,float c1,float c2 ,float c3){
	lights.push_back(Light(Vec4f(p1,p2,p3,p4), Vec3f(c1,c2,c3)));
	std::cout << "add color with values (" << p1 <<", " << p2 << ", " << p3 << "," << p4 << ") (" << c1 << ", " << c2 << ", " << c3 << ")" << std::endl;
	lightCount++;
	return true;
}


