/*
 * Transform.h
 *
 *  Created on: 5 Kas 2014
 *      Author: Engin
 */

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Vec3f.h"
#include "Mat4f.h"
#include <cmath>

class Transform {
private:
	Transform(); // this class is only static

public:
	static Mat4f scale(const float, const float, const float);
	static Mat4f translate(const float tx, const float ty, const float tz);
};

#endif /* TRANSFORM_H_ */
