/*
 * Vec4f.h
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#ifndef VEC4F_H_
#define VEC4F_H_

#include <math.h>
#include <iostream>
#include <cassert>
#include "Vec3f.h"

class Vec4f {
public:
	float x;
	float y;
	float z;
	float w;
	Vec4f();
	Vec4f(Vec3f, float);
	Vec4f(float, float, float, float);
	virtual ~Vec4f();
	Vec4f normalize() const;

	float getElement(const int) const;
	Vec4f operator-(const Vec4f&);
	Vec4f operator+(const Vec4f&);
	Vec4f operator*(const Vec4f&) const;
	operator Vec3f();

	float& operator[](int);
};

std::ostream& operator<<(std::ostream &strm, const Vec4f &vector);
Vec4f operator*(float& i, const Vec4f& vector);

namespace Vec4fNS {

Vec4f normalize(const Vec4f&);
float dot(const Vec4f&, const Vec4f&);
Vec4f clamp(const Vec4f&, float, float);

}

#endif /* VEC4F_H_ */
