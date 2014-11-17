/*
 * Camera.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#include "Camera.h"
#include <iostream>

Camera::Camera(float lookfromx, float lookfromy, float lookfromz, float lookatx,
		float lookaty, float lookatz, float upx, float upy, float upz,
		float fovy, int height, int width) {
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

	//calculate the u,v,w

	Vec3f positionMinusCenter = position - look;

	w = vec3fNS::normalize(positionMinusCenter);
	u = vec3fNS::normalize(vec3fNS::cross(up, w));
	v = vec3fNS::cross(w, u);


	halfWidth = (float) width / 2;
	halfHeight = (float) height / 2;
	xChangeFactor = tan(fovx / 2) / halfWidth;
	yChangeFactor = tan(fovy / 2) / halfHeight;
}

Ray Camera::getRay(int x, int y) {
	//Calculate the direction
	//since grader wants pixel centers, we will add 0.5 to pixels.

	float horizontalChange = xChangeFactor
			* ((float) x + 0.5f - halfWidth);

	float verticalChange = yChangeFactor
			* (halfHeight - (float)y + 0.5f);
	//std::cout << "for " << x << ", " << y << " horizontal change is "<< horizontalChange << " vertical change is "<< verticalChange << std::endl;
	Vec3f direction = (verticalChange * v) + (horizontalChange * u) - w;
	//Vec3f direction = (verticalChange * u) + (horizontalChange* v) - w;
	//Vec3f directionPart = verticalChange * v;

	direction = vec3fNS::normalize(direction);

	//std::cout << "the for u(" << u.x << "," << u.y << "," << u.z << ")" << " ray part is (" << direction.x << "," << direction.y << "," << direction.z << ")" << std::endl;

	Ray temp(position, direction, 0, 100);
	return temp;

}
