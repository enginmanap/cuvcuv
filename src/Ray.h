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
	double refractionIndex, distance;
	void generateInverseDirection() {
		if (direction.x == 0)
			inverseDirection.x = std::numeric_limits<double>::max();
		else
			inverseDirection.x = 1.0 / direction.x;
		if (direction.y == 0)
			inverseDirection.y = std::numeric_limits<double>::max();
		else
			inverseDirection.y = 1.0 / direction.y;
		if (direction.z == 0)
			inverseDirection.z = std::numeric_limits<double>::max();
		else
			inverseDirection.z = 1.0 / direction.z;
		inverseDirection.w = 0; //std::numeric_limits<double>::infinity();//since direction must have 0 as w
	}
public:
	Ray() :	position(Vec4f()), direction(Vec4f()), inverseDirection(Vec4f()), refractionIndex(0), distance(0) {}
	Vec4f getPosition() const {	return position;}
	Vec4f getDirection() const {return direction;}
	Vec4f getInverseDirection() const {return inverseDirection;}
	Ray(const Vec4f& position, const Vec4f& direction, double refractionIndex, double distance) :
		position(position), direction(direction), refractionIndex(refractionIndex), distance(distance) {generateInverseDirection();}
	Ray(const Vec3f& position, const Vec3f& direction, double refractionIndex, double distance) :
		position(Vec4f(position, 1.0)), direction(Vec4f(direction, 0.0)), refractionIndex(refractionIndex), distance(distance) {generateInverseDirection();}

	void setDirection(const Vec4f& direction) {
		this->direction = direction;
		generateInverseDirection();
	}

	void setPosition(const Vec4f& position) {this->position = position;}

	static std::vector<Ray> generateDeriveredRays(const Vec4f& origin,const Vec3f& direction, double refractionIndex, unsigned char gridSize, double maxDerivation){
		return generateDeriveredRays(origin,direction, Vec3f(0,1,0), refractionIndex, gridSize, maxDerivation, maxDerivation);
	}
	static std::vector<Ray> generateDeriveredRays(const Vec4f&,const Vec3f&, const Vec3f&, double, unsigned char, double,double);

	double getDistance() const {	return distance;}
	void setDistance(double distance) {this->distance = distance;}

	double getRefractionIndex() const {return refractionIndex;}
	void setRefractionIndex(double refractionIndex) {this->refractionIndex = refractionIndex;}
};

std::ostream& operator<<(std::ostream &, const Ray &);

#endif /* SRC_RAY_H_ */
