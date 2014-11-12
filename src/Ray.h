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
	Vec4f position, direction;
	float minDistance, maxDistance;
public:
	Ray();
	Vec4f getPosition() const;
	Vec4f getDirection() const;
	Ray(Vec4f, Vec4f, float,float);
	Ray(Vec3f, Vec3f, float,float);
};

std::ostream& operator<<(std::ostream &, const Ray &);

#endif /* SRC_RAY_H_ */
