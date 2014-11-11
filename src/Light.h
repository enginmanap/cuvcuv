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
#define EPSILON 0.00001F
#endif

class Light {
private:
	Vec4f position; //if 4th is zero, it means no position but direction
	Vec3f color;
	Vec3f attenuation;

public:
	Light();
	Light(Vec4f,Vec3f);

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
