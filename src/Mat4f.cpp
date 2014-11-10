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
	/*	std::cout << "created matrix:" << std::endl << *this << std::endl;

	 std::cout << "row[0]:" << this->rows[0]  << std::endl;
	 std::cout << "row[1]:" << this->rows[1]  << std::endl;
	 std::cout << "row[2]:" << this->rows[2]  << std::endl;
	 std::cout << "row[3]:" << this->rows[3]  << std::endl;
	 */
}

Mat4f::Mat4f(float defaultValue) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			this->rows[i][j] = defaultValue;
		}
	}
}

Mat4f::Mat4f(float xx,float xy,float xz,float xw,float yx,float yy,float yz,float yw,float zx,float zy,float zz,float zw,float wx,float wy ,float wz,float ww){
	this->rows[0][0] = xx;
	this->rows[0][1] = xy;
	this->rows[0][2] = xz;
	this->rows[0][3] = xw;
	this->rows[1][0] = yx;
	this->rows[1][1] = yy;
	this->rows[1][2] = yz;
	this->rows[1][3] = yw;
	this->rows[2][0] = zx;
	this->rows[2][1] = zy;
	this->rows[2][2] = zz;
	this->rows[2][3] = zw;
	this->rows[3][0] = wx;
	this->rows[3][1] = wy;
	this->rows[3][2] = wz;
	this->rows[3][3] = ww;

}

Vec4f& Mat4f::operator[](const int index) {
	switch (index) {
	case 0:
		return rows[0];
		break;
	case 1:
		return rows[1];
		break;
	case 2:
		return rows[2];
		break;
	case 3:
		return rows[3];
		break;
	default:
		std::cerr << "Mat4f[] index out of range" << std::endl;
		assert(0);
		break;
	}
}

float Mat4f::getElement(const int x, const int y) const {
	return rows[x].getElement(y);
}

Mat4f Mat4f::operator *(Mat4f& matrix) {

	Mat4f result(0);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			for (int k = 0; k < 4; ++k) {
				result[i][j] += rows[i][k] * matrix[k][j];
			}
		}
	}
	return result;
}


Vec4f& Mat4f::getRow(const int index) {
	return rows[index];
}

std::ostream& operator<<(std::ostream &strm, Mat4f &matrix) {
	for (int i = 0; i < 4; ++i) {
		strm << matrix.getRow(i) << std::endl;
	}
	return strm;
}

/**
 * this method gets the inverse of the matrix given.
 * If there is no inverse, it throws -1.
 */
Mat4f Mat4f::inverse(const Mat4f& matrix) {
	Mat4f inverse, copy;
	copy = matrix;

	Vec4f row;
	float portion;
	for (int i = 0; i < 4; ++i) {
		if (fabs(copy[i][i]) < EPSILON) {
			Vec4f temp = copy[i];
			int rowID = 0;
			//we should swap rows. find the biggest row.
			for (int j = i; j < 4; ++j) {
				if (copy[j][i] > temp[i]) {
					temp = copy[j];
					rowID = j;
				}
			}
			if (rowID == 0) {
				std::cerr << "no inverse possible row(" << i << ")"
						<< std::endl;
				std::cerr << "matrix to invert:" << std::endl << copy
						<< std::endl;
				throw -1;
				return Mat4f();
			} else {
				copy[rowID] = copy[i];
				copy[i] = temp;
				temp = inverse[i];
				inverse[i] = inverse[rowID];
				inverse[rowID] = temp;
			}

		}

		row = copy[i];

		if (fabs(row[i] - 1.0f) > EPSILON) { //if one, than we dont need to process

			portion = 1 / row[i];
			row = portion * row;
			inverse[i] = portion * inverse[i];
			copy[i] = portion * copy[i];

		}
		//now make the other lines zero
		//we know the first element is 1, so we can use it
		for (int j = 0; j < 4; ++j) {
			if (i == j)
				continue;
			if (copy[j][i] != 0) {
				//inverse[j] = inverse[j] - copy[j][i] * copy[i];
				inverse[j] = inverse[j] - copy[j][i] * inverse[i];
				copy[j] = copy[j] - copy[j][i] * copy[i];

			}
		}
	}

	return inverse;

}

Mat4f Mat4f::transpose() const{

	Mat4f trans;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			trans[j][i] = this->getElement(i,j);
		}
	}
	return trans;
}

Mat4f Mat4f::operator+(const Mat4f& matrix) const{
	Mat4f ret;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				ret[i][j] = this->getElement(i,j) +  matrix.getElement(i,j);
			}
		}
		return ret;
}

Mat4f operator*(float multipler, const Mat4f& matrix) {
	Mat4f ret;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			ret[i][j] = multipler *  matrix.getElement(i,j);
		}
	}
	return ret;
}

Vec4f Mat4f::operator*(const Vec4f& vector)const {
		Vec4f result;
		for (int i = 0; i < 4; ++i) {
				for (int k = 0; k < 4; ++k) {
					result[i] += getElement(i,k) * vector.getElement(k);
				}
		}
		return result;
}

Vec4f operator*(const Vec4f& vector, const Mat4f& matrix) {
		Vec4f result;
		for (int i = 0; i < 4; ++i) {
				for (int k = 0; k < 4; ++k) {
					result[i] += matrix.getElement(k,i) * vector.getElement(k);
				}
		}
		return result;
}

/**
 * assumes the forth element is 1;
 */
Vec3f operator*(const Vec3f vector,const Mat4f& matrix)  {
	Vec3f result;
	for (int i = 0; i < 3; ++i) {
			for (int k = 0; k < 3; ++k) {
				result[i] += matrix.getElement(k,i) * vector.getElement(k);
			}
			result[i] += matrix.getElement(3,i);
	}
	return result;
}


Vec3f Mat4f::operator*(const Vec3f& vector) const{
	Vec3f result;
	for (int i = 0; i < 3; ++i) {
			for (int k = 0; k < 3; ++k) {
				result[i] += getElement(i,k) * vector.getElement(k);
			}
			result[i] += getElement(i,3);
	}
	return result;
}
