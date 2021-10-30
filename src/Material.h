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
private:
	// specular coefficent 
	float ks_ = 0.7f;
	// diffuse coefficent 
	float kd_ = 0.7f;
	// ambient coefficent
	float ka_ = 0.1f;
	// specular Exponent
	float specularExponent_ = 30;

	// Amount of reflection in final render
	float reflection_ = 0.0f;
	// Fraction to offset the reflection to give a fuzzy look
	float fuzz_ = 0.0f;

	// Amount of refraction in final render
	float refraction_ = 0.0f;
	// index of refraction
	float ir_ = 1.0f;

	// ambient color
	Colour ambientColour = blue();
	// diffuse color
	Colour diffuseColour = Colour(230, 216, 173);

	// Gets a random point in the unit sphere
	Vec3 rabdomPointInUnitSphere() const;
public:
	// Constructors
	Material() {}
	Material(float ks, float ka, float kd, float specularExponent) : ka_(ka), ks_(ks), kd_(kd), specularExponent_(specularExponent) {}
	Material(const Material& m) {
		this->ks_ = m.ks_;
		this->kd_ = m.kd_;
		this->ka_ = m.ka_;
		this->specularExponent_ = m.specularExponent_;
		this->reflection_ = m.reflection_;
		this->refraction_ = m.refraction_;
		this->ambientColour = m.ambientColour;
		this->diffuseColour = m.diffuseColour;

	}

	/* Getters / Setters */
	Colour getAmbientColor() const { return ambientColour; }
	Colour getDiffuseColor() const { return diffuseColour; }
	float getSpecularExp() const { return specularExponent_; }
	float getReflection() const { return reflection_; }
	float getRefraction() const { return refraction_; }
	float getKa() const { return ka_; }
	float getKd() const { return kd_; }
	float getKs() const { return ks_; }

	void setAmbientColor(Colour colour) { ambientColour = colour; }
	void setDiffuseColor(Colour colour) { diffuseColour = colour; }
	void setSpecularExp(float specularExponent) { specularExponent_ = specularExponent; }
	void setReflection(float reflection) { reflection_ = reflection; }
	void setRefraction(float refraction) {refraction_ = refraction; }
	void setKa(float ka) { ka_ = ka; }
	void setKd(float kd) { kd_ = kd; }
	void setKs(float ks) { ks_ = ks; }

	// Finds the reflected ray
	bool getReflectedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray& ray) const;
	// Finds the refracted ray
	bool getRefractedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray& ray) const;
};

Vec3 raytracer::Material::rabdomPointInUnitSphere() const {
	Vec3 point;
	do {
		float rand1 = (float)rand() / (RAND_MAX + 1.0f);
		float rand2 = (float)rand() / (RAND_MAX + 1.0f);
		float rand3 = (float)rand() / (RAND_MAX + 1.0f);
		point = 2.0f * Vec3(rand1, rand2, rand3) - Vec3(1.0f, 1.0f, 1.0f);
	} while (pow(point.norm(), 2) >= 1.0f);
	return point;
}

bool raytracer::Material::getReflectedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray& ray) const {
	// normalized vectors
	rayDirection.normalize();
	normal.normalize();

	// Get reflected vector
	Vec3 reflected = -2 * (rayDirection.dot(normal)) * (normal)+rayDirection;

	// Finds relfected ray
	ray = Ray(intersection + EPSILON * normal, reflected + fuzz_ * rabdomPointInUnitSphere());

	// returns if a reflection occurs or not
	return (ray.getDirection().dot(normal) > 0.0f);
}

bool raytracer::Material::getRefractedRay(Vec3 rayDirection, Vec3 intersection, Vec3 normal, Ray& ray) const {
	// normalized vectors
	rayDirection.normalize();
	normal.normalize();

	// Index of efraction
	float n1 = 1.0f;
	float n2 = ir_;

	// computes cos of the incient angle
	float cosI = rayDirection.dot(normal);

	// determine if the ray is entering or leaving the object
	bool isEntering = cosI < 0;

	if(isEntering) {
		// Enetering Object
		cosI = -cosI;
	}
	else {
		// leaving Object
		std::swap(n1, n2);
		normal = -normal;
	}
	float n = n1 / n2;

	// Find sin squared of the refracted ray's angle
	float sinR2 = pow(n, 2.0f) * (1 - pow(cosI, 2.0f));

	// Checks for Total Interal Refraction
	if (sinR2 > 1.0f) {
		return false;
	}
	
	// finds cos of the refracted ray's angle
	float cosR = sqrtf(1.0f - sinR2);

	// finds the direction of the reflected ray and normalzes
	Vec3 dir = n * rayDirection + (n * cosI - cosR) * normal;
	dir.normalize();

	// computes refracted ray
	Vec3 rayOrigin = intersection - EPSILON * normal;

	ray = Ray(rayOrigin, dir);

	return true;
}

#endif
