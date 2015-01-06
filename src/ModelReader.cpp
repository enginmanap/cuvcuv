/*
 * ModelReader.cpp
 *
 *  Created on: 25 Ara 2014
 *      Author: Engin Manap
 */

#include "ModelReader.h"

bool ModelReader::readFace(std::stringstream& ss, int params[], int& readNumbers){
	memset(params,0,MAX_PARAMS*3*sizeof(int));
    std::string current;
    int currentValue=0;
    for (int param = 0; param < MAX_PARAMS; ++param) {
    	currentValue=param*3;
        ss >> current;
        if(ss.fail()){
            readNumbers=param;
            if(param<3){
                return false;
            }
            return true;
        }
        std::string delimiter= "/";
        unsigned int startPos = 0;
        unsigned int endPos = current.find_first_of(delimiter, 0);
        if(endPos == 0){
            std::cout << "face definition does not have a vertex index" << std::endl;
            exit(1);
        }
        //std::cout <<"start pos" << startPos << ", endPos " << endPos << std::endl;
        params[currentValue++] = atoi(current.substr(startPos, endPos).c_str());
        while (endPos <= current.size())
        {
            startPos = endPos+1;
            endPos = current.find_first_of(delimiter, startPos);

            //std::cout <<"start pos" << startPos << ", endPos " << endPos << std::endl;
            params[currentValue++] = atoi(current.substr(startPos, endPos - startPos).c_str());
        }
    }
    return true;
}

Model* ModelReader::readModelFile(Scene& scene) {
	model = new Model(scene.getTransform());
	model->setMaterial(scene.getMaterial());
	std::string command;
	float parameters[MAX_PARAMS];
	std::string stringParams[MAX_PARAMS];
	int faceParameters[MAX_PARAMS*3];//this is for vertex, texture and normal
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
		} else if (command == "vt") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if(parameterCount < 2) {
					std::cerr << "vertex texture coordinate does not contain 2 coordinates, only " << parameterCount << " provided." << std::endl;
				} else {
					model->addVertexTextureCoordinate(parameters[0], parameters[1]);
				}
			}
		} else if (command == "vn") {
			if (readFloatParams(stringStream, parameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "vertex normal does not contain 3 coordinates, only " << parameterCount << " provided." << std::endl;
				} else {
					model->addVertexNormal(parameters[0], parameters[1], parameters[2]);
				}
			}
		} else if (command == "s") {
			//TODO implement setting smoothing
		} else if (command == "g") {
			//TODO implement setting grouping
		} else if (command == "f") {
			if (readFace(stringStream, faceParameters, parameterCount)) {
				if(parameterCount < 3) {
					std::cerr << "face does not contain 3 vertices, only " << parameterCount << " provided." << std::endl;
				} else {
					if(faceParameters[1] == 0 && faceParameters[2] == 0){//this is a basic triangle with out any normal definition
						model->addTriangleBase(faceParameters[0], faceParameters[1*3],
								faceParameters[2*3]);
						//there might be more faces, we will interpret as GL_TRIANGLE_FAN
						float fanCenter=faceParameters[0], previousVertex=faceParameters[2*3];
						for(int i=3; i < parameterCount; ++i) {
							model->addTriangleBase((int) fanCenter, previousVertex,
									(int) faceParameters[i*3]);
							previousVertex = faceParameters[i*3];
						}
					} else { //this is a triangle with normal definitions and texture coordinates
						model->addTriangle(faceParameters[0], faceParameters[1*3], faceParameters[2*3],
								faceParameters[2], faceParameters[1*3+2], faceParameters[2*3+2],
								faceParameters[1], faceParameters[1*3+1], faceParameters[2*3+1]);
						//there might be more faces, we will interpret as GL_TRIANGLE_FAN
						int fanCenter=faceParameters[0], fanCenterNormal=faceParameters[2], fanCenterTexture=faceParameters[1], previousVertex=faceParameters[2*3], previousVertexNormal=faceParameters[2*3+2], previousVertexTexture=faceParameters[2*3+1];
						for(int i=3; i < parameterCount; ++i) {
							model->addTriangle(fanCenter, previousVertex,faceParameters[i*3],
									fanCenterNormal, previousVertexNormal,faceParameters[i*3+2],
									fanCenterTexture, previousVertexTexture,faceParameters[i*3+1]);
							previousVertex = faceParameters[i*3];
							previousVertexNormal = faceParameters[i*3+2];
							previousVertexTexture = faceParameters[i*3+1];
						}
					}
				}
			}
		} else if (command == "mtllib") {
			if (readStringParams(stringStream, stringParams, 1)) {
				//create a material reader and read material lib

				MaterialReader materialReader(filePath, stringParams[0]);
				std::vector<Texture*> textureVector;
				std::vector<Material*> materials = materialReader.readMaterialFile(textureVector);

				if(materials.size() != 0){
					scene.addMaterial(materials); //clearing the material is going to be done by scene
					//model->setMaterial(materials[0]);
				} else {
					std::cerr << "Material file " << stringParams[0] <<" has no material to be read." << std::endl;
				}
				if(!textureVector.empty()){
					scene.addTexture(textureVector);
				}
			}
		} else if (command == "usemtl") {
			if (readStringParams(stringStream, stringParams, 1)) {
				Material* mat = scene.getMaterial(stringParams[0]);
				if(mat!=NULL) {
					model->setMaterial(mat);
				} else {
					std::cerr << "Material with name " << stringParams[0] << " can not be found." << std::endl;
				}
			}
		} else
			std::cerr << "command unknown: \"" << command << "\"" << std::endl;

	}
	model->buildOctree();
	scene.addModel(model);
	return model;
}
