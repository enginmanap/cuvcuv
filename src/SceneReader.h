/*
 * SceneReader.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_SIMPLEFILEREADER_H_
#define SRC_SIMPLEFILEREADER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stack>
#include "Scene.h"
#include "Mat4f.h"
#include "Transform.h"
#include "FileReader.h"
#include "MaterialReader.h"

#define MAX_PARAMS 10

class SceneReader: public FileReader {
	Scene* scene;
public:
	SceneReader(std::string filename): FileReader(filename){this->scene = NULL;};
	/**
	 * A file can contain these:
	 * size width height
	 * maxdepth depth
	 * output filename
	 *
	 * camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy
	 *
	 * sphere x y z radius
	 *
	 * maxverts number -> defines maximum vertices that will be used for triangles
	 * maxvertnorms numver ->  defines maximum vertices with normals that will be used for triangles
	 * vertex x y z -> array will start with number 0
	 * vertexnormal x y z nx ny nz -> vertex with normal. array will start with number 0
	 * tri v1 v2 v3 -> a triangle using the vertex array. order is counter clockwise. Normal calculation needed.
	 * trinormal v1 v2 v3 -> a triangle with normal, using the vertex with normal array.
	 *
	 * translate x y z
	 * rotate x y z angle
	 * scale x y z
	 * pushTransform
	 * popTransform
	 *
	 * directional x y z r g b
	 * point x y z r g b
	 * attenuation const linear quadratic -> default 1,0,0
	 * ambient r g b -> default 0.2, 0.2, 0.2
	 *
	 * diffuse r g b
	 * specular r g b
	 * shininess s
	 * emission r g b
	 */
	Scene* readFile();

	~SceneReader();
};

#endif /* SRC_SIMPLEFILEREADER_H_ */
