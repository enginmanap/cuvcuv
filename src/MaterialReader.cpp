/*
 * MaterialReader.cpp
 *
 *  Created on: 23 Ara 2014
 *      Author: engin
 */

#include "MaterialReader.h"


Material* MaterialReader::readMaterialFile() {
	std::string command;
	float floatParameters[MAX_PARAMS];
	std::string stringParameters[MAX_PARAMS];
	std::string firstLine;
	int parameterCount;
	if (!readLine(firstLine)) {
		std::cerr << "the file has no element to read" << std::endl;
		exit(1);
	} else {
		std::stringstream stringStream(firstLine);
		stringStream >> command;
		if (command != "newmtl") {
			std::cerr << "First command of material file has to be \"newmtl\"" << std::endl;
			exit(1);
		} else {
			if (readStringParams(stringStream, stringParameters, 1)) {
				material = new Material(stringParameters[0]);
			} else {
				std::cerr << "newmtl command parameters could not be read"
						<< std::endl;
				exit(1);
			}

		}

	}
	//Now we have the material , we can load out of order;
	std::string line;

	while (readLine(line)) {
		std::stringstream stringStream(line);
		stringStream >> command;
		if (command == "Ka") {//ambient
			if (readFloatParams(stringStream, floatParameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "Ka does not contain 3 floats, only " << parameterCount << " provided." << std::endl;
				} else {
					temporaryVector.x=floatParameters[0];
					temporaryVector.y=floatParameters[1];
					temporaryVector.z=floatParameters[2];
					material->setAmbient(temporaryVector);
				}
			}
		} else if (command == "Ke") {//emission
			if (readFloatParams(stringStream, floatParameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "Ka does not contain 3 floats, only " << parameterCount << " provided." << std::endl;
				} else {
				temporaryVector.x=floatParameters[0];
				temporaryVector.y=floatParameters[1];
				temporaryVector.z=floatParameters[2];
				material->setEmission(temporaryVector);
				}
			}
		} else if (command == "Kd") {//diffuse
			if (readFloatParams(stringStream, floatParameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "Ka does not contain 3 floats, only " << parameterCount << " provided." << std::endl;
				} else {
					temporaryVector.x=floatParameters[0];
					temporaryVector.y=floatParameters[1];
					temporaryVector.z=floatParameters[2];
					material->setDiffuse(temporaryVector);
				}
			}
		} else if (command == "Ks") {//specular
			if (readFloatParams(stringStream, floatParameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "Ka does not contain 3 floats, only " << parameterCount << " provided." << std::endl;
				} else {
					temporaryVector.x=floatParameters[0];
					temporaryVector.y=floatParameters[1];
					temporaryVector.z=floatParameters[2];
					material->setSpecular(temporaryVector);
				}
			}
		} else if (command == "Ns") {//shininess
			if (readFloatParams(stringStream, floatParameters, parameterCount)) {
				material->setShininess(floatParameters[0]);
			}
		} else
			std::cerr << "command unknown: \"" << command << "\"" << std::endl;

	}
	return material;
}
