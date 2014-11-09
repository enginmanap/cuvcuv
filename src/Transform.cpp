/*
 * Transform.cpp
 *
 *  Created on: 5 Kas 2014
 *      Author: Engin
 */

#include "Transform.h"

Transform::Transform() {
}

Mat4f Transform::scale(const float sx, const float sy, const float sz) {
	Mat4f ret;
	ret[0].x = sx;
	ret[1].y = sy;
	ret[2].z = sz;
	return ret;
}

Mat4f Transform::translate(const float tx, const float ty, const float tz) {
	Mat4f ret;
	ret[0].w = tx;
	ret[1].w = ty;
	ret[2].w = tz;
	return ret;
}

Mat4f Transform::rotate(const float rx, const float ry, const float rz,
		float degrees) {
	std::cout << rx <<", " << ry << ", " << rz<< ", " << degrees << std::endl;
	Vec3f axis(rx, ry, rz);
	Mat4f ret;

	Mat4f identityMat;
	Mat4f firstPartMat = Mat4f(
			axis[0] * axis[0], axis[0] * axis[1], axis[0] * axis[2], 0,
			axis[1] * axis[0], axis[1] * axis[1], axis[1] * axis[2], 0,
			axis[2] * axis[0], axis[2] * axis[1], axis[2] * axis[2], 0,
			0,0,0,1);

	Mat4f secondPartMat = Mat4f(
			0.0f, axis[2], -axis[1],0.0f,
			-axis[2], 0.0f, axis[0],0.0f,
			axis[1], -axis[0], 0.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f);

	float radiants = degrees * M_PI / 180;

	ret = ((float)cos(radiants)) * identityMat;
	ret = ret + ((float)(1 - cos(radiants))) * firstPartMat;
	ret = ret + ((float)sin(radiants)) * secondPartMat;
	//now we should reset the last element
	ret[3][3]=1.0f;
	return ret;

}
