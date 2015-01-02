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
	Vec4f():x(0),y(0),z(0),w(0){};
	Vec4f(Vec3f vec, float w):x(vec.x), y(vec.y), z(vec.z), w(w) {};
	Vec4f(float x, float y, float z, float w):x(x),y(y),z(z),w(w){};
	virtual ~Vec4f();
	Vec4f normalize() const;
	float length();

	Vec4f operator-(const Vec4f&)const;
	Vec4f operator+(const Vec4f&);
	Vec4f operator*(const Vec4f&) const;
	operator Vec3f() const {return Vec3f(x,y,z);};

	//these elements are here, so gcc can inline/optimise them. it can be forced, but I won't.
	float getElement(const int index) const {switch (index) {
	case 0:	return x; break;
	case 1:	return y; break;
	case 2:	return z; break;
	case 3:	return w; break;
	default: std::cerr << "Vecf4[] index out of range" << std::endl;
	assert(0);
	}};

	float& operator[](int index) {switch (index) {
	case 0:	return x; break;
	case 1:	return y; break;
	case 2:	return z; break;
	case 3:	return w; break;
	default: std::cerr << "Vecf4[] index out of range" << std::endl;
	assert(0);
	}};
};

std::ostream& operator<<(std::ostream &strm, const Vec4f &vector);
Vec4f operator*(float i, const Vec4f& vector);

namespace Vec4fNS {

Vec4f normalize(const Vec4f&);
float dot(const Vec4f&, const Vec4f&);
Vec4f clamp(const Vec4f&, float, float);

}

#endif /* VEC4F_H_ */
