/*
 * Transform.cpp
 *
 *  Created on: 5 Kas 2014
 *      Author: Engin
 */

#include "Transform.h"

Transform::Transform() {
}

Mat4f Transform::scale(const double sx, const double sy, const double sz) {
	Mat4f ret;
	ret[0].x = sx;
	ret[1].y = sy;
	ret[2].z = sz;
	return ret;
}

Mat4f Transform::translate(const double tx, const double ty, const double tz) {
	Mat4f ret;
	ret[3].x = tx;
	ret[3].y = ty;
	ret[3].z = tz;
	return ret;
}

Mat4f Transform::rotate(const double rx, const double ry, const double rz,
		double degrees) {
	//std::cout << rx <<", " << ry << ", " << rz<< ", " << degrees << std::endl;
	Vec3f axis(rx, ry, rz);
	Mat4f ret;

	Mat4f identityMat;
	Mat4f firstPartMat = Mat4f(
			axis[0] * axis[0], axis[0] * axis[1], axis[0] * axis[2], 0,
			axis[1] * axis[0], axis[1] * axis[1], axis[1] * axis[2], 0,
			axis[2] * axis[0], axis[2] * axis[1], axis[2] * axis[2], 0,
			0,0,0,1);

	Mat4f secondPartMat = Mat4f(
			0.0, axis[2], -axis[1],0.0,
			-axis[2], 0.0, axis[0],0.0,
			axis[1], -axis[0], 0.0,0.0,
			0.0,0.0,0.0,1.0);

	double radiants = degrees * M_PI / 180;

	ret = ((double)cos(radiants)) * identityMat;
	ret = ret + ((double)(1 - cos(radiants))) * firstPartMat;
	ret = ret + ((double)sin(radiants)) * secondPartMat;
	//now we should reset the last element
	ret[3][3]=1.0;
	return ret;

}
