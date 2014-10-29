/*
 * Sampler.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

class Sampler {
private:
	unsigned int height, width;
	unsigned int currentPoint;

public:

	Sampler(unsigned int, unsigned int);
	unsigned int getHeight();
	unsigned int getWidht();
	bool getPoint(unsigned int&, unsigned int&);
	virtual ~Sampler();
}
;

#endif /* SAMPLER_H_ */
