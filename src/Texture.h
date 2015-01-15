/*
 * Texture.h
 *
 *  Created on: 5 Oca 2015
 *      Author: engin
 */

#ifndef SRC_TEXTURE_H_
#define SRC_TEXTURE_H_

#ifndef EPSILON
#define EPSILON 0.001
#endif

#include <string>
#include "Vec3f.h"

class Texture {
private:
	std::string name;
	int height, width, components;
	unsigned char* image;

public:
	Texture(std::string);
	virtual ~Texture();

	Vec3f getColor(float, float) const;
};

#endif /* SRC_TEXTURE_H_ */
