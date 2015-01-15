/*
 * Light.cpp
 *
 *  Created on: 10 Kas 2014
 *      Author: Engin
 */

#include "Light.h"

bool Light::setAttenuation(Vec3f& attenuation) {
	this->attenuation = attenuation;
	return true;
}

float Light::getAttenuationFactor(float distance) {
	if (fabs(this->position.w) < EPSILON)
		return 1.0f;
	return (1
			/ (attenuation.x + attenuation.y * distance
					+ (attenuation.z * distance * distance)));
}

