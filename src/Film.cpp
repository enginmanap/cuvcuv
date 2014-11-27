/*
 * Fim.cpp
 *
 *  Created on: 27 Kas 2014
 *      Author: engin
 */

#include "Film.h"

Film::Film(unsigned int height, unsigned int width, unsigned char colorDepth, unsigned char sampleRate) :
		height(height), width(width), colorDepth(colorDepth), samplingRate(sampleRate) {

	this->colorRange = pow(2, colorDepth) - 1; //for 8 bits, this means 255

	this->pixels = new unsigned char[height * width * colorDepth]; //FIXME this is wrong, for 8 bit colors, we dont need 8 byte arrays.
	memset(this->pixels, (int) colorRange, height * width * colorDepth); //standart says char is 1 byte

	this->sampleCounts = new unsigned char[height * width]; //don't support more than 255 samples
	memset(this->sampleCounts, 0, height * width); //we start with 0 samples

}

Film::~Film() {
	delete[] this->pixels;
}

unsigned char* Film::getPixels(unsigned int& height, unsigned int& width) {
	height = this->height;
	width = this->width;
	return this->pixels;
}

bool Film::setPixel(unsigned int& x, unsigned int& y, Vec3f& color) {
	unsigned int index = (this->width * y + x); //FIXME why we are not handling as matrix
	int currentSampleCount = this->sampleCounts[index];
	if (currentSampleCount >= samplingRate) {
		std::cerr << "sample rate limit passed, image is not altered." << std::endl;
		return false;
	} else {
		this->sampleCounts[index]++;
		//at this point, each element is 4 bytes, so index should have 4 bytes
		index *=4;
		unsigned char oldx, oldy, oldz, oldw;
		oldx = this->pixels[index + 0] * currentSampleCount;
		oldy = this->pixels[index + 1] * currentSampleCount;
		oldz = this->pixels[index + 2] * currentSampleCount;
		oldw = this->pixels[index + 3] * currentSampleCount;

		color = colorRange * vec3fNS::clamp(color, 0, 1); //Opengl auto clamps, we do manually.
#ifdef USE_FREEIMAGE_PNG
		oldx += (unsigned char) color.z;
		oldy += (unsigned char) color.y;
		oldz += (unsigned char) color.x;
		oldw += 255;
#else
		oldx += (unsigned char) color.x;
		oldy += (unsigned char) color.y;
		oldz += (unsigned char) color.z;
		oldw += 255;
#endif /*USE_FREEIMAGE_PNG*/

		//now we have another sample in oldx/y/z/w
		currentSampleCount++;

		this->pixels[index + 0] = oldx / currentSampleCount;
		this->pixels[index + 1] = oldy / currentSampleCount;
		this->pixels[index + 2] = oldz / currentSampleCount;
		this->pixels[index + 3] = oldw / currentSampleCount;
		return true;
	}
}
