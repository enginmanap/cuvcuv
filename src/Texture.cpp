/*
 * Texture.cpp
 *
 *  Created on: 5 Oca 2015
 *      Author: engin
 */

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/**
 * loads image with given filename
 */
Texture::Texture(std::string fileName) {

	//this function sets height witdh and components (RGB, RGBA etc.)
	//0 means read all components, 1 for greyscale, 2 for greyscale with alpha 3-4 RGB/A
	image = stbi_load(fileName.c_str(), &width, &height, &components, 0);
	if(image == NULL){
		std::cerr << "texture load failed for file " << fileName << std::endl;
		throw 1;
	} else {
		std::cout << "texture loaded from file " << fileName << std::endl;
	}
}

Texture::~Texture() {
	stbi_image_free(image);
}

Vec3f Texture::getColor(const float x, const float y) const{
	//TODO implement bilinear/Trilinear filtering
	if(y > 1 || x > 1){
		std::cerr << "the requested pixel is out of texture. texture size: (" << height <<"," << width << "), request: (" << x << "," << y << ")" << std::endl;
		return Vec3f(0,0,0);
	}
	int tX = x * width;
	int tY = y * height;
	//std::cout << "h " << height << " w " << width << std::endl;
	unsigned char* pixel = image + ((width*height) - (width * tY) + tX) * components;
	//std::cout << "requested pixel " << x * height <<", "<< y * height;
	//std::cout << " value " << (int)*pixel << ", " << (int)*(pixel+1) << ", " << (int)*(pixel+2) << std::endl;
	//now we will  construct the color
	return Vec3f((*pixel)/255.0f,(*(pixel+1))/255.0f,(*(pixel+2))/255.0f);
}
