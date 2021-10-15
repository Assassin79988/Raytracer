#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "DataTypes.h"
#include <math.h>
#include <iostream>

#define MAX(a,b) (a<b)?b:a

namespace raytracer {
	class Material;
}

class raytracer::Material {
private:
	float ks_ = 0.7;
	float kd_ = 0.6;
	float ka_ = 0.1;
	float specularExponent_ = 13;

	Colour ambientColour = black();
	Colour diffuseColour = blue();
	Colour specularColor = white();

	uchar clamp(int color) const;
public:
	Material() {}

	Colour phong(Vec3 viewer, Vec3 normal, Vec3 lightVector) const;
	Colour blinnPhong(Vec3 viewer, Vec3 normal, Vec3 lightVector) const;
};

uchar raytracer::Material::clamp(int color) const {
	if (color < 0) return 0;
	if (color >= 255) return 255;
	return color;
}

Colour raytracer::Material::phong(Vec3 viewer, Vec3 normal, Vec3 lightVector) const {
	float diffuseTerm = MAX(lightVector.dot(normal), 0);

	Vec3 relfectedLight = 2 * lightVector.dot(normal) * normal - lightVector;
	relfectedLight.normalize();

	float specularTerm = pow(MAX(relfectedLight.dot(viewer), 0), specularExponent_);
	//if (diffuseTerm < 0.01) std::cout << relfectedLight.dot(viewer) <<  "\t" << pow(relfectedLight.dot(viewer), specularExponent_) << std::endl;
	Colour colour(0,0,0);
	colour[0] = clamp(ka_ * ambientColour[0] + kd_ * diffuseColour[0] * diffuseTerm + ks_ * specularColor[0] * specularTerm);
	colour[1] = clamp(ka_ * ambientColour[1] + kd_ * diffuseColour[1] * diffuseTerm + ks_ * specularColor[1] * specularTerm);
	colour[2] = clamp(ka_ * ambientColour[2] + kd_ * diffuseColour[2] * diffuseTerm + ks_ * specularColor[2] * specularTerm);
	return colour;
}

Colour raytracer::Material::blinnPhong(Vec3 viewer, Vec3 normal, Vec3 lightVector) const {
	float diffuseTerm = MAX(lightVector.dot(normal), 0);

	Vec3 halfVector = (viewer + lightVector) / 2.0f;
	halfVector.normalize();

	float specularTerm = pow(MAX(halfVector.dot(normal), 0), specularExponent_);
	//if (diffuseTerm < 0.01) std::cout << relfectedLight.dot(viewer) <<  "\t" << pow(relfectedLight.dot(viewer), specularExponent_) << std::endl;
	Colour colour(0, 0, 0);
	colour[0] = clamp(ka_ * ambientColour[0] + kd_ * diffuseColour[0] * diffuseTerm + ks_ * specularColor[0] * specularTerm);
	colour[1] = clamp(ka_ * ambientColour[1] + kd_ * diffuseColour[1] * diffuseTerm + ks_ * specularColor[1] * specularTerm);
	colour[2] = clamp(ka_ * ambientColour[2] + kd_ * diffuseColour[2] * diffuseTerm + ks_ * specularColor[2] * specularTerm);
	return colour;
}

#endif
