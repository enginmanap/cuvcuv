/*
 * Ray.cpp
 *
 *  Created on: 29 Eki 2014
 *      Author: Engin Manap
 */

#include "Ray.h"


std::ostream& operator<<(std::ostream &strm, const Ray &ray){
	return strm << "pos(" << ray.getPosition() << "), direction(" << ray.getDirection() << ")";
}
