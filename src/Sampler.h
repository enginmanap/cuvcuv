/*
 * Sampler.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <iostream>

class Sampler {
private:
	unsigned int height, width;
	unsigned int currentPoint;

public:

	Sampler(unsigned int, unsigned int);
	unsigned int getHeight() {return height;};
	unsigned int getWidht() {return width;};
	bool getPoint(unsigned int&, unsigned int&);
}
;

#endif /* SAMPLER_H_ */
