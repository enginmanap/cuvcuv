/*
 * Ray.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_RAY_H_
#define SRC_RAY_H_

#include "Vec3f.h"

class Ray {
private:
	Vec3f position, direction;
	float minDistance, maxDistance;
public:
	Vec3f getPosition();
	Vec3f getDirection();
	Ray(Vec3f, Vec3f, float,float);
	virtual ~Ray();
};

#endif /* SRC_RAY_H_ */
