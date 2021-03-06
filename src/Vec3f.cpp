/*
 * Vec3f.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "Vec3f.h"

Vec3f Vec3f::normalize() const {
	return Vec3fNS::normalize(*this);
}

Vec3f Vec3fNS::normalize(const Vec3f& vector) {
	//calculate the vector size
	double length = sqrt(
			(vector.x * vector.x) + (vector.y * vector.y)
					+ (vector.z * vector.z));

	//now define a new vector from it
	Vec3f temp(vector.x / length, vector.y / length, vector.z / length);
	return temp;
}

Vec3f Vec3fNS::cross(const Vec3f& vector1, const Vec3f& vector2) {
	Vec3f temp(vector1.y * vector2.z - vector1.z * vector2.y,
			vector1.z * vector2.x - vector1.x * vector2.z,
			vector1.x * vector2.y - vector1.y * vector2.x);
	return temp;
}

double Vec3fNS::dot(const Vec3f& vector1, const Vec3f& vector2) {
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

Vec3f Vec3fNS::clamp(const Vec3f& vector, double min, double max) {
	if (min > max) {
		std::cerr << "clamping is not possible for min: " << min << " max"
				<< max << std::endl;
		return vector;
	}
	Vec3f temp = vector;
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
	return temp;
}

Vec3f Vec3f::operator-(const Vec3f& vector2) const {
	Vec3f temp;
	temp.x = this->x - vector2.x;
	temp.y = this->y - vector2.y;
	temp.z = this->z - vector2.z;
	return temp;
}

Vec3f Vec3f::operator+(const Vec3f& vector2) const {
	Vec3f temp;
	temp.x = this->x + vector2.x;
	temp.y = this->y + vector2.y;
	temp.z = this->z + vector2.z;
	return temp;
}

Vec3f Vec3f::operator*(const Vec3f& vector2) const {
	Vec3f temp;
	temp.x = x * vector2.x;
	temp.y = y * vector2.y;
	temp.z = z * vector2.z;
	return temp;
}

Vec3f operator*(double i, const Vec3f& vector) {

	return Vec3f(i * vector.x, i * vector.y, i * vector.z);
}

double Vec3f::getElement(const int index) const {
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
	default:
		std::cerr << "Vecf3[] index out of range" << std::endl;
		assert(0);
		break;
	}
}

double& Vec3f::operator[](int index) {
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
	default:
		std::cerr << "Vecf3[] index out of range" << std::endl;
		assert(0);
		break;
	}
}

std::ostream& operator<<(std::ostream &strm, const Vec3f &vector) {
	return strm << "(" << vector.x << ", " << vector.y << ", " << vector.z
			<< ")";
}
