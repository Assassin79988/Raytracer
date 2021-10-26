#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "DataTypes.h"
#include "Ray.h"
#include <math.h>
#include <iostream>

namespace raytracer {
	class Material;
}

class raytracer::Material {
protected:
	float ks_ = 0.7f;
	float kd_ = 0.7f;
	float ka_ = 0.1f;
	float specularExponent_ = 10;

	float reflection_ = 0.0f;
	float refraction_ = 0.0f;

	Colour ambientColour = red();
	Colour diffuseColour = Colour(230, 216, 173);
	Colour specularColor = white();
public:
	Material() {}
	Material(const Material& m) {
		this->ks_ = m.ks_;
		this->kd_ = m.kd_;
		this->ka_ = m.ka_;
		this->specularExponent_ = m.specularExponent_;
		this->reflection_ = m.reflection_;
		this->refraction_ = m.refraction_;
		this->ambientColour = m.ambientColour;
		this->diffuseColour = m.diffuseColour;
		this->specularColor = m.specularColor;

	}

	Colour getAmbientColor() const { return ambientColour; }
	Colour getDiffuseColor() const { return diffuseColour; }
	Colour getSpecularColor() const { return specularColor; }
	float getSpecularExp() const { return specularExponent_; }
	float getReflection() const { return reflection_; }
	float getRefraction() const { return refraction_; }
	float getKa() const { return ka_; }
	float getKd() const { return kd_; }
	float getKs() const { return ks_; }

	void setAmbientColor(Colour colour) { ambientColour = colour; }
	void setDiffuseColor(Colour colour) { diffuseColour = colour; }
	void setSpecularColor(Colour colour) { specularColor = colour; }
	void setSpecularExp(float specularExponent) { specularExponent_ = specularExponent; }
	void setReflection(float reflection) { reflection_ = reflection; }
	void setRefraction(float refraction) {refraction_ = refraction; }
	void setKa(float ka) { ka_ = ka; }
	void setKd(float kd) { kd_ = kd; }
	void setKs(float ks) { ks_ = ks; }

	virtual bool getReflectedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray& ray) const = 0;
};

#endif
