/*
 * FileReader.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "FileReader.h"

FileReader::FileReader(std::string filePath, std::string fileName) :
		filePath(filePath), fileName(fileName) {
	std::string concat = filePath + fileName;

	fileReadStream.open(concat.c_str());
	if (!fileReadStream.is_open()) {
		std::cerr << "file \"" << filePath << fileName
				<< "\" could not open for reading" << std::endl;
		throw 1;
	}
}

bool FileReader::readLine(std::string &buffer) {
	std::string line;

	do {
		getline(fileReadStream, line);
		if (line[0] != '#'
				&& line.find_first_not_of(" \t\r\n") != std::string::npos) {
			buffer = line;
			return true;
		}
	} while (fileReadStream);
	buffer = "";
	return false;
}

/**
 * Reads given count of parameters and puts to double array,
 * if the line contains less parameters, prints error and returns false.
 *
 * @stringStream: line to parse, with out the command
 * @parameters:array of double that parameters will be put
 * @parameterCount: number of parameters to read
 *
 * Returns: true if parameters assigment finished,
 * false if line finished before given count of parameters
 */
bool FileReader::readFloatParams(std::stringstream &stringStream,
		double *parameters, int& readParameterCount) {
	readParameterCount = MAX_PARAMS;
	for (int param = 0; param < MAX_PARAMS; ++param) {
		stringStream >> parameters[param];
		if (stringStream.fail()) {
			readParameterCount = param;
			if (param == 0) {
				std::cerr << "reading failed at start, please check input."
						<< std::endl;
				return false;
			}
			return true;
		}
	}
	return true;
}

/**
 * Reads given count of parameters and puts to string array,
 * if the line contains less parameters, prints error and returns false.
 *
 * @stringStream: line to parse, with out the command
 * @parameters:array of string that parameters will be put
 * @parameterCount: number of parameters to read
 *
 * Returns: true if parameters assigment finished,
 * false if line finished before given count of parameters
 */
bool FileReader::readStringParams(std::stringstream &stringStream,
		std::string *parameters, int parameterCount) {
	for (int param = 0; param < parameterCount; ++param) {
		stringStream >> parameters[param];
		if (stringStream.fail()) {
			std::cerr << "reading string failed for " << param << std::endl;
			return false;
		}
	}
	return true;
}

FileReader::~FileReader() {
//since scene is created by file reader, it should clean it up;
	fileReadStream.close();
}

