/*
 * Mat4f.h
 *
 *  Created on: 4 Kas 2014
 *      Author: Engin
 */

#ifndef MAT4F_H_
#define MAT4F_H_

#include <cassert>
#include <cmath>
#include "Vec4f.h"

#define EPSILON 0.00001F

class Mat4f {
private:
	Vec4f rows[4];
public:
	Mat4f();
	Mat4f(float);

	Vec4f& getRow(const int);
	float getElement(const int, const int) const;
	Vec4f& operator[](const int);
	Mat4f operator*(Mat4f&);
	static Mat4f inverse(const Mat4f&);
};

std::ostream& operator<<(std::ostream &strm, Mat4f &matrix);

#endif /* MAT4F_H_ */
