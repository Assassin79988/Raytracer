#include "Material.h"

/*
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
*/