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
	double shininess, dissolve, refractionIndex;
	Texture* map_Kd;
	Texture* map_Bump;
	//transparency or dissolve is not used yet.
public:
	Material(std::string name) :
			name(name), shininess(0.0), dissolve(1.0), refractionIndex(1.0), map_Kd(NULL), map_Bump(NULL) {
	}
	//FIXME this is stupid, we need copy constructor.
	Material(std::string name, Vec3f ambient, Vec3f diffuse, Vec3f specular,
			Vec3f emission, double shininess, double dissolve, double refractionIndex, Texture* map_Kd, Texture* map_Bump) :
			name(name), ambient(ambient), diffuse(diffuse), specular(specular), emission(
					emission), shininess(shininess),dissolve(dissolve), refractionIndex(refractionIndex), map_Kd(map_Kd), map_Bump(map_Bump) {
		//the textures are managed by scene
		//so we can safely copy the pointer
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

	double getShininess() const {
		return shininess;
	}

	void setShininess(double shininess) {
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

	Texture* getMapBump() const {
		return map_Bump;
	}

	void setMapBump(Texture* mapBump) {
		map_Bump = mapBump;
	}

	double getDissolve() const {
		return dissolve;
	}

	void setDissolve(double dissolve) {
		this->dissolve = dissolve;
	}

	double getRefractionIndex() const {
		return refractionIndex;
	}

	void setRefractionIndex(double refractionIndex) {
		this->refractionIndex = refractionIndex;
	}
};

#endif /* SRC_MATERIAL_H_ */
