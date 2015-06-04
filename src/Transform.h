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

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif //M_PI

class Transform {
private:
	Transform(); // this class is only static

public:
	static Mat4f scale(const double, const double, const double);
	static Mat4f translate(const double, const double, const double);
	static Mat4f rotate(const double, const double, const double, const double);
};

#endif /* TRANSFORM_H_ */
