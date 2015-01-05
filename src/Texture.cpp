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
	image = stbi_load(fileName.c_str(), &height, &width, &components, 0);
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

Vec3f Texture::getColor(const int x, const int y) const{
	//TODO implement bilinear/Trilinear filtering
	if(y > width || x > height){
		std::cerr << "the requested pixel is out of texture. texture size: (" << width <<"," << height << "), request: (" << x << "," << y << ")" << std::endl;
		return Vec3f(0,0,0);
	}
	unsigned char* pixel = image + ((height * y) + x) * components;
	//std::cout << "requested pixel " << x <<", "<< y;
	//std::cout << " value " << (int)*pixel << ", " << (int)*(pixel+1) << ", " << (int)*(pixel+2) << std::endl;
	//now we will  construct the color
	return Vec3f((*pixel)/255.0f,(*(pixel+1))/255.0f,(*(pixel+2))/255.0f);
}
