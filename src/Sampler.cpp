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


bool Sampler::getPoint(unsigned int& x, unsigned int& y) {
	if (currentPoint < height * width) {
		x = currentPoint % width;
		y = currentPoint / width;
		currentPoint++;
		if(currentPoint % ((height * width)/100) == 0){
			std::cout << "rendering: %" << currentPoint / ((height * width)/100) << std::endl;
		}
		return true;
	} else {
		std::cout << "no more sample to render. " << std::endl;
		return false;
	}
}
