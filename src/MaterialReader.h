/*
 * MaterialReader.h
 *
 *  Created on: 23 Ara 2014
 *      Author: engin
 */

#ifndef SRC_MATERIALREADER_H_
#define SRC_MATERIALREADER_H_

#include <string>
#include <fstream>

#include "Material.h"
#include "Vec3f.h"
#include "FileReader.h"

#ifndef MAX_PARAMS
#define MAX_PARAMS 10
#endif //MAX_PARAMS


class MaterialReader : public FileReader {
	Material* material;

	Vec3f temporaryVector;

public:

	MaterialReader(std::string filename): FileReader(filename){this->material=NULL;};
	virtual ~MaterialReader();

	Scene* readFile() {return NULL;};
	Material* readMaterialFile();
};

#endif /* SRC_MATERIALREADER_H_ */
