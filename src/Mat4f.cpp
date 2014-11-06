/*
 * Mat4f.cpp
 *
 *  Created on: 4 Kas 2014
 *      Author: Engin
 */

#include "Mat4f.h"

/**
 * default constructor creates identity matrix
 */
Mat4f::Mat4f() {
	this->rows[0][0] = 1;
	this->rows[1][1] = 1;
	this->rows[2][2] = 1;
	this->rows[3][3] = 1;
}

Mat4f::Mat4f(float defaultValue) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			this->rows[i][j] = defaultValue;
		}
	}
}

Vec4f& Mat4f::operator[](const int index) {
	switch (index) {
	case 0:
		return (rows[0]);
		break;
	case 1:
		return rows[0];
		break;
	case 2:
		return rows[0];
		break;
	case 3:
		return rows[0];
		break;
	default:
		std::cerr << "Mat4f[] index out of range" << std::endl;
		assert(0);
		break;
	}
}

Mat4f Mat4f::operator *(Mat4f& matrix) {
	Mat4f result(0);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result[i][j] = rows[i][k] * matrix[k][j];
			}
		}
	}
	return result;
}

std::ostream& operator<<(std::ostream &strm, Mat4f &matrix) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			strm << matrix[i][j];
		}
	}
	return strm;
}

/**
 * this method assumes the
 */
Mat4f Mat4f::inverse(Mat4f& matrix) {

	Mat4f inverse;/*
	Vec4f row;
	float diff;
	row = matrix[0];
	diff = row[0] - 1 / row[0]; //this is the common multiplier
	inverse[0] = diff * row;
	//now make the lower lines zero
	inverse[1] = inverse - matrix[1];
	inverse[2] = inverse - matrix[2];
	inverse[3] = inverse - matrix[3];

	//now make the second row
	row = inverse[1];
	diff = row[1] - 1 / row[1];
	inverse[1] = diff * row[1];
	//now make the first line 0
	inverse[0] = inverse[0] - inverse[1] * inverse[0][1];
	//inverse[2] = inverse[2];*/
	return inverse;
}
