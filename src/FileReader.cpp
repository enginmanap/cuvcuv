/*
 * FileReader.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "FileReader.h"

FileReader::FileReader(std::string fileName) {
	this->fileName = fileName;
	this->scene = NULL;

	fileReadStream.open(fileName.c_str());
	if (!fileReadStream.is_open()) {
		std::cerr << "file \"" << fileName << "\" could not open for reading"
				<< std::endl;
		throw 1;
	}
}

bool FileReader::readLine(std::string &buffer) {
	std::string line;

	do {
		getline(fileReadStream, line);
		if (line[0] != '#'
				&& line.find_first_not_of(" \t\r\n") != std::string::npos) {
			buffer = line;
			return true;
		}
	} while (fileReadStream);
	buffer = "";
	return false;
}

bool FileReader::readParams(std::stringstream &stringStream, float *parameters,
		int parameterCount) {
	for (int param = 0; param < parameterCount; ++param) {
		stringStream >> parameters[param];
		if (stringStream.fail()) {
			std::cerr << "reading failed for " << param << std::endl;
			return false;
		}
	}
	return true;
}

Scene* FileReader::readFile() {
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
			if (readParams(stringStream, parameters, 2)) {
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
			if (readParams(stringStream, parameters, 10)) {
				scene->setCamera(parameters[0], parameters[1], parameters[2],
						parameters[3], parameters[4], parameters[5],
						parameters[6], parameters[7], parameters[8],
						parameters[9]);
			}
		} else if (command == "ambient") {
			if (readParams(stringStream, parameters, 3)) {
				scene->setCurrentAmbient(parameters[0], parameters[1],
						parameters[2]);
			}
		} else if (command == "sphere") {
			if (readParams(stringStream, parameters, 4)) {
				scene->addSphere(parameters[0], parameters[1], parameters[2],
						parameters[3]);
			}
		} else if (command == "maxverts") {
			if (readParams(stringStream, parameters, 1)) {
				scene->createVertexSpace(parameters[0]);
			}
		} else if (command == "vertex") {
			if (readParams(stringStream, parameters, 3)) {
				scene->addVertex(parameters[0], parameters[1], parameters[2]);
			}
		} else if (command == "tri") {
			if (readParams(stringStream, parameters, 3)) {
				scene->addTriangle((int)parameters[0], (int)parameters[1], (int)parameters[2]);
			}
		} else if (command == "diffuse") {
			if (readParams(stringStream, parameters, 3)) {
				scene->setCurrentDiffuse(parameters[0], parameters[1], parameters[2]);
			}
		} else if (command == "specular") {
			if (readParams(stringStream, parameters, 3)) {
				scene->setCurrentSpecular(parameters[0], parameters[1], parameters[2]);
			}
		} else if (command == "shininess") {
			if (readParams(stringStream, parameters, 1)) {
				scene->setCurrentShininess(parameters[0]);
			}
		} else if (command == "translate") {
			if (readParams(stringStream, parameters, 3)) {
				temproryMatrix = Transform::translate(parameters[0],parameters[1],parameters[2]);
				scene->addTransform(temproryMatrix);
			}
		} else if (command == "scale") {
			if (readParams(stringStream, parameters, 3)) {
				temproryMatrix = Transform::scale(parameters[0],parameters[1],parameters[2]);
				scene->addTransform(temproryMatrix);
			}
		} else if (command == "rotate") {
			if (readParams(stringStream, parameters, 4)) {
				temproryMatrix = Transform::rotate(parameters[0],parameters[1],parameters[2],parameters[3]);
				scene->addTransform(temproryMatrix);
			}
		} else if (command == "pushTransform") {
				scene->pushTransform();
		} else if (command == "popTransform") {
				scene->popTransform();

		} else
			std::cerr << "command unknown: \"" << command << "\"" << std::endl;

	}

	return scene;
}

FileReader::~FileReader() {
//since scene is created by file reader, it should clean it up;
	if (scene != NULL)
		delete scene;

	fileReadStream.close();
}

