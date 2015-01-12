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
Texture::Texture(std::string fileName): name(fileName) {

	//this function sets height witdh and components (RGB, RGBA etc.)
	//0 means read all components, 1 for greyscale, 2 for greyscale with alpha 3-4 RGB/A
	image = stbi_load(fileName.c_str(), &width, &height, &components, 0);
	if(image == NULL){
		std::cerr << "texture load failed for file " << fileName << std::endl;
		std::cout << stbi_failure_reason() <<  std::endl;
		throw 1;
	} else {
		std::cout << "texture loaded from file " << fileName << std::endl;
	}
}

Texture::~Texture() {
	stbi_image_free(image);
}

Vec3f Texture::getColor(float x, float y) const{
	//TODO implement bilinear/Trilinear filtering

	// sometimes relative coordinates can be bigger than 1 caused by rounding errors etc.
	// same goes for 0 too
	if(x < 0 && x > -EPSILON){
		x = 0.0f;
	} else if(x > 1 && x < 1 + EPSILON){
		x = 1.0f;
	}
	if (y < 0 && y > -EPSILON){
		y = 0.0f;
	} else if (y > 1 && y < 1 + EPSILON){
		y = 1.0f;
	}
	//this case means the difference was bigger than epsilon
	if(x > 1 || x < 0 || y > 1 || y<0) {
#pragma omp critical
		std::cerr << "the requested pixel is out of texture \""<< name <<"\". texture size: (" << height <<"," << width << "), request: (" << y*height << "," << x*width << ")" << "(" << y << "," << x << ")" << std::endl;
		return Vec3f(0,0,0);
	}
	int tX = x * (width-1);//-1 because it should start from 0, not one
	int tY = y * (height-1);// same here
	//std::cout << "h " << height << " w " << width << std::endl;
	unsigned char* pixel = image + (width*(height-1- tY) + tX) * components;
/*
#pragma omp critical
	{
		std::cerr << "\"" << name <<"\" texture size: (" << height <<"," << width << "), request: (" << tY << "," << tX << ")" << "(" << y << "," << x << ")" << ", means " << (width*(height-1)) - (width * tY) << "," << tX<< std::endl;

	}
*/
	//now we will  construct the color
	return Vec3f((*pixel)/255.0f,(*(pixel+1))/255.0f,(*(pixel+2))/255.0f);
}
