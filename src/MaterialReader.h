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

class MaterialReader : public FileReader {
	Material* material;

	Vec3f temporaryVector;

public:

	MaterialReader(std::string filename): FileReader(filename){this->material=NULL;};
	virtual ~MaterialReader();

	Material* readMaterialFile();
};

#endif /* SRC_MATERIALREADER_H_ */
