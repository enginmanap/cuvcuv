/*
 * Camera.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vec3f.h"
#include "Ray.h"
#include <math.h>

class Camera {
private:
	Vec3f position, look, up;
	float fovy, fovx;
	unsigned int height, width;
	unsigned int currentPoint;
	Vec3f w,u,v;

	float xChangeFactor,yChangeFactor,halfWidth,halfHeight;
public:
	Camera(float, float, float, float, float, float, float, float, float,
			float, unsigned int, unsigned int);
	bool getPoint(unsigned int& x, unsigned int& y);
	bool getRay( unsigned int&, unsigned int&,Ray&);

};

#endif /* CAMERA_H_ */
