/*
 * Vec4f.cpp
 *
 *  Created on: 3 Kas 2014
 *      Author: Engin
 */

#include "Vec4f.h"

/**
 * if no value is given, all elements are zero
 */
Vec4f::Vec4f() {
	x = y = z = w = 0;
}

Vec4f::Vec4f(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vec4f::Vec4f(Vec3f vector, float w){
	//std::cout << "building from vector" << vector << std::endl;
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	this->w = w;
}

Vec4f::~Vec4f() {

}

Vec4f Vec4f::normalize() const {
	return Vec4fNS::normalize(*this);
}

float Vec4f::length(){
	return sqrt(
			(x * x) + (y * y)
					+ (z * z));
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

float Vec4f::getElement(const int index) const {
	switch (index) {
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	case 3:
		return w;
		break;
	default:
		std::cerr << "Vecf3[] index out of range" << std::endl;
		assert(0);
		break;
	}
}

float& Vec4f::operator[](int index) {
	switch (index) {
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	case 2:
		return z;
		break;
	case 3:
		return w;
		break;
	default:
		std::cerr << "Vecf4[] index out of range" << std::endl;
		assert(0);
		break;
	}
}

Vec4f operator*(float i, const Vec4f& vector) {

	return Vec4f(i * vector.x, i * vector.y, i * vector.z, i * vector.w);
}


std::ostream& operator<<(std::ostream &strm, const Vec4f &vector) {
	return strm << "(" << vector.x << ", " << vector.y << ", " << vector.z
			<< ", " << vector.w << ")";
}

Vec4f::operator Vec3f(){
	return Vec3f(x,y,z);
}
