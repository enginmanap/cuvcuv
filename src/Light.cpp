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
}
