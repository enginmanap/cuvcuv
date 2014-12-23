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

#ifndef MAX_PARAMS
#define MAX_PARAMS 10
#endif //MAX_PARAMS


class FileReader {
protected:
	std::string fileName;
	std::ifstream fileReadStream;
	Scene* scene;
	bool readLine(std::string&);
	virtual bool readFloatParams(std::stringstream&, float *, int);
	virtual bool readStringParams(std::stringstream&, std::string *, int);
public:
	FileReader(std::string filename);
	virtual ~FileReader();

	virtual Scene* readFile() = 0;
};

#endif /* SRC_FILEREADER_H_ */
