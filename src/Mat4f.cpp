/*
 * Mat4f.cpp
 *
 *  Created on: 4 Kas 2014
 *      Author: Engin
 */

#include "Mat4f.h"

Mat4f::Mat4f() {
	// TODO Auto-generated constructor stub

}

Vec4f& Mat4f::operator[](int index) const{
	switch (index) {
	case 0:
		return rows[0];
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
