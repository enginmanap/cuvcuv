/*
 * Fim.h
 *
 *  Created on: 27 Kas 2014
 *      Author: Engin Manap
 */

#ifndef SRC_FILM_H_
#define SRC_FILM_H_

#include <cstring>
#include <cmath>
#include "Vec3f.h"

class Film {
	unsigned char* pixels;
	unsigned char* sampleCounts;

	unsigned int height, width;
	unsigned char colorDepth, samplingRate;

	float colorRange;
public:
	Film(unsigned int, unsigned int, unsigned char, unsigned char);
	virtual ~Film();
	bool setPixel(unsigned int&, unsigned int&, Vec3f&);
	unsigned char* getPixels(unsigned int&, unsigned int&);
};

#endif /* SRC_FILM_H_ */
