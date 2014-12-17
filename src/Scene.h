/*
 * Scene.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <string>
#include "Vec3f.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
#include "Ray.h"
#include "RayTracer.h"
#include "Light.h"
#include "Octree.h"
#include "Film.h"
#include "Material.h"


#ifndef COLOR_DEPTH
#define COLOR_DEPTH 8
#endif

class Scene {
	unsigned int height, width;

	Film* film;
	unsigned char sampleRate;

	std::map<std::string,Material*> materialMap;
	std::stack<std::string> materialNames;//this is necessery because no name materials are possible;
	static int materialCount;
	Material* currentMaterial;//this will change with push/pop of materialNames

	std::string saveFilename;

	Camera *camera;


	short lightCount;
	std::vector<Light> lights;
	Vec3f currentAttenuation;

	int maxVertexCount, currentVertex;
	Vec3f* vertexArray;

	int SphereCount;
	int triangleCount;
	std::vector<Primitive*> primitives;
	std::stack<Mat4f> transformStack;

	unsigned int maxDepth;
	RayTracer rayTracer;
	Octree *spatialTree;

	Vec3f temproryVector; //this is used in order not to create/destroy all elements


public:

	Scene(unsigned int, unsigned int);
	bool setCamera(float, float, float, float, float, float, float, float,
			float, float);

	virtual ~Scene();

	bool getSamplingSize(unsigned int&, unsigned int&);
	void setSampleRate(unsigned char samplingRate);

	bool setCurrentAmbient(float, float, float);
	bool setCurrentEmission(float, float, float);
	bool setCurrentDiffuse(float, float, float);
	bool setCurrentSpecular(float, float, float);
	bool setCurrentShininess(float);
	bool setCurrentAttenuation(float, float, float);

	bool setSaveFilename(std::string);
	std::string getSaveFilename();
	bool setMaxDepth(unsigned int);

	bool createVertexSpace(int);
	void printVertexes();

	unsigned char* getPixels(unsigned int height, unsigned int width) {return this->film->getPixels(height,width);}

	bool addVertex(float, float, float);
	bool addTriangle(int, int, int);
	bool addSphere(float, float, float, float);
	bool addLight(float, float, float, float, float, float, float);


	bool pushTransform();
	Mat4f popTransform();
	Mat4f addTransform(Mat4f&);

	void buildOctree();
	bool renderScene();
};

#endif /* SCENE_H_ */
