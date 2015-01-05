/*
 * Material.h
 *
 *  Created on: 10 Ara 2014
 *      Author: Engin Manap
 */

#ifndef SRC_MATERIAL_H_
#define SRC_MATERIAL_H_

#include "Vec3f.h"
#include "string"
#include "Texture.h"

#ifndef DEFAULT_MATERIAL_NAME
#define DEFAULT_MATERIAL_NAME "_default_material"
#endif

class Material {
	std::string name;
	Vec3f ambient, diffuse, specular, emission;
	float shininess;
	Texture* map_Kd;
	//transparency or dissolve is not used yet.
public:
	Material(std::string name) :
			name(name), shininess(0.0f), map_Kd(NULL){
	}
	Material(std::string name, Vec3f ambient, Vec3f diffuse, Vec3f specular, Vec3f emission, float shininess, Texture* map_Kd) :
			name(name), ambient(ambient),diffuse(diffuse), specular(specular), emission(emission),shininess(shininess), map_Kd(map_Kd){
		//the textures are managed by scene
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

	Texture* const getMapKd() const {
		return map_Kd;
	}

	void setMapKd(Texture* mapKd) {
		map_Kd = mapKd;
	}
	;
};

#endif /* SRC_MATERIAL_H_ */
