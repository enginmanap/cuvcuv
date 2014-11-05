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
	Vec4f rows[4];

	Vec4f& operator[](int);
};

#endif /* MAT4F_H_ */
