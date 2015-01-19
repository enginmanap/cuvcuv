/*
 * Scene.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "Scene.h"

int Scene::materialCount = 0;

/**
 * A scene starts definition starts with the sample size,
 * so we are setting height width, they will be passed to camera;
 */
Scene::Scene(unsigned int height, unsigned int width) :
		height(height), width(width), sampleRate(1), shadowGrid(1), lightCount(
				0), currentAttenuation(Vec3f(1, 0, 0)), maxVertexCount(2000), currentVertex(
				0), SphereCount(0), triangleCount(0), maxDepth(5) {
	this->camera = NULL;
	this->vertexVector.reserve(maxVertexCount);

	currentMaterial = new Material(DEFAULT_MATERIAL_NAME);
	materialMap[currentMaterial->getName()] = currentMaterial;
	materialNames.push(currentMaterial->getName());

	//mat->setShininess = 0.0f;
	//mat->setAttenuation = Vec3f(1, 0, 0); these are default

	this->saveFilename = "output.png";

	transformStack.push(Mat4f()); //since default constructor generates identity matrix.

	this->rayTracer = NULL;
	this->spatialTree = NULL;

	//TODO these are recreating if a setting found, there should be a better way
	film = new Film(height, width, COLOR_DEPTH, sampleRate);
	rayTracer = new RayTracer(1);

}

Scene::~Scene() {
	if (camera != NULL)
		delete camera;

	for (std::vector<Primitive*>::iterator it = primitives.begin();
			it != primitives.end(); ++it) {
		delete (*it);
	}

	materialMap.erase(currentMaterial->getName());
	delete currentMaterial;
	currentMaterial = NULL;

	std::map<std::string, Material*>::iterator itr = materialMap.begin();
	while (itr != materialMap.end()) {
		std::map<std::string, Material*>::iterator toErase = itr;
		++itr;
		delete toErase->second;
		materialMap.erase(toErase);
	}

	for (std::vector<Texture*>::iterator it = textures.begin();
			it != textures.end(); ++it) {
		delete (*it);
	}

	delete rayTracer;
	delete spatialTree;
	delete film;
}

void Scene::setShadowGrid(unsigned char shadowGrid) {
	this->shadowGrid = shadowGrid;
	if (rayTracer != NULL)
		delete rayTracer;

	rayTracer = new RayTracer(shadowGrid);
}

void Scene::setSampleRate(unsigned char samplingRate) {
	this->sampleRate = samplingRate;
	if (film != NULL)
		delete film;

	film = new Film(height, width, COLOR_DEPTH, sampleRate);
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
	outputStringStream << DEFAULT_MATERIAL_NAME << "_" << ++materialCount;
	currentMaterial = new Material(outputStringStream.str(),
			currentMaterial->getAmbient(), currentMaterial->getDiffuse(),
			currentMaterial->getSpecular(), currentMaterial->getEmission(),
			currentMaterial->getShininess(), currentMaterial->getDissolve(), currentMaterial->getRefractionIndex(), currentMaterial->getMapKd());
	materialMap[currentMaterial->getName()] = currentMaterial;

	//std::cout << "adding material with name" << currentMaterial->getName()<<std::endl;

	return true;
}

Mat4f Scene::popTransform() {
	if (transformStack.size() <= 1) {
		std::cerr << "no transform to pop" << std::endl;
		return Mat4f();
	}
	Mat4f temp = transformStack.top();
	transformStack.pop();

	//std::cout << "deleting material with name" << currentMaterial->getName() << std::endl;
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
			lookatz, upx, upy, upz, fovy, this->height, this->width);
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

bool Scene::addMaterial(std::vector<Material*>& materials) {
	for (unsigned int index = 0; index < materials.size(); ++index) {
		if (materialMap[materials[index]->getName()] != NULL) {
			//we had a material with same name
			std::cout << "double definition of material "
					<< materials[index]->getName() << ", last one will be kept"
					<< std::endl;
			delete materialMap[materials[index]->getName()];
			materialMap.erase(materials[index]->getName());
		}
		materialMap[materials[index]->getName()] = materials[index];
		currentMaterial = materials[index];
		std::cout << "new material " << materials[index]->getName() << " added."
				<< std::endl;
	}
	materialNames.push(currentMaterial->getName());
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
	if (currentVertex == maxVertexCount) {
		std::cerr
				<< "vertex vector resizing, using VertexCount command in scene definition can prevent this"
				<< std::endl;
		maxVertexCount = maxVertexCount * 2;
		vertexVector.resize(maxVertexCount);
	}
	this->vertexVector[currentVertex] = Vec3f(x, y, z);
	currentVertex++;
	return true;
}

void Scene::printVertexes() {
	for (int i = 0; i < this->currentVertex; ++i) {
		std::cout << "vertex[" << i << "]=" << this->vertexVector[i]
				<< std::endl;
	}
}

bool Scene::addTriangle(int vertice1, int vertice2, int vertice3) {
	if (vertice1 >= 0 && vertice2 >= 0 && vertice3 >= 0
			&& vertice1 < currentVertex && vertice2 < currentVertex
			&& vertice3 < currentVertex) {
		TriangleBase* triangle = new TriangleBase(this->vertexVector[vertice1],
				this->vertexVector[vertice2], this->vertexVector[vertice3],
				transformStack.top());
		triangle->setMaterial(currentMaterial);
		//std::cout << "new triangle with material: " << currentMaterial->getName() << std::endl;
		//triangle->setTransformation(transformStack.top());
		primitives.push_back(triangle);
		triangleCount++;
		return true;
	} else {
		std::cerr << "one of the vertices used is not defined ("
				<< currentVertex << ") ";
		if (vertice1 < 0)
			std::cerr << "vertex 1 " << vertice1;
		if (vertice2 < 0)
			std::cerr << "vertex 2 " << vertice2;
		if (vertice3 < 0)
			std::cerr << "vertex 3 " << vertice3;
		if (vertice1 >= currentVertex)
			std::cerr << " vertex 1 bigger " << vertice1;
		if (vertice2 >= currentVertex)
			std::cerr << " vertex 2 bigger " << vertice2;
		if (vertice3 >= currentVertex)
			std::cerr << " vertex 3 bigger " << vertice3;

		std::cerr << std::endl;
		return false;
	}
}

bool Scene::addSphere(float x, float y, float z, float radius) {
	Sphere* sphere = new Sphere(x, y, z, radius, transformStack.top());
	sphere->setMaterial(currentMaterial);
	primitives.push_back(sphere);
	//sphere->setTransformation(transformStack.top());
	SphereCount++;
	//std::cout << "add sphere " << sphere->id << " with the values (" << x
	//		<< ", " << y << ", " << z << ") and radius: " << radius
	//		<< std::endl;
	return true;
}

bool Scene::addModel(Model* model) {
	primitives.push_back(model);
	//TODO do we need to add a modelCount variable?
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
			maxbb.y = currentBBUpper.y;
		if (currentBBUpper.z > maxbb.z)
			maxbb.z = currentBBUpper.z;

		if (currentBBLower.x < minbb.x)
			minbb.x = currentBBLower.x;
		if (currentBBLower.y < minbb.y)
			minbb.y = currentBBLower.y;
		if (currentBBLower.z < minbb.z)
			minbb.z = currentBBLower.z;
	}
	//The octree root node size will be equal to the scene size
	this->spatialTree = new Octree(NULL, maxbb, minbb, primitives, 10);	//TODO 10 is hardcoded max depth
	std::cout << "spatial tree generated with dimentions: " << maxbb << ","
			<< minbb << std::endl;
	//this->spatialTree->print();
}

bool Scene::renderScene() {
	if (this->camera == NULL) {
		std::cerr << "Can't render without a camera set." << std::endl;
	}
	unsigned int x = 0, y = 0;
	Vec3f color;
	bool morePixels;
	std::vector<Ray> rays;
#pragma omp parallel private(color,x,y,rays,morePixels)
	{
		morePixels = this->camera->getRays(x, y, sampleRate, rays);
		while (morePixels) {
			for (unsigned int i = 0; i < rays.size(); ++i) {
				color = rayTracer->trace(rays[i], *spatialTree, lights,
						this->maxDepth);
				this->film->setPixel(x, y, color);
			}
#pragma omp critical
			morePixels = this->camera->getRays(x, y, sampleRate, rays);
		}
	}
	return true;
}

bool Scene::addLight(float p1, float p2, float p3, float p4, float size, float c1, float c2,
		float c3) {
	Vec4f lightPos(p1, p2, p3, p4);
	Vec3f lightColor(c1, c2, c3);
	lights.push_back(Light(lightPos, size, lightColor));

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
