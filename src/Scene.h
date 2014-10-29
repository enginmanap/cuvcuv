/*
 * Scene.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include "sphere.h"
#include "Camera.h"
#include "Sampler.h"
#include <iostream>

class Scene {

	Camera *camera;
	Sampler *sampler;

	short lightCount;
	//std::vector<Light> lightVector;
	int vertexCount;
	//std::vector<Vec3> vectors;

	int ObjectCount;
	//std::vector<Triangle>
	std::vector<Sphere> spheres;

public:
	Scene(int, int);
	bool setCamera(float, float, float, float, float, float, float, float,
			float, float);
	bool addSphere(float, float, float, float);
	virtual ~Scene();
	bool getSamplingSize(int&, int&);

};

#endif /* SCENE_H_ */
