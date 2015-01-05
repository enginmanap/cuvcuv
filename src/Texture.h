/*
 * Texture.h
 *
 *  Created on: 5 Oca 2015
 *      Author: engin
 */

#ifndef SRC_TEXTURE_H_
#define SRC_TEXTURE_H_

#include <string>
#include "Vec3f.h"

class Texture {
private:
	int height,width,components;
	unsigned char* image;

public:
	Texture(std::string);
	virtual ~Texture();

	Vec3f getColor(const float, const float) const;
};

#endif /* SRC_TEXTURE_H_ */
