/*
 * Vec3f.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_VEC3F_H_
#define SRC_VEC3F_H_

#include <math.h>
#include <iostream>
#include <cassert>
#include "Mat4f.h"

class Vec3f {
public:
	float x;
	float y;
	float z;
	Vec3f();
	Vec3f(float, float, float);
	virtual ~Vec3f();

	float getElement(const int) const;
	Vec3f normalize() const;
	Vec3f operator-(const Vec3f&) const;
	Vec3f operator+(const Vec3f&) const;
	Vec3f operator*(const Vec3f&) const;
	Vec3f operator*(const Mat4f&) const;
	float& operator[](int); //it is not const, so we can assign to it.
};

std::ostream& operator<<(std::ostream &strm, const Vec3f &vector);
Vec3f operator*(float&, const Vec3f&);
Vec3f operator*(const Mat4f&, const Vec3f&);

namespace vec3fNS {

Vec3f normalize(const Vec3f&);
Vec3f cross(const Vec3f&, const Vec3f&);
float dot(const Vec3f&, const Vec3f&);
Vec3f clamp(const Vec3f&, float, float);

}

#endif /* SRC_VEC3F_H_ */
