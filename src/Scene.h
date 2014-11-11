/*
 * Scene.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <stack>
#include <iostream>
#include "Vec3f.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
#include "Sampler.h"
#include "Ray.h"
#include "RayTracer.h"
#include "Light.h"
#include <string>
//SDL defines portable Uint32
#include "SDL2/SDL.h"

#define COLOR_DEPTH 8

class Scene {

	Vec3f currentAmbientLight;
	Vec3f currentEmissionLight;
	Vec3f currentDiffuse;
	Vec3f currentSpecular;
	float currentShininess;
	Vec3f currentAttenuation;
	std::string saveFilename;

	Camera *camera;
	Sampler *sampler;

	float colorRange;
	Uint32* pixels;

	short lightCount;
	std::vector<Light> lights;
	int maxVertexCount, currentVertex;
	Vec3f* vertexArray;

	int SphereCount;
	int triangleCount;
	std::vector<Primitive*> primitives;
	std::stack<Mat4f> transformStack;

	unsigned int maxDepth;
	RayTracer rayTracer;

public:

	Scene(int, int);
	bool setCamera(float, float, float, float, float, float, float, float,
			float, float);

	virtual ~Scene();
	bool getSamplingSize(int&, int&);
	bool setCurrentAmbient(float, float, float);
	bool setCurrentEmission(float, float, float);
	bool setCurrentDiffuse(float, float, float);
	bool setCurrentSpecular(float, float, float);
	bool setCurrentShininess(float);
	bool setCurrentAttenuation(float, float, float);

	bool setSaveFilename(std::string);
	std::string getSaveFilename();
	bool setMaxDepth(unsigned int);

	Uint32* getPixels(int&, int&);

	bool createVertexSpace(int);
	bool addVertex(float, float, float);
	bool addTriangle(int, int, int);
	bool addSphere(float, float, float, float);
	bool addLight(float, float, float, float, float, float, float);
	void printVertexes();

	bool pushTransform();
	Mat4f popTransform();
	Mat4f addTransform(Mat4f&);

	void renderScene();
};

#endif /* SCENE_H_ */
