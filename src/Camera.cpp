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

Camera::Camera(double lookfromx, double lookfromy, double lookfromz, double lookatx,
		double lookaty, double lookatz, double upx, double upy, double upz,
		double fovy, unsigned int height, unsigned int width) {
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

	this->fovx = (double) (2 * atan((width * 0.5) / d));
	//this->fovx = this->fovy * ((double) width / (double) height);
	std::cout << "fovy " << this->fovy << " fovx " << this->fovx << std::endl;

	this->height = height;
	this->width = width;
	this->currentPoint = 0;

	//calculate the u,v,w

	FocalDistance = position - look;

	w = Vec3fNS::normalize(FocalDistance);
	u = Vec3fNS::normalize(Vec3fNS::cross(up, w));
	v = Vec3fNS::cross(w, u);

	halfWidth = (double) width / 2;
	halfHeight = (double) height / 2;
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
bool Camera::getRays(unsigned int& x, unsigned int& y, unsigned int rayCount, unsigned int DOFRate,
		std::vector<Ray>& rays) {
	if (rayCount < 1) {
		std::cerr << "requested ray count less than 1" << std::endl;
		assert(0);
	}

	if (this->getPoint(x, y)) {

		double horizontalChange = xChangeFactor
				* ((double) x + (0.5) - halfWidth);

		double verticalChange = yChangeFactor
				* (halfHeight - ((double) y + (0.5)));
		Vec3f direction = (verticalChange * v) + (horizontalChange * u) - w;
		direction = Vec3fNS::normalize(direction);


		//calculate focal point, and randomize the path, reverse.
		Vec3f focalPoint = this->position + FocalDistance.length()* direction;

		//std::cout << "direction" << direction << ", focal point " << focalPoint << std::endl;

		rays.clear();
		//std::vector<Ray> temp = Ray::generateDeriveredRays(Vec4f(focalPoint,1.0),direction,up, 1.0, rayCount,xChangeFactor*0.5,yChangeFactor*0.5);
		
		
		if(DOFRate == 1) {
			std::vector<Ray> temp = Ray::generateDeriveredRays(Vec4f(position, 1.0),direction,up, 1.0, rayCount,xChangeFactor*0.5,yChangeFactor*0.5);
			rays.insert(rays.end(), temp.begin(),temp.end());

		} else {
		//this means we need to generate rays for depth of field too.
			std::vector<Ray> focalRays;

			//change ray origin based on direction u/w
			Vec3f u,v, w, offseV;
			Vec3f tempDirection, tempOrigin;
			w = -1 * direction.normalize();
			u = Vec3fNS::normalize(Vec3fNS::cross(up, w));
			v = Vec3fNS::cross(w, u);

			//FIXME We should read this value as part of camera setup
			double aperture = 0.25;
			double uChange,vChange;

			
			for(unsigned int i=0; i<DOFRate;i++){
				uChange = (rand() / double(RAND_MAX + 1) - 0.5) * aperture;
				vChange = (rand() / double(RAND_MAX + 1) - 0.5) * aperture;

				//now calculate direction from new origin to the focal point
				tempOrigin = this->position + uChange * u + vChange * v;
				tempDirection = (focalPoint - tempOrigin).normalize();

				focalRays.push_back(Ray(tempOrigin,tempDirection,1.0,100));
				//std::cout << temp[temp.size()-1] << ", target " << temp[temp.size()-1].getPosition() + FocalDistance.length() * temp[temp.size()-1].getDirection() << std::endl;

			}


			//std::cout << std::endl;
			
			for(std::vector<Ray>::iterator rayIt = focalRays.begin();
			rayIt != focalRays.end(); ++ rayIt){
				std::vector<Ray> temp = Ray::generateDeriveredRays(rayIt->getPosition(),rayIt->getDirection(),up, 1.0, rayCount,xChangeFactor*0.5,yChangeFactor*0.5);
				rays.insert(rays.end(), temp.begin(),temp.end());
			}
		}

		return true;
	} else {
		return false;
	}

}
