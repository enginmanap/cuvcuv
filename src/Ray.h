/*
 * Ray.h
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#ifndef SRC_RAY_H_
#define SRC_RAY_H_

#include <limits>
#include <vector>
#include <cstdlib>
#include "Vec4f.h"

class Ray {
private:
	Vec4f position, direction, inverseDirection;
	float minDistance, maxDistance;
	void generateInverseDirection() {
		if (direction.x == 0)
			inverseDirection.x = std::numeric_limits<float>::max();
		else
			inverseDirection.x = 1.0f / direction.x;
		if (direction.y == 0)
			inverseDirection.y = std::numeric_limits<float>::max();
		else
			inverseDirection.y = 1.0f / direction.y;
		if (direction.z == 0)
			inverseDirection.z = std::numeric_limits<float>::max();
		else
			inverseDirection.z = 1.0f / direction.z;
		inverseDirection.w = 0; //std::numeric_limits<float>::infinity();//since direction must have 0 as w
	}
public:
	Ray() :	position(Vec4f()), direction(Vec4f()), inverseDirection(Vec4f()), minDistance(0), maxDistance(100) {}
	Vec4f getPosition() const {	return position;}
	Vec4f getDirection() const {return direction;}
	Vec4f getInverseDirection() const {return inverseDirection;}
	Ray(const Vec4f& position, const Vec4f& direction, float minDistance, float maxDistance) :
		position(position), direction(direction), minDistance(minDistance), maxDistance(maxDistance) {generateInverseDirection();}
	Ray(const Vec3f& position, const Vec3f& direction, float min, float max) :
		position(Vec4f(position, 1.0f)), direction(Vec4f(direction, 0.0f)), minDistance(min), maxDistance(max) {generateInverseDirection();}

	void setDirection(const Vec4f& direction) {
		this->direction = direction;
		generateInverseDirection();
	}

	void setPosition(const Vec4f& position) {this->position = position;}

	static std::vector<Ray> generateDeriveredRays(const Vec4f& origin,const Vec3f& direction,unsigned char gridSize, float maxDerivation){
		return generateDeriveredRays(origin,direction, Vec3f(0,1,0), gridSize, maxDerivation, maxDerivation);
	}
	static std::vector<Ray> generateDeriveredRays(const Vec4f&,const Vec3f&, const Vec3f&, unsigned char, float,float);

};

std::ostream& operator<<(std::ostream &, const Ray &);

#endif /* SRC_RAY_H_ */
