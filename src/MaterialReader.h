/*
 * MaterialReader.h
 *
 *  Created on: 23 Ara 2014
 *      Author: Engin Manap
 */

#ifndef SRC_MATERIALREADER_H_
#define SRC_MATERIALREADER_H_

#include <string>
#include <fstream>
#include <vector>

#include "Material.h"
#include "Vec3f.h"
#include "FileReader.h"


class MaterialReader : public FileReader {
	std::string filePath;
	std::vector<Material*> materials;
	Material* currentMaterial;

	Vec3f temporaryVector;

public:

	MaterialReader(std::string filePath, std::string filename): FileReader(filePath, filename), filePath(filePath), currentMaterial(NULL){};

	std::vector<Material*> readMaterialFile(std::vector<Texture*>&);
};

#endif /* SRC_MATERIALREADER_H_ */
