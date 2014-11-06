/*
 * Mat4f.h
 *
 *  Created on: 4 Kas 2014
 *      Author: Engin
 */

#ifndef MAT4F_H_
#define MAT4F_H_

#include <cassert>
#include "Vec4f.h"

class Mat4f {
public:
	Mat4f();
	Mat4f(float);
	Vec4f rows[4];

	Vec4f& operator[](const int);
	Mat4f operator*(Mat4f&);
	static Mat4f inverse(Mat4f&);
};

std::ostream& operator<<(std::ostream &strm, Mat4f &matrix);

#endif /* MAT4F_H_ */
