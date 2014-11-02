/*
 * Scene.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <iostream>
#include "Vec3f.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Camera.h"
#include "Sampler.h"
#include "Ray.h"
#include "RayTracer.h"
//SDL defines portable Uint32
#include "SDL2/SDL.h"

#define COLOR_DEPTH 8

class Scene {
	float colorRange;
	Vec3f currentAmbientLight;
	Camera *camera;
	Sampler *sampler;

	Uint32* pixels;

	short lightCount;
	//std::vector<Light> lightVector;
	int maxVertexCount, currentVertex;
	Vec3f* vertexArray;


	int triangleCount;
	std::vector<Triangle> triangles;
	int SphereCount;
	std::vector<Sphere> spheres;

	RayTracer rayTracer;

public:

	Scene(int, int);
	bool setCamera(float, float, float, float, float, float, float, float,
			float, float);

	virtual ~Scene();
	bool getSamplingSize(int&, int&);
	bool setCurrentAmbient(float, float, float);
	bool addSphere(float, float, float, float);

	Uint32* getPixels(int&, int&);
	void renderScene();
	bool createVertexSpace(int);
	bool addVertex(float,float,float);
	bool addTriangle(int,int,int);
	void printVertexes();
};

#endif /* SCENE_H_ */
