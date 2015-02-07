/*
 * Light.h
 *
 *  Created on: 10 Kas 2014
 *      Author: Engin
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Vec3f.h"
#include "Vec4f.h"

#ifndef EPSILON
#define EPSILON 0.000001F
#endif

class Light {
private:
	Vec4f position; //if 4th is zero, it means no position but direction
	Vec3f color;
	Vec3f attenuation;
	double size;

public:
	Light(Vec4f& position, double size, Vec3f& color) :
			position(position), color(color), attenuation(
					Vec3f(1.0, 0.0, 0.0)),size(size){
	}
	bool setAttenuation(Vec3f&);
	double getAttenuationFactor(double);

	const Vec3f& getColor() const {
		return color;
	}

	const Vec4f& getPosition() const {
		return position;
	}

	double getSize() const {
		return size;
	}
};

#endif /* LIGHT_H_ */
