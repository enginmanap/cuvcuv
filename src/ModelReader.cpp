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
	int parameterCount;

	std::string line;

	Mat4f temproryMatrix;

	while (readLine(line)) {
		std::stringstream stringStream(line);
		stringStream >> command;
		if (command == "VertexCount") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				model->createVertexSpace(parameters[0]);
			}
		} else if (command == "v") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "vertex does not contain 3 coordinates, only " << parameterCount << " provided." << std::endl;
				} else {
					model->addVertex(parameters[0], parameters[1], parameters[2]);
				}
			}
		} else if (command == "f") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "face does not contain 3 vertices, only " << parameterCount << " provided." << std::endl;
				} else {
					model->addTriangle((int) parameters[0], (int) parameters[1],
							(int) parameters[2]);

					//there might be more faces, we will interpret as GL_TRIANGLE_FAN
					float fanCenter=parameters[0], previousVertex=parameters[2];
					for(int i=3; i < parameterCount; ++i) {
						model->addTriangle((int) fanCenter, previousVertex,
								(int) parameters[i]);
						previousVertex = parameters[i];
					}
				}
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
