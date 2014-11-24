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
#define EPSILON 0.0001F
#endif

class Light {
private:
	Vec4f position; //if 4th is zero, it means no position but direction
	Vec3f color;
	Vec3f attenuation;

public:
	Light();
	Light(Vec4f& position,Vec3f& color): position(position),color(color), attenuation(Vec3f(1.0f,0.0f,0.0f)) {}
	bool setAttenuation(Vec3f&);
	float getAttenuationFactor(float);

	const Vec3f& getColor() const {
		return color;
	}

	const Vec4f& getPosition() const {
		return position;
	}
};

#endif /* LIGHT_H_ */
