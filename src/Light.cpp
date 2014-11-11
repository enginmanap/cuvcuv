/*
 * Light.cpp
 *
 *  Created on: 10 Kas 2014
 *      Author: Engin
 */

#include "Light.h"

Light::Light() {
	// TODO Auto-generated constructor stub

}

Light::Light(Vec4f position,Vec3f color){
	this->position = position;
	this->color = color;
	this->attenuation = Vec3f(1.0f,0.0f,0.0f);
}

bool Light::setAttenuation(Vec3f& attenuation){
	this->attenuation = attenuation;
	return true;
}

float Light::getAttenuationFactor(float distance){
	return (1 / (attenuation.x + attenuation.y * distance + (attenuation.z * distance * distance)));
}


