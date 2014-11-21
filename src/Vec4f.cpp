/*
 * Vec4f.cpp
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#include "Vec4f.h"

Vec4f::~Vec4f() {

}

Vec4f Vec4f::normalize() const {
	return Vec4fNS::normalize(*this);
}

float Vec4f::length(){
	return sqrt(
			((x * x) + (y * y)
					+ (z * z)) / (w * w) );
}

Vec4f Vec4fNS::normalize(const Vec4f& vector) {
	//calculate the vector size
	float length = sqrt(
			(vector.x * vector.x) + (vector.y * vector.y)
					+ (vector.z * vector.z) + (vector.w * vector.w));

	//now define a new vector from it
	Vec4f temp(vector.x / length, vector.y / length, vector.z / length,
			vector.w / length);
	return temp;
}

float Vec4fNS::dot(const Vec4f& vector1, const Vec4f& vector2) {
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z
			+ vector1.w * vector2.w;
}

Vec4f Vec4fNS::clamp(const Vec4f& vector, float min, float max) {
	if (min > max) {
		std::cerr << "clamping is not possible for min: " << min << " max"
				<< max << std::endl;
		return vector;
	}
	Vec4f temp = vector;
	if (vector.x > max)
		temp.x = max;
	else if (vector.x < min)
		temp.x = min;
	if (vector.y > max)
		temp.y = max;
	else if (vector.y < min)
		temp.y = min;
	if (vector.z > max)
		temp.z = max;
	else if (vector.z < min)
		temp.z = min;
	if (vector.w > max)
		temp.w = max;
	else if (vector.w < min)
		temp.w = min;
	return temp;
}

Vec4f Vec4f::operator-(const Vec4f& vector2) const {
	Vec4f temp;
	temp.x = x - vector2.x;
	temp.y = y - vector2.y;
	temp.z = z - vector2.z;
	temp.w = w - vector2.w;
	return temp;
}

Vec4f Vec4f::operator+(const Vec4f& vector2) {
	Vec4f temp;
	temp.x = x + vector2.x;
	temp.y = y + vector2.y;
	temp.z = z + vector2.z;
	temp.w = w + vector2.w;
	return temp;
}

Vec4f Vec4f::operator*(const Vec4f& vector2) const {
	Vec4f temp;
	temp.x = x * vector2.x;
	temp.y = y * vector2.y;
	temp.z = z * vector2.z;
	temp.w = w * vector2.w;
	return temp;
}

Vec4f operator*(float i, const Vec4f& vector) {

	return Vec4f(i * vector.x, i * vector.y, i * vector.z, i * vector.w);
}


std::ostream& operator<<(std::ostream &strm, const Vec4f &vector) {
	return strm << "(" << vector.x << ", " << vector.y << ", " << vector.z
			<< ", " << vector.w << ")";
}
