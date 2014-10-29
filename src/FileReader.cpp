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
	} while (1);
	buffer = "";
	return false;
}

bool FileReader::readParams(std::stringstream &stringStream, float *parameters,
		int parameterCount) {
	for (int param = 0; param < parameterCount; ++param) {
		stringStream >> parameters[param];
		if (stringStream.fail()) {
			std::cout << "reading failed for " << param << std::endl;
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
				scene = new Scene(parameters[0], parameters[1]);
			} else {
				std::cerr << "size command parameters could not be read"
						<< std::endl;
				exit(1);
			}

		}

	}
	//Now we have the scene, we can load camera, light etc. out of order;
	std::string line;

	while (readLine(line)) {
		std::stringstream stringStream(line);
		stringStream >> command;
		if (command == "camera") {
			if (readParams(stringStream, parameters, 10))
				scene->setCamera(parameters[0], parameters[1], parameters[2],
						parameters[3], parameters[4], parameters[5],
						parameters[6], parameters[7], parameters[8],
						parameters[9]);
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

