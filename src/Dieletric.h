#ifndef DIELETRIC_H_
#define DIELETRIC_H_

#include "DataTypes.h"
#include "Ray.h"
#include "Material.h"
#include <math.h>
#include <iostream>

namespace raytracer {
	class Dieletric;
}

class raytracer::Dieletric : public Material {
public:
	Dieletric() {}
	Dieletric(const Dieletric& m) {
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

	bool getReflectedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray &ray) const override;
};

bool raytracer::Dieletric::getReflectedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray& ray) const {

	Vec3 reflected = -2 * (rayDirection.dot(normal)) * (normal)+rayDirection;
	ray =  Ray(intersection + 0.01 * normal, reflected);

	return (ray.getDirection().dot(normal) > 0.0f);
}

#endif
