/*
 * octree.cpp
 *
 *  Created on: 16 Kas 2014
 *      Author: engin
 */

#include "Octree.h"
#include <iostream>
#include <string>

Octree::Octree(Octree* parent, Vec3f upperEnd, Vec3f lowerEnd, std::vector<Primitive*> primitives) {
	//std::cout << "creating Octree with primitive count: " << primitives.size() << std::endl;
	//std::cout << "and up: " << upperEnd << ", low: " << lowerEnd << std::endl;
	static std::string level = "";
	this->parent = parent;
	this->upperEnd = upperEnd;
	this->lowerEnd = lowerEnd;
	Vec3f temp = this->upperEnd + this->lowerEnd;
	this->center = ((float)1/2) * temp;
	//std::cout << level << "up: " << upperEnd << ", low: " << lowerEnd << ", primitives: " << primitives.size() << ", center: " << center << std::endl;
	memset(children, 0,sizeof(Octree*) * 8);

	std::vector<Primitive*> contained,notContained,toCheck; //TODO this should be done by addPrimitive method.
	toCheck = primitives;
	if(primitives.size() > 1) {
		//we should calculate children
		if(upperEnd.x - lowerEnd.x >1.0f) {//if we can split more
			//calculate the sides
			//we need 8 up, 8 low, we have up down center^^^2, so we need 4 more
			/**
			 * the childs are:
			 * x = ++++----
			 * y = ++--++--
			 * z = +-+-+-+-
			 */
			Vec3f up[8];
			up[0] = upperEnd;
			up[1] = upperEnd; up[1].z = center.z;
			up[2] = upperEnd; up[2].y = center.y;
			up[3] = upperEnd; up[3].y = center.y; up[3].z = center.z;
			up[4] = upperEnd; up[4].x = center.x;
			up[5] = upperEnd; up[5].x = center.x; up[5].z = center.z;
			up[6] = upperEnd; up[6].x = center.x; up[6].y = center.y;
			up[7] = upperEnd; up[7].x = center.x; up[7].z = center.z;up[7].y = center.y;
			//std::cout << "up1: " << up[1]<< std::endl;
			Vec3f down[8];
			down[0] = center;
			down[1] = center; down[1].z = lowerEnd.z;
			down[2] = center; down[2].y = lowerEnd.y;
			down[3] = center; down[3].y = lowerEnd.y; down[3].z = lowerEnd.z;
			down[4] = center; down[4].x = lowerEnd.x;
			down[5] = center; down[5].x = lowerEnd.x; down[5].z = lowerEnd.z;
			down[6] = center; down[6].x = lowerEnd.x; down[6].y = lowerEnd.y;
			down[7] = center; down[7].x = lowerEnd.x; down[7].z = lowerEnd.z;down[7].y = lowerEnd.y;
			//std::cout << "low1: " << down[1]<< std::endl;

			for (unsigned int subtree = 0; subtree < 8; ++subtree) {
				for(unsigned int primitive=0; primitive < toCheck.size(); ++primitive) {
					switch (toCheck[primitive]->isInBoundingBox(up[subtree],down[subtree])) {
						case 2:
							//std::cout<< "putting to subtree node " << subtree << std::endl;
							contained.push_back(toCheck[primitive]);
							break;
						case 1:
							//std::cout<< "partially contained " << subtree << std::endl;
							contained.push_back(toCheck[primitive]);
							notContained.push_back(toCheck[primitive]);
							break;
						case 0:
							//std::cout<< "no contact with " << subtree << std::endl;
							notContained.push_back(toCheck[primitive]);
							break;
						default:
							break;
					}
				}
				std::string oldLevel = level;
				level = level + "  ";
				children[subtree] = new Octree(this,up[subtree],down[subtree],contained);
				level =oldLevel;
				contained.clear();
				toCheck.clear();
				toCheck = notContained;
				notContained.clear();
			}
		}
	}
	this->primitives = toCheck; //this leaf only has the elements that its children does not
}

Octree::~Octree() {
	for (int i = 0; i < 8; ++i) {
		if(children[i]!=0){
			delete children[i];
		}
	}
}

