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
	static Mat4f translate(const float, const float, const float);
	static Mat4f rotate(const float, const float, const float, const float);
};

#endif /* TRANSFORM_H_ */
