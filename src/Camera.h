/*
 * Camera.h
 *
 *  Created on: 29 Eki 2014
 *      Author: engin
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vec3f.h"

class Camera {
private:
	Vec3f position, look, up;
	float fovy;
public:
	Camera(float, float, float, float, float, float, float, float, float,
			float);
	virtual ~Camera();
};

#endif /* CAMERA_H_ */
