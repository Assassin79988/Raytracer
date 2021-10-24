#ifndef METAL_H_
#define METAL_H_

#include "DataTypes.h"
#include "Ray.h"
#include "Material.h"
#include <math.h>
#include <iostream>

namespace raytracer {
	class Metal;
}

class raytracer::Metal : public Material {
private:
	float fuzz = 0.3f;

	Vec3 rabdomPointInUnitSphere() const;
public:
	Metal() {}
	Metal(const Metal& m) {
		this->ks_ = m.ks_;
		this->kd_ = m.kd_;
		this->ka_ = m.ka_;
		this->specularExponent_ = m.specularExponent_;
		this->reflection_ = m.reflection_;
		this->refraction_ = m.refraction_;
		this->ambientColour = m.ambientColour;
		this->diffuseColour = m.diffuseColour;
		this->specularColor = m.specularColor;
		this->fuzz = m.fuzz;

	}
	bool getReflectedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray& ray) const override;
};

Vec3 raytracer::Metal::rabdomPointInUnitSphere() const {
	Vec3 point;
	//std::cout << "Here." << std::endl;
	do {
		float rand1 = (float)rand() / (RAND_MAX + 1.0f);
		float rand2 = (float)rand() / (RAND_MAX + 1.0f);
		float rand3 = (float)rand() / (RAND_MAX + 1.0f);
		//std::cout << rand1 << std::endl;
		point = 2.0f * Vec3(rand1, rand2, rand3) - Vec3(1.0f, 1.0f, 1.0f);
	} while (pow(point.norm(),2) >= 1.0f);
	//point /= 5;
	//std::cout << "Done." << std::endl;
	return point;
}

bool raytracer::Metal::getReflectedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray &ray) const {
	rayDirection.normalize();
	normal.normalize();
	Vec3 reflected = -2 * (rayDirection.dot(normal)) * (normal) + rayDirection;

	ray = Ray(intersection + 0.01 * normal, reflected + fuzz * rabdomPointInUnitSphere());

	return (ray.getDirection().dot(normal) > 0.0f);
}
#endif
