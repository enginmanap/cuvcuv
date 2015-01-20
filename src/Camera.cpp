/*
 * Camera.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
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
	up = Vec3fNS::normalize(up);

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

	w = Vec3fNS::normalize(positionMinusCenter);
	u = Vec3fNS::normalize(Vec3fNS::cross(up, w));
	v = Vec3fNS::cross(w, u);

	halfWidth = (float) width / 2;
	halfHeight = (float) height / 2;
	xChangeFactor = tan(this->fovx / 2) / halfWidth;
	yChangeFactor = tan(this->fovy / 2) / halfHeight;
	srand(time(NULL));
}

bool Camera::getPoint(unsigned int& x, unsigned int& y) {
	if (currentPoint < height * width) {
		x = currentPoint % width;
		y = currentPoint / width;
		currentPoint++;
		if (currentPoint % ((height * width) / 100) == 0) {
			std::cout << "rendering: %"
					<< currentPoint / ((height * width) / 100) << std::endl;
		}
		return true;
	} else {
		std::cout << "no more pixel to render. " << std::endl;
		return false;
	}
}

/**
 * This method generates a vector of rays, formed as a grid, count determined by
 * rayCount
 */
bool Camera::getRays(unsigned int& x, unsigned int& y, unsigned int rayCount,
		std::vector<Ray>& rays) {
	if (rayCount < 1) {
		std::cerr << "requested ray count less than 1" << std::endl;
		assert(0);
	}

	if (this->getPoint(x, y)) {
		float horizontalChange = xChangeFactor
				* ((float) x + (0.5f) - halfWidth);

		float verticalChange = yChangeFactor
				* (halfHeight - ((float) y + (0.5f)));
		Vec3f direction = (verticalChange * v) + (horizontalChange * u) - w;
		direction = Vec3fNS::normalize(direction);
		rays.clear();
		std::vector<Ray> temp = Ray::generateDeriveredRays(Vec4f(position,0.0),direction,up, 1.0f, rayCount,xChangeFactor*0.5f, yChangeFactor*0.5f);
		rays.insert(rays.end(), temp.begin(),temp.end());
		return true;
	} else {
		return false;
	}

}
