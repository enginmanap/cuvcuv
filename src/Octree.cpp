/*
 * octree.cpp
 *
 *  Created on: 16 Kas 2014
 *      Author: engin
 */

#include "Octree.h"
#include <iostream>
#include <string>

/**
 * Creating a node automatically creates subtrees,
 * put primitives in respective
 */
Octree::Octree(Octree* parent, Vec3f& upperEnd, Vec3f& lowerEnd, std::vector<Primitive*>& primitives, unsigned char maxDepth): parent(parent), upperEnd(upperEnd),lowerEnd(lowerEnd) {
	//this variable is used for logging. With it we can intent based on the depth.
	static std::string level = "";
	Vec3f temp = this->upperEnd + this->lowerEnd;
	this->center = ((float) 1 / 2) * temp;
	memset(children, 0, sizeof(Octree*) * 8); //set children to NULL
	if (this->parent == NULL) {
		this->isSplittingRedudant = 0; //if root node
	} else {
		this->isSplittingRedudant = std::max(this->parent->isSplittingRedudant - 1, 0); //if parent was somewhat redudant, child is too
	}
	/**
	 * trying to split until each node has 1 element can easily break memory limits,
	 * and it is not a good idea. some pretty regular cases like 2 triangles forming
	 * a rectangle can never be split. We should never try to split more than
	 * k * primitives.size(). Scientific way to determine k would be measuring
	 * time it takes to test node intersection and primitive intersection and getting
	 * the number, but I am going to make it simply by saying can not delve more than
	 * half the depth.
	 */
	if(maxDepth > (primitives.size()/2+1)){//+1 is for 2 primitive cases
		maxDepth = primitives.size()/2;
	}

	std::vector<Primitive*> contained[8], notContained; //TODO this should be done by addPrimitive method.
	if(maxDepth <= 1 && primitives.size() >= 10){
		//this is not an error case, and it is common appearently
		//std::cout << "not split due to maxDepth setting for primitive count " << primitives.size() << std::endl;
	}
	if (maxDepth > 1 && primitives.size() > 1 && (upperEnd.x - lowerEnd.x > 0.1f)) {
		//we should calculate children if we can split more

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
		up[1] = upperEnd;
		up[1].z = center.z;
		up[2] = upperEnd;
		up[2].y = center.y;
		up[3] = upperEnd;
		up[3].y = center.y;
		up[3].z = center.z;
		up[4] = upperEnd;
		up[4].x = center.x;
		up[5] = upperEnd;
		up[5].x = center.x;
		up[5].z = center.z;
		up[6] = upperEnd;
		up[6].x = center.x;
		up[6].y = center.y;
		up[7] = upperEnd;
		up[7].x = center.x;
		up[7].z = center.z;
		up[7].y = center.y;
		//std::cout << "up1: " << up[1]<< std::endl;
		Vec3f down[8];
		down[0] = center;
		down[1] = center;
		down[1].z = lowerEnd.z;
		down[2] = center;
		down[2].y = lowerEnd.y;
		down[3] = center;
		down[3].y = lowerEnd.y;
		down[3].z = lowerEnd.z;
		down[4] = center;
		down[4].x = lowerEnd.x;
		down[5] = center;
		down[5].x = lowerEnd.x;
		down[5].z = lowerEnd.z;
		down[6] = center;
		down[6].x = lowerEnd.x;
		down[6].y = lowerEnd.y;
		down[7] = center;
		down[7].x = lowerEnd.x;
		down[7].z = lowerEnd.z;
		down[7].y = lowerEnd.y;

		unsigned char notContainedCounter = 0;
		for (std::vector<Primitive*>::iterator primIter = primitives.begin(); primIter != primitives.end(); ++primIter) {
			notContainedCounter = 0;
			for (unsigned int subtree = 0; subtree < 8; ++subtree) {
				/*
				 * 1 is contact or completely in, and 0 is no contact.
				 */
				if((*primIter)->isInBoundingBox(up[subtree], down[subtree]) == 1) {
					contained[subtree].push_back(*primIter);
				} else {
					++notContainedCounter;
				}
			}
			if (notContainedCounter == 8) {
				notContained.push_back(*primIter);
			}
		}
		//check if any node has exactly same primitives
		for (unsigned int subtree = 0; subtree < 8; ++subtree) {
			if (primitives.size() == contained[subtree].size())
				isSplittingRedudant++; //maybe we should not split?
		}
		if (isSplittingRedudant <= 15) { //since root box is optimised, we can try to be better
			//if splitting is not too redudant
			std::string oldLevel = level;
			level = level + "  ";
			for (unsigned int subtree = 0; subtree < 8; ++subtree) {
				children[subtree] = new Octree(this, up[subtree], down[subtree], contained[subtree], (maxDepth-1));
			}
			level = oldLevel;
			//FIXME should newer happen, but it does
			this->primitives = std::set<Primitive*>(notContained.begin(), notContained.end());
		} else {
			//if splitting was redudant, then skip splitting
			this->primitives = std::set<Primitive*>(primitives.begin(), primitives.end());
		}

	} else {
		//we hit some limit, and not going to create children
		this->primitives = std::set<Primitive*>(primitives.begin(), primitives.end());
	}

}

Octree::~Octree() {
	for (int i = 0; i < 8; ++i) {
		if (children[i] != 0) {
			delete children[i];
		}
	}
}

/*
 * Checks if ray intersects bounding box using slab method
 * http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
 */
bool Octree::isRayIntersects(const Ray& ray) const {
	Vec4f directionInverse = ray.getInverseDirection();

	Vec4f rayPos = ray.getPosition();
	float lowerXIntersection = (lowerEnd.x - rayPos.x) * directionInverse.x;
	float upperXIntersection = (upperEnd.x - rayPos.x) * directionInverse.x;

	float tmin = std::min(lowerXIntersection, upperXIntersection);
	float tmax = std::max(lowerXIntersection, upperXIntersection);

	float lowerYIntersection = (lowerEnd.y - rayPos.y) * directionInverse.y;
	float upperYIntersection = (upperEnd.y - rayPos.y) * directionInverse.y;

	tmin = std::max(tmin, std::min(lowerYIntersection, upperYIntersection));
	tmax = std::min(tmax, std::max(lowerYIntersection, upperYIntersection));

	float loverZIntersection = (lowerEnd.z - rayPos.z) * directionInverse.z;
	float upperZIntersection = (upperEnd.z - rayPos.z) * directionInverse.z;

	tmin = std::max(tmin, std::min(loverZIntersection, upperZIntersection));
	tmax = std::min(tmax, std::max(loverZIntersection, upperZIntersection));

    //tmin is intersection distance, if it is smaller than tmax
	return tmax >= std::max(0.0f, tmin);
}

/**
 * this function returns primitives that are
 * possibly intersecting with the ray.
 */
void Octree::getIntersectingPrimitives(const Ray& ray, std::set<Primitive*>& primitiveSet) const {
	//the camera might be in the box, in that case, the box itself is considered intersecting
	Vec3f rayPos = ray.getPosition();
	bool isCameraIn = false;
	if (rayPos.x >= lowerEnd.x && rayPos.x <= upperEnd.x && rayPos.y >= lowerEnd.y && rayPos.y <= upperEnd.y && rayPos.z >= lowerEnd.z
			&& rayPos.z <= upperEnd.z) {
		//the equals are needed, because event though calculating exact 0 is hard, giving it is easy
		isCameraIn = true;
	}

	if (isCameraIn || this->isRayIntersects(ray)) { //take advantage of short circuit
	//TODO ordering child traverse might improve performance
		if (children[0] != NULL) { //we know if there is 1 child, there are 8
			for (int i = 0; i < 8; ++i) {
				//if (children[i]->primitives.size() > 0) { //this check assumes no children means no children in branch, but there would not be any branch
				children[i]->getIntersectingPrimitives(ray, primitiveSet);
			}
		}
		//insert primitives of this node to the list.
		primitiveSet.insert(this->primitives.begin(), this->primitives.end());
	}
}

void Octree::print(){
	static std::string depth = "";
	std::string oldDepth = depth;
	std::ostringstream sstream;
	for(std::set<Primitive*>::iterator primIter=this->primitives.begin();primIter != this->primitives.end();++primIter){
		sstream << (*primIter)->id << " ";
	}
	std::cout << depth << "node " << this->upperEnd <<","<<this->lowerEnd << " has " << this->primitives.size() << " objects: " << sstream.str()<<std::endl;
	if(this->children[0]!=NULL){
		depth += "  ";
		for (int i = 0; i < 8; ++i) {
			this->children[i]->print();

		}
		depth = oldDepth;
	}

}
