/*
 * Vec3f.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Vec3f.h"

Vec3f::Vec3f() {
	x = y = z = 0;
}

Vec3f::Vec3f(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3f::~Vec3f() {

}

Vec3f Vec3f::normalize(const Vec3f& vector) {
	//calculate the vector size
	float length = sqrt(
			(vector.x * vector.x) + (vector.y * vector.y)
					+ (vector.z * vector.z));

	//now define a new vector from it
	Vec3f temp(vector.x / length, vector.y / length, vector.z / length);
	return temp;
}

Vec3f Vec3f::cross(const Vec3f& vector1, const Vec3f& vector2) {
	Vec3f temp(vector1.y * vector2.z - vector1.z * vector2.y,
			vector1.z * vector2.x - vector1.x * vector2.z,
			vector1.x * vector2.y - vector1.y * vector2.x);
	return temp;
}

Vec3f Vec3f::operator-(const Vec3f& vector2) {
	Vec3f temp;
	temp.x = x - vector2.x;
	temp.x = y - vector2.y;
	temp.x = z - vector2.z;
	return temp;
}

Vec3f Vec3f::operator+(const Vec3f& vector2) {
	Vec3f temp;
	temp.x = x + vector2.x;
	temp.x = y + vector2.y;
	temp.x = z + vector2.z;
	return temp;
}

Vec3f operator*(float& i, const Vec3f& vector) {

	return Vec3f(i * vector.x, i * vector.y, i * vector.z);
}
