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

