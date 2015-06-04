/*
 * FileReader.h
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#ifndef SRC_FILEREADER_H_
#define SRC_FILEREADER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stack>
#include "Mat4f.h"
#include "Transform.h"

#ifndef MAX_PARAMS
#define MAX_PARAMS 10
#endif //MAX_PARAMS

class FileReader {
protected:
	std::string filePath;
	std::string fileName;
	std::ifstream fileReadStream;
	bool readLine(std::string&);
	virtual bool readFloatParams(std::stringstream&, double *, int&);
	virtual bool readStringParams(std::stringstream&, std::string *, int);
public:
	FileReader(std::string filePath, std::string filename);
	virtual ~FileReader();
	inline static char separator() {
#ifdef _WIN32
		return '\\';
#else
		return '/';
#endif
	}
};

#endif /* SRC_FILEREADER_H_ */
