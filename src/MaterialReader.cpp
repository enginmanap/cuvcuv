/*
 * MaterialReader.cpp
 *
 *  Created on: 23 Ara 2014
 *      Author: Engin Manap
 */

#include "MaterialReader.h"

/**
 * This function returns a vector of materials,
 * and pushes any textures to the parameter.
 *
 * @params:
 * textures: std::vector<Texture*>
 *
 * @returns:
 * std::vector<Material*>
 */
std::vector<Material*> MaterialReader::readMaterialFile(std::vector<Texture*>& textures) {


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
				currentMaterial = new Material(stringParameters[0]);
				materials.push_back(currentMaterial);
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
					currentMaterial->setAmbient(temporaryVector);
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
				currentMaterial->setEmission(temporaryVector);
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
					currentMaterial->setDiffuse(temporaryVector);
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
					currentMaterial->setSpecular(temporaryVector);
				}
			}
		} else if (command == "Ns") {//shininess
			if (readFloatParams(stringStream, floatParameters, parameterCount)) {
				currentMaterial->setShininess(floatParameters[0]);
			}
		} else if (command == "newmtl") {//shininess
			if (readStringParams(stringStream, stringParameters, 1)) {
				currentMaterial = new Material(stringParameters[0]);
				materials.push_back(currentMaterial);
			} else {
				std::cerr << "newmtl command parameters could not be read"	<< std::endl;
			}
		} else if (command == "map_Kd") {//shininess
			if (readStringParams(stringStream, stringParameters, 1)) {
				Texture* texture = new Texture(stringParameters[0]);
				currentMaterial->setMapKd(texture);
				textures.push_back(texture);
			} else {
				std::cerr << "newmtl command parameters could not be read"	<< std::endl;
			}
		} else if (command == "d") {
			//TODO implement setting dissolved (transparency
		} else if (command == "illum") {
			//TODO implement setting illimunation model
		} else
			std::cerr << "command unknown: \"" << command << "\"" << std::endl;

	}
	return materials;
}
