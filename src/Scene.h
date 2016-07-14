/*
 * Scene.h
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
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
#include "TriangleBase.h"
#include "Camera.h"
#include "Ray.h"
#include "RayTracer.h"
#include "Light.h"
#include "Octree.h"
#include "Film.h"
#include "Material.h"
#include "Model.h"

#ifndef COLOR_DEPTH
#define COLOR_DEPTH 8
#endif

class Scene {
	unsigned int height, width;

	Film* film;
	unsigned char sampleRate, shadowGrid;
	double aperture;
	std::map<std::string, Material*> materialMap;
	std::stack<std::string> materialNames; //this is necessery because we need to keep the name of material before pushTransform
	static int materialCount;
	Material* currentMaterial; //this will change with push/pop of materialNames

	std::vector<Texture*> textures;

	std::string saveFilename;

	Camera *camera;

	short lightCount;
	std::vector<Light> lights;
	Vec3f currentAttenuation;

	int maxVertexCount, currentVertex;
	std::vector<Vec3f> vertexVector;

	int SphereCount;
	int triangleCount;
	std::vector<Primitive*> primitives;
	std::stack<Mat4f> transformStack;

	unsigned int maxDepth;
	RayTracer* rayTracer;
	Octree *spatialTree;

	Vec3f temproryVector; //this is used in order not to create/destroy all elements
	
	unsigned int DOFRate;
public:

	Scene(unsigned int, unsigned int);
	bool setCamera(double, double, double, double, double, double, double, double,
			double, double);

	virtual ~Scene();

	bool getSamplingSize(unsigned int&, unsigned int&);
	void setShadowGrid(unsigned char);
	void setSampleRate(unsigned char samplingRate);
	void setDOFRate(unsigned char);
	void setAperture(double);

	bool setCurrentAmbient(double, double, double);
	bool setCurrentEmission(double, double, double);
	bool setCurrentDiffuse(double, double, double);
	bool setCurrentSpecular(double, double, double);
	bool setCurrentShininess(double);
	void setCurrentDissolve(double dissolve){currentMaterial->setDissolve(dissolve);}
	void setCurrentRefractionIndex(double RefractionIndex){currentMaterial->setRefractionIndex(RefractionIndex);}

	bool addMaterial(std::vector<Material*>&);
	void addTexture(std::vector<Texture*>& newTextures) {
		textures.insert(textures.end(), newTextures.begin(), newTextures.end());
	}
	Material* getMaterial(std::string& materialName) {
		return materialMap[materialName];
	}
	Material* getMaterial() {
		return currentMaterial;
	}

	bool setCurrentAttenuation(double, double, double);

	bool setSaveFilename(std::string);
	std::string getSaveFilename();
	bool setMaxDepth(unsigned int);

	bool createVertexSpace(int);
	void printVertexes();

	unsigned char* getPixels(unsigned int height, unsigned int width) {
		return this->film->getPixels(height, width);
	}

	bool addVertex(double, double, double);
	bool addTriangle(int, int, int);
	bool addSphere(double, double, double, double);
	bool addModel(Model*);

	bool addLight(double, double, double, double, double, double, double, double);

	bool pushTransform();
	Mat4f popTransform();
	Mat4f addTransform(Mat4f&);
	/**
	 * returns the current transform
	 */
	Mat4f getTransform() {
		return transformStack.top();
	}

	void buildOctree();
	bool renderScene();
};

#endif /* SCENE_H_ */
