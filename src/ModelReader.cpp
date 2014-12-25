/*
 * ModelReader.cpp
 *
 *  Created on: 25 Ara 2014
 *      Author: engin
 */

#include "ModelReader.h"

Model* ModelReader::readModelFile(Scene& scene) {
	model = new Model(scene.getTransform());
	model->setMaterial(scene.getMaterial());
	std::string command;
	float parameters[MAX_PARAMS];
	std::string stringParams[MAX_PARAMS];

	std::string line;

	Mat4f temproryMatrix;

	while (readLine(line)) {
		std::stringstream stringStream(line);
		stringStream >> command;
		if (command == "VertexCount") {
			if (readFloatParams(stringStream, parameters, 1)) {
				model->createVertexSpace(parameters[0]);
			}
		} else if (command == "v") {
			if (readFloatParams(stringStream, parameters, 3)) {
				model->addVertex(parameters[0], parameters[1], parameters[2]);
			}
		} else if (command == "f") {
			if (readFloatParams(stringStream, parameters, 3)) {
				model->addTriangle((int) parameters[0], (int) parameters[1],
						(int) parameters[2]);
			}
		} else if (command == "mtllib") {
			if (readStringParams(stringStream, stringParams, 1)) {
				//create a material reader and read material lib
				MaterialReader materialReader(stringParams[0]);
				Material* mat = materialReader.readMaterialFile();
				scene.addMaterial(mat); //clearing the material is going to be done by scene
				model->setMaterial(mat);
			}
		} else if (command == "usemtl") {
			if (readStringParams(stringStream, stringParams, 1)) {
				Material* mat = scene.getMaterial(stringParams[0]);
				if(mat!=NULL)
					model->setMaterial(mat);
			}
		} else
			std::cerr << "command unknown: \"" << command << "\"" << std::endl;

	}
	model->buildOctree();
	scene.addModel(model);
	return model;
}
