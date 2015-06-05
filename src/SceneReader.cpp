/*
 * SceneReader.cpp
 *
 *  Created on: 12 Ara 2014
 *      Author: Engin Manap
 */

#include "SceneReader.h"

Scene* SceneReader::readFile() {

	bool isNameSet = false;
	std::string command;
	double parameters[MAX_PARAMS];
	int parameterCount;

	std::string stringParams[MAX_PARAMS];
	std::string firstLine;
	if (!readLine(firstLine)) {
		std::cerr << "the file has no element to read" << std::endl;
		exit(1);
	} else {
		std::stringstream stringStream(firstLine);
		stringStream >> command;
		if (command != "size") {
			std::cerr << "First command has to be \"size\"" << std::endl;
			exit(1);
		} else {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 2) {
					std::cerr << "size does not contain 2 integers, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene = new Scene(parameters[1], parameters[0]);
				}
			} else {
				std::cerr << "size command parameters could not be read"
						<< std::endl;
				exit(1);
			}

		}

	}
	//Now we have the scene, we can load camera, light etc. out of order;
	std::string line;

	Mat4f temproryMatrix;

	while (readLine(line)) {
		std::stringstream stringStream(line);
		stringStream >> command;
		if (command == "camera") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 10) {
					std::cerr << "camera does not contain 10 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->setCamera(parameters[0], parameters[1],
							parameters[2], parameters[3], parameters[4],
							parameters[5], parameters[6], parameters[7],
							parameters[8], parameters[9]);
				}
			}
		} else if (command == "output") {
			std::string outputFile;
			stringStream >> outputFile;
			scene->setSaveFilename(outputFile);
			isNameSet = true;
		} else if (command == "Ka") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "Ka does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->setCurrentAmbient(parameters[0], parameters[1],
							parameters[2]);
				}
			}
		} else if (command == "Ke") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "Ke does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->setCurrentEmission(parameters[0], parameters[1],
							parameters[2]);
				}
			}
		} else if (command == "Kd") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "Kd does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->setCurrentDiffuse(parameters[0], parameters[1],
							parameters[2]);
				}
			}
		} else if (command == "Ks") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "Ks does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->setCurrentSpecular(parameters[0], parameters[1],
							parameters[2]);
				}
			}
		} else if (command == "Ns") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				scene->setCurrentShininess(parameters[0]);
			}
		} else if (command == "d" || command == "Tr") {
			if (readFloatParams(stringStream, parameters,
					parameterCount)) {
				scene->setCurrentDissolve(parameters[0]);
			} else {
				std::cerr << "Tr/d command parameter could not be read"
						<< std::endl;
			}

		} else if (command == "Ni") {
			if (readFloatParams(stringStream, parameters,
					parameterCount)) {
				scene->setCurrentRefractionIndex(parameters[0]);
			} else {
				std::cerr << "Ni command parameter could not be read"
						<< std::endl;
			}

		} else if (command == "point") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 6) {
					std::cerr << "point light does not contain 6 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					//notice the 1 as 4th param it means this is point light
					scene->addLight(parameters[0], parameters[1], parameters[2],
							1,0, parameters[3], parameters[4], parameters[5]);
				}
			}
		} else if (command == "lamp") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 7) {
					std::cerr << "lamp light does not contain 7 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					//this is a point light (4th param 1) and has size (5th param is 4th read)
					scene->addLight(parameters[0], parameters[1], parameters[2],
							1, parameters[3], parameters[4], parameters[5], parameters[6]);
				}
			}
		} else if (command == "directional") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 6) {
					std::cerr
							<< "directional light does not contain 6 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					//notice the 0 as 4th param, it means light has no position only direction
					scene->addLight(parameters[0], parameters[1], parameters[2],
							0,0, parameters[3], parameters[4], parameters[5]);
				}
			}
		} else if (command == "attenuation") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "attenuation does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->setCurrentAttenuation(parameters[0], parameters[1],
							parameters[2]);
				}
			}
		} else if (command == "sphere") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 4) {
					std::cerr
							<< "sphere definition does not contain 4 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->addSphere(parameters[0], parameters[1],
							parameters[2], parameters[3]);
				}
			}
		} else if (command == "VertexCount") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				scene->createVertexSpace(parameters[0]);
			}
		} else if (command == "v") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr
							<< "vertex definition does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->addVertex(parameters[0], parameters[1],
							parameters[2]);
				}
			}
		} else if (command == "f") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "face does not contain 3 vertices, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					scene->addTriangle((int) parameters[0], (int) parameters[1],
							(int) parameters[2]);
					double fanCenter = parameters[0], previousVertex =
							parameters[2];
					for (int i = 3; i < parameterCount; ++i) {
						scene->addTriangle((int) fanCenter, previousVertex,
								(int) parameters[i]);
						previousVertex = parameters[i];
					}
				}
			}
		} else if (command == "translate") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "translate does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					temproryMatrix = Transform::translate(parameters[0],
							parameters[1], parameters[2]);
					scene->addTransform(temproryMatrix);
				}
			}
		} else if (command == "scale") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 3) {
					std::cerr << "scale does not contain 3 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					temproryMatrix = Transform::scale(parameters[0],
							parameters[1], parameters[2]);
					scene->addTransform(temproryMatrix);
				}
			}
		} else if (command == "rotate") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if (parameterCount < 4) {
					std::cerr << "rotate does not contain 4 doubles, only "
							<< parameterCount << " provided." << std::endl;
				} else {
					temproryMatrix = Transform::rotate(parameters[0],
							parameters[1], parameters[2], parameters[3]);
					scene->addTransform(temproryMatrix);
				}
			}
		} else if (command == "pushTransform") {
			scene->pushTransform();
		} else if (command == "popTransform") {
			scene->popTransform();
		} else if (command == "maxdepth") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				scene->setMaxDepth((unsigned int) parameters[0]);
			}
		} else if (command == "sampleRate") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				scene->setSampleRate((unsigned char) parameters[0]);
			}
		} else if (command == "shadowGrid") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				scene->setShadowGrid((unsigned char) parameters[0]);
			}
		} else if (command == "mtllib") {
			if (readStringParams(stringStream, stringParams, 1)) {
				//create a material reader and read material lib
				MaterialReader materialReader(filePath, stringParams[0]);

				std::vector<Texture*> textureVector;
				std::vector<Material*> materials =
						materialReader.readMaterialFile(textureVector);
				scene->addMaterial(materials); //clearing the material is going to be done by scene
				if (!textureVector.empty()) {
					scene->addTexture(textureVector);
				}
			}
		} else if (command == "loadModel") {
			if (readStringParams(stringStream, stringParams, 1)) {
				//create a ModelReader and load any Materials and load 1 model
				std::string fileToread = stringParams[0];
				std::string objPath;

				unsigned int endPos = fileToread.find_last_of('/');
				if (endPos == std::string::npos) {
					objPath = "";
				} else {
					objPath = fileToread.substr(0, endPos + 1); //+1 for including seperator in the end
					fileToread = fileToread.substr(endPos + 1);
				}
				//since the filePath already has /
				objPath = filePath + objPath;
				ModelReader modelReader(objPath, fileToread);
				modelReader.readModelFile(*scene);
			}
		} else
			std::cerr << "command unknown: \"" << command << "\"" << std::endl;

	}

	//If no name was set during parsing, name the scene same as input.
	if (!isNameSet) {
		std::string outputName = this->fileName + ".png";
		scene->setSaveFilename(outputName);
	}
	return scene;
}

SceneReader::~SceneReader() {
	if (scene != NULL)
		delete scene;
}

