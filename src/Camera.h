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
	int height, width;
	Vec3f w,u,v;

	float xChangeFactor,yChangeFactor,halfWidth,halfHeight;
public:
	Camera(float, float, float, float, float, float, float, float, float,
			float, int, int);
	Ray getRay(int x, int y);

};

#endif /* CAMERA_H_ */
