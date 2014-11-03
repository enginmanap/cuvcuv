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

class Vec4f {
public:
	float x;
	float y;
	float z;
	float w;
	Vec4f();
	Vec4f(float, float, float,float);
	virtual ~Vec4f();
	static Vec4f normalize(const Vec4f&);
	//static Vec4f cross(const Vec4f&, const Vec4f&); is this actual? If it is, do we need it?
	static float dot(const Vec4f&, const Vec4f&);
	static Vec4f clamp(const Vec4f&, float, float);

	Vec4f operator-(const Vec4f&);
	Vec4f operator+(const Vec4f&);
	Vec4f operator*(const Vec4f&) const;

};

std::ostream& operator<<(std::ostream &strm, const Vec4f &vector);
Vec4f operator*(float& i, const Vec4f& vector);

#endif /* VEC4F_H_ */
