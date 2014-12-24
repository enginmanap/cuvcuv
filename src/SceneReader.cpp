/*
 * SceneReader.cpp
 *
 *  Created on: 12 Ara 2014
 *      Author: Engin Manap
 */

#include "SceneReader.h"

Scene* SceneReader::readFile() {
	std::string command;
	float parameters[MAX_PARAMS];
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
			if (readFloatParams(stringStream, parameters, 2)) {
				scene = new Scene(parameters[1], parameters[0]);
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
			if (readFloatParams(stringStream, parameters, 10)) {
				scene->setCamera(parameters[0], parameters[1], parameters[2],
						parameters[3], parameters[4], parameters[5],
						parameters[6], parameters[7], parameters[8],
						parameters[9]);
			}
		} else if (command == "output") {
			std::string outputFile;
			stringStream >> outputFile;
			scene->setSaveFilename(outputFile);
		} else if (command == "ambient") {
			if (readFloatParams(stringStream, parameters, 3)) {
				scene->setCurrentAmbient(parameters[0], parameters[1],
						parameters[2]);
			}
		} else if (command == "emission") {
			if (readFloatParams(stringStream, parameters, 3)) {
				scene->setCurrentEmission(parameters[0], parameters[1],
						parameters[2]);
			}
		} else if (command == "diffuse") {
			if (readFloatParams(stringStream, parameters, 3)) {
				scene->setCurrentDiffuse(parameters[0], parameters[1],
						parameters[2]);
			}
		} else if (command == "specular") {
			if (readFloatParams(stringStream, parameters, 3)) {
				scene->setCurrentSpecular(parameters[0], parameters[1],
						parameters[2]);
			}
		} else if (command == "shininess") {
			if (readFloatParams(stringStream, parameters, 1)) {
				scene->setCurrentShininess(parameters[0]);
			}
		} else if (command == "point") {
			if (readFloatParams(stringStream, parameters, 6)) {
				//notice the 1 as 4th param
				scene->addLight(parameters[0], parameters[1], parameters[2], 1,
						parameters[3], parameters[4], parameters[5]);
			}
		} else if (command == "directional") {
			if (readFloatParams(stringStream, parameters, 6)) {
				//notice the 0 as 4th param, it means light has no position only direction
				scene->addLight(parameters[0], parameters[1], parameters[2], 0,
						parameters[3], parameters[4], parameters[5]);
			}
		} else if (command == "attenuation") {
			if (readFloatParams(stringStream, parameters, 3)) {
				scene->setCurrentAttenuation(parameters[0], parameters[1],
						parameters[2]);
			}
		} else if (command == "sphere") {
			if (readFloatParams(stringStream, parameters, 4)) {
				scene->addSphere(parameters[0], parameters[1], parameters[2],
						parameters[3]);
			}
		} else if (command == "maxverts") {
			if (readFloatParams(stringStream, parameters, 1)) {
				scene->createVertexSpace(parameters[0]);
			}
		} else if (command == "vertex") {
			if (readFloatParams(stringStream, parameters, 3)) {
				scene->addVertex(parameters[0], parameters[1], parameters[2]);
			}
		} else if (command == "tri") {
			if (readFloatParams(stringStream, parameters, 3)) {
				scene->addTriangle((int) parameters[0], (int) parameters[1],
						(int) parameters[2]);
			}
		} else if (command == "translate") {
			if (readFloatParams(stringStream, parameters, 3)) {
				temproryMatrix = Transform::translate(parameters[0],
						parameters[1], parameters[2]);
				scene->addTransform(temproryMatrix);
			}
		} else if (command == "scale") {
			if (readFloatParams(stringStream, parameters, 3)) {
				temproryMatrix = Transform::scale(parameters[0], parameters[1],
						parameters[2]);
				scene->addTransform(temproryMatrix);
			}
		} else if (command == "rotate") {
			if (readFloatParams(stringStream, parameters, 4)) {
				temproryMatrix = Transform::rotate(parameters[0], parameters[1],
						parameters[2], parameters[3]);
				scene->addTransform(temproryMatrix);
			}
		} else if (command == "pushTransform") {
			scene->pushTransform();
		} else if (command == "popTransform") {
			scene->popTransform();
		} else if (command == "maxdepth") {
			if (readFloatParams(stringStream, parameters, 1)) {
				scene->setMaxDepth((unsigned int) parameters[0]);
			}
		} else if (command == "sampleRate") {
			if (readFloatParams(stringStream, parameters, 1)) {
				scene->setSampleRate((unsigned char) parameters[0]);
			}
		} else
			std::cerr << "command unknown: \"" << command << "\"" << std::endl;

	}

	return scene;
}

