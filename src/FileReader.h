/*
 * FileReader.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_FILEREADER_H_
#define SRC_FILEREADER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stack>
#include "Scene.h"
#include "Mat4f.h"
#include "Transform.h"

#define MAX_PARAMS 10

class FileReader {
protected:
	std::string fileName;
	std::ifstream fileReadStream;
	Scene* scene;
	bool readLine(std::string&);
	virtual bool readParams(std::stringstream&, float *, int);
public:
	FileReader(std::string filename);
	virtual ~FileReader();

	virtual Scene* readFile() = 0;
};

#endif /* SRC_FILEREADER_H_ */
