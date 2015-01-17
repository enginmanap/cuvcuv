/*
 * Ray.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "Ray.h"

std::ostream& operator<<(std::ostream &strm, const Ray &ray) {
	return strm << "pos(" << ray.getPosition() << "), direction("
			<< ray.getDirection() << ")";
}

/**
 * generates 1 ray per grid + center.
 * order:
 * 1-> center
 * 2-5-> corners
 * rest -> per grid random rays
 *
 * params:
 * @origin: origin for rays to generate
 * @direction: direction to derive from, also first rays direction
 * @gridSize: how many parts to split. it is squired, so 3 means 3x3
 * @maxDerivation: how much derivation is required. 0 means all of them will be same, 1 means even perpendicular is allowed.
 *
 * returns:
 * vector of rays generated
 */
std::vector<Ray> Ray::generateDeriveredRays(const Vec4f& origin,const Vec3f& direction,unsigned char gridSize, float maxDerivation){
	//FIXME this vector should not be recreated per call, it should be a parameter

	std::vector<Ray> deriveredRays;
	Vec3f u,v, w, offseV;
	Vec3f up(0,1,0), tempDirection;
	w = direction.normalize();
	u = Vec3fNS::normalize(Vec3fNS::cross(up, w));
	v = Vec3fNS::cross(w, u);

	// gridsize 2 will mean 1 on each side, so we will /2 it
	float perGridSize = maxDerivation/(gridSize/2);

	deriveredRays.push_back(Ray(origin,w,0,100));

	//set the corners max values, so they can be tested beforehand
	if (gridSize >= 2) {
		for (char i = 0; i < 2; ++i) {
			for (char j = 0; j < 2; ++j) {
				//2*i-1 means if 0 -1, if 1 1.
				tempDirection = ((maxDerivation*((2*i)-1))*u) + ((maxDerivation*((2*j)-1))*v);
				tempDirection = tempDirection + w;
				deriveredRays.push_back(Ray(origin,tempDirection.normalize(),0,100));
			}
		}
	}

	//generate random rays for each grid cell, except corners.
	if (gridSize > 2) {

		float offsetU, offsetV;

		for (unsigned char i = 0; i < gridSize; ++i) {
			for (unsigned char j = 0; j < gridSize; ++j) {
				//pass the corners
				if (!((i == 0 && j == 0) || (i == gridSize - 1 && j == 0)|| (i == 0 && j == gridSize - 1)|| (i == gridSize - 1 && j == gridSize - 1))) {
					offsetU = ((rand() / float(RAND_MAX + 1) * perGridSize) + perGridSize * i) - maxDerivation;
					offsetV = ((rand() / float(RAND_MAX + 1) * perGridSize) + perGridSize * j) - maxDerivation;
					tempDirection = w + offsetU*u + offsetV*v;
					deriveredRays.push_back(Ray(origin,tempDirection.normalize(),0,100));
				}
			}
		}
	}
	/*
#pragma omp critical
	{
	std::cout <<"rays start "<< std::endl;
	for(int i=0; i< deriveredRays.size();++i){
		std::cout <<"ray dir: " << deriveredRays[i].getDirection() << std::endl;
	}
	}
	std::cout <<"rays end "<< std::endl;
*/
	return deriveredRays;
}
