/*
 * Sampler.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Sampler.h"

Sampler::Sampler(unsigned int height, unsigned int width) {
	this->height = height;
	this->width = width;

}

unsigned int Sampler::getHeight(){
	return height;
}

unsigned int Sampler::getWidht(){
	return width;
}

Sampler::~Sampler() {
	// TODO Auto-generated destructor stub
}

