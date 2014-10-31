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
	currentPoint = 0;

}

unsigned int Sampler::getHeight() {
	return height;
}

unsigned int Sampler::getWidht() {
	return width;
}

Sampler::~Sampler() {
	// TODO Auto-generated destructor stub
}

bool Sampler::getPoint(unsigned int& x, unsigned int& y) {
	if (currentPoint < height * width) {
		x = currentPoint / width;
		y = currentPoint % width;
		currentPoint++;
		return true;
	} else {
		return false;
	}
}
