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
#include "Camera.h"
#include "Sampler.h"
#include "Ray.h"
#include "RayTracer.h"

class Scene {
	Vec3f currentAmbientLight;
	Camera *camera;
	Sampler *sampler;

	short lightCount;
	//std::vector<Light> lightVector;
	int vertexCount;
	//std::vector<Vec3> vectors;

	int ObjectCount;
	//std::vector<Triangle>
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

	void renderScene();
};

#endif /* SCENE_H_ */
