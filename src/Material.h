#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "DataTypes.h"
#include <math.h>
#include <iostream>

namespace raytracer {
	class Material;
}

class raytracer::Material {
private:
	float ks_ = 0.7f;
	float kd_ = 0.7f;
	float ka_ = 0.1f;
	float specularExponent_ = 10;

	Colour ambientColour = red();
	Colour diffuseColour = Colour(230, 216, 173);
	Colour specularColor = white();
public:
	Material() {}

	Colour getAmbientColor() { return ambientColour; }
	Colour getDiffuseColor() { return diffuseColour; }
	Colour getSpecularColor() { return specularColor; }
	float getSpecularExp() { return specularExponent_; }
	float getKa() { return ka_; }
	float getKd() { return kd_; }
	float getKs() { return ks_; }

	void setAmbientColor(Colour colour) { ambientColour = colour; }
	void setDiffuseColor(Colour colour) { diffuseColour = colour; }
	void setSpecularColor(Colour colour) { specularColor = colour; }
	void setSpecularExp(float specularExponent) { specularExponent_ = specularExponent; }
	void setKa(float ka) { ka_ = ka; }
	void setKd(float kd) { kd_ = kd; }
	void setKs(float ks) { ks_ = ks; }
};

#endif
