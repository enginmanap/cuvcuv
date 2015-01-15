/*
 * ModelReader.h
 *
 *  Created on: 25 Ara 2014
 *      Author: Engin Manap
 */

#ifndef SRC_MODELREADER_H_
#define SRC_MODELREADER_H_

#include <string>
#include "FileReader.h"
#include "Model.h"
#include "Scene.h"
#include "Mat4f.h"
#include "MaterialReader.h"

class ModelReader: public FileReader {
	Model* model;
	std::string filePath;
	bool readFace(std::stringstream&, int*, int&);
public:
	ModelReader(std::string filePath, std::string filename) :
			FileReader(filePath, filename), filePath(filePath) {
		this->model = NULL;
	}
	;
	/**
	 * Scene parameter is used to add any materials and the(1) model
	 * to the scene. returned pointer can be ignored.
	 *
	 * @params
	 * scene: Scene object to alter
	 *
	 * @returns
	 * pointer to read model
	 */
	Model* readModelFile(Scene&);
};

#endif /* SRC_MODELREADER_H_ */
