/*
 * Ray.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SRC_RAY_H_
#define SRC_RAY_H_

#include "Vec4f.h"

class Ray {
private:
	Vec4f position, direction, inverseDirection;
	float minDistance, maxDistance;
public:
	Ray(): position(Vec4f()),direction(Vec4f()), inverseDirection(Vec4f()), minDistance(0), maxDistance(100) {};
	Vec4f getPosition() const {return position;};
	Vec4f getDirection() const {return direction;};
	Vec4f getInverseDirection() const {return inverseDirection;};
	Ray(Vec4f position, Vec4f direction, float minDistance,float maxDistance) : position(position),direction(direction), minDistance(minDistance), maxDistance(maxDistance) {
		this->inverseDirection = Vec4f(1.0f/this->direction.x, 1.0f/this->direction.y, 1.0f/this->direction.z, 0.0f);}
	Ray(Vec3f&, Vec3f&, float,float);
};

std::ostream& operator<<(std::ostream &, const Ray &);

#endif /* SRC_RAY_H_ */
