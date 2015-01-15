/*
 * Camera.h
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vec3f.h"
#include "Ray.h"
#include <cassert>
#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif //M_PI

class Camera {
private:
	Vec3f position, look, up;
	float fovy, fovx;
	unsigned int height, width;
	unsigned int currentPoint;
	Vec3f w, u, v;

	float xChangeFactor, yChangeFactor, halfWidth, halfHeight;
	void getRay(unsigned int x, unsigned int y, Ray& ray);
	void getRay(unsigned int, unsigned int, Ray&, float, float);
public:
	Camera(float, float, float, float, float, float, float, float, float, float,
			unsigned int, unsigned int);
	bool getPoint(unsigned int& x, unsigned int& y);
	bool getRays(unsigned int&, unsigned int&, unsigned int, Ray*);

};

#endif /* CAMERA_H_ */
