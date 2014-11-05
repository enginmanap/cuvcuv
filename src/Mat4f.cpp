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

Vec4f& Mat4f::operator[](int index) {
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

