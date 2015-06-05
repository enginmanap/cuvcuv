/*
 * Vec3f.h
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#ifndef SRC_VEC3F_H_
#define SRC_VEC3F_H_

#include <math.h>
#include <iostream>
#include <cassert>

class Vec3f {
public:
	double x;
	double y;
	double z;
	Vec3f() :
			x(0), y(0), z(0) {
	}
	; //if no value is given, all elements are zero
	Vec3f(double x, double y, double z) :
			x(x), y(y), z(z) {
	}
	;

	double getElement(const int) const;
	Vec3f normalize() const;
	Vec3f operator-(const Vec3f&) const;
	Vec3f operator+(const Vec3f&) const;
	Vec3f operator*(const Vec3f&) const;
	double& operator[](int); //it is not const, so we can assign to it.
	double length() const {
		return sqrt((x * x) + (y * y) + (z * z));
	}
	;

};

std::ostream& operator<<(std::ostream &strm, const Vec3f &vector);
Vec3f operator*(double, const Vec3f&);

namespace Vec3fNS {

Vec3f normalize(const Vec3f&);
Vec3f cross(const Vec3f&, const Vec3f&);
double dot(const Vec3f&, const Vec3f&);
Vec3f clamp(const Vec3f&, double, double);

}

#endif /* SRC_VEC3F_H_ */
