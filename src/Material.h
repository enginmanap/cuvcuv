/*
 * Material.h
 *
 *  Created on: 10 Ara 2014
 *      Author: engin
 */

#ifndef SRC_MATERIAL_H_
#define SRC_MATERIAL_H_

#include "Vec3f.h"
#include "string"

class Material {
	std::string name;
	Vec3f ambient, diffuse, specular,emission;
	float shininess;
	//transparency or dissolve is not used yet.
public:
	Material(std::string name) :
			name(name), shininess(0.0f){
	}
	const Vec3f& getAmbient() const {
		return ambient;
	}

	void setAmbient(const Vec3f& ambient) {
		this->ambient = ambient;
	}

	const Vec3f& getDiffuse() const {
		return diffuse;
	}

	void setDiffuse(const Vec3f& diffuse) {
		this->diffuse = diffuse;
	}

	const std::string& getName() const {
		return name;
	}

	void setName(const std::string& name) {
		this->name = name;
	}

	float getShininess() const {
		return shininess;
	}

	void setShininess(float shininess) {
		this->shininess = shininess;
	}

	const Vec3f& getSpecular() const {
		return specular;
	}

	void setSpecular(const Vec3f& specular) {
		this->specular = specular;
	}

	const Vec3f& getEmission() const {
		return emission;
	}

	void setEmission(const Vec3f& emission) {
		this->emission = emission;
	}

	;
};

#endif /* SRC_MATERIAL_H_ */
