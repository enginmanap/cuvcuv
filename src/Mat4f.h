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
#include <limits>
#include "Vec4f.h"


#ifndef EPSILON
#define EPSILON 0.001F
#endif

class Mat4f {
private:
	Vec4f rows[4];
public:
	Mat4f();
	Mat4f(float,float,float,float,float,float,float,float,float,float,float,float,float,float,float,float);
	Mat4f(float);

	Vec4f& getRow(const int);
	float getElement(const int, const int) const;
	Vec4f& operator[](const int);
	Mat4f operator+(const Mat4f&) const;
	Mat4f operator*(Mat4f&);
	Vec4f operator*(const Vec4f&) const;
	Vec3f operator*(const Vec3f&) const;
	static Mat4f inverse(const Mat4f&);
	Mat4f transpose()const;
};

std::ostream& operator<<(std::ostream &strm, Mat4f &matrix);
Mat4f operator*(float, const Mat4f&);
Vec4f operator*(const Vec4f&, const Mat4f&);
Vec3f operator*(const Vec3f&,const Mat4f&);

#endif /* MAT4F_H_ */
