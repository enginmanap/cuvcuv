/*
 * Camera.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Camera.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>       /* time */

Camera::Camera(float lookfromx, float lookfromy, float lookfromz, float lookatx,
		float lookaty, float lookatz, float upx, float upy, float upz,
		float fovy, unsigned int height, unsigned int width) {
	position.x = lookfromx;
	position.y = lookfromy;
	position.z = lookfromz;

	look.x = lookatx;
	look.y = lookaty;
	look.z = lookatz;
//	look = vec3fNS::normalize(look);

	up.x = upx;
	up.y = upy;
	up.z = upz;
	up = vec3fNS::normalize(up);

	this->fovy = fovy * M_PI / 180;
	double d = (height * 0.5) / tan(this->fovy * 0.5);

	this->fovx = (float) (2 * atan((width * 0.5) / d));
	//this->fovx = this->fovy * ((float) width / (float) height);
	std::cout << "fovy " << this->fovy << " fovx " << this->fovx << std::endl;

	this->height = height;
	this->width = width;
	this->currentPoint = 0;

	//calculate the u,v,w

	Vec3f positionMinusCenter = position - look;

	w = vec3fNS::normalize(positionMinusCenter);
	u = vec3fNS::normalize(vec3fNS::cross(up, w));
	v = vec3fNS::cross(w, u);


	halfWidth = (float) width / 2;
	halfHeight = (float) height / 2;
	xChangeFactor = tan(this->fovx / 2) / halfWidth;
	yChangeFactor = tan(this->fovy / 2) / halfHeight;

}

bool Camera::getPoint(unsigned int& x, unsigned int& y) {
	if (currentPoint < height * width) {
		x = currentPoint % width;
		y = currentPoint / width;
		currentPoint++;
		if(currentPoint % ((height * width)/100) == 0){
			std::cout << "rendering: %" << currentPoint / ((height * width)/100) << std::endl;
		}
		return true;
	} else {
		std::cout << "no more pixel to render. " << std::endl;
		return false;
	}
}

/**
 * This method generates a ray
 */
bool Camera::getRays(unsigned int& x, unsigned int& y, unsigned int rayCount, Ray* ray) {
	if(rayCount < 1){
		std::cerr << "requested ray count less than 1" << std::endl;
		assert(0);
	}

	if(this->getPoint(x,y)){
		getRay(x,y,*ray);
		if(rayCount != 1) { // we should not alter for 1 ray;
			for(unsigned int i=1;i<rayCount;++i){
				//we need a random float between -0.5 and 0.5 for aliasing
				float xOffset = ((rand() % 100) / 100.0f) - 0.5f;
				float yOffset = ((rand() % 100) / 100.0f) - 0.5f;

				getRay(x,y,ray[i],xOffset,yOffset);
			}
		}
		return true;
	} else {
		return false;
	}

}

/**
 * this method generates a ray to center of the pixel,
 * with the offset values x and y
 */
void Camera::getRay(unsigned int x, unsigned int y, Ray& ray, float xOffset, float yOffset) {
		float horizontalChange = xChangeFactor
				* ((float) x + (0.5f + xOffset) - halfWidth);


		float verticalChange = yChangeFactor
				* (halfHeight - ((float)y + (0.5f + yOffset)));
		//std::cout << "for " << x << ", " << y << " horizontal change is "<< horizontalChange << " vertical change is "<< verticalChange << std::endl;
		Vec3f direction = (verticalChange * v) + (horizontalChange * u) - w;

		direction = vec3fNS::normalize(direction);
		//std::cout << "the for u(" << u.x << "," << u.y << "," << u.z << ")" << " ray part is (" << direction.x << "," << direction.y << "," << direction.z << ")" << std::endl;
		ray.setPosition(Vec4f(position ,1.0f));
		ray.setDirection(Vec4f(direction, 0.0f));
}

/**
 * This method generates a ray to the center of given point
 */
void Camera::getRay(unsigned int x, unsigned int y, Ray& ray) {
	getRay(x,y,ray,0,0);
}
