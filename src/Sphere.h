#ifndef SPHERE_H_
#define SPHERE_H_

#include "Object.h"
#include <iostream>
#include <math.h>

namespace raytracer {
	class Sphere;
}

class raytracer::Sphere : public Object {
private:
	// Default Sphere's Center 
	const Vec3 DEFAULT_CENTER = Vec3(0.0, 0.0, 0.0);
	// Default Sphere's Radius 
	const float DEFAULT_RADIUS = 0.0;

	// Stores the center of the sphere
	Vec3 center_;
	//Stores the radius of the sphere
	float radius_;

	// Checks if the rays origin lies within the sphere or not
	bool isInsideObject(Ray ray) const;
public:
	// Constructures
	Sphere() : center_(DEFAULT_CENTER), radius_(DEFAULT_RADIUS) { id = ++objectCount; }
	Sphere(Vec3 center, float radius) : center_(center), radius_(radius) { id = ++objectCount; }
	// Deconstructures
	~Sphere() {}

	// Getters/Setters
	Vec3 getCenter() const { return center_; }
	float getRadius() const { return radius_; }
	void setCenter(Vec3 center) { center_ = center; }
	void setRadius(float radius) { radius_ = radius; }

	/* Overriden methods from base class */
	Vec3 getNormal(Vec3 const pt) const override;
	inline bool hasIntersect(Ray ray, float& t) const override;
	BoundingBox* createBoundingBox() const override;
	bool hasBoundingBox() const override { return true; }

	// Operator Overload = operator
	Sphere operator=(const Sphere& b);
};

raytracer::Sphere raytracer::Sphere::operator=(const Sphere& b) {
	this->center_ = b.getCenter();
	this->radius_ = b.getRadius();

	return *this;
}

bool raytracer::Sphere::isInsideObject(Ray ray) const{
	// Check if the rat's origin lies within the sphere with the following
	// sqrt((O.x - C.x)^2 + (O.y - C.y)^2 + (O.z - C.z)^2) <= R (lies within or on sphere)
	Vec3 rayOrigin = ray.getOrigin();
	return sqrtf(pow(rayOrigin[0] - center_[0], 2.0f) + pow(rayOrigin[1] - center_[1], 2.0f) + pow(rayOrigin[2] - center_[2], 2.0f)) <= radius_;
}

raytracer::BoundingBox* raytracer::Sphere::createBoundingBox() const {
	Vec3 center = center_;
	Vec3 radius = Vec3(radius_, radius_, radius_);

	// Finds the min/max values on the sphere and creates a bounding box
	Vec3 min = center - radius;
	Vec3 max = center + radius;

	BoundingBox* boundingBox = new BoundingBox(min, max);
	return boundingBox;
}
Vec3 raytracer::Sphere::getNormal(Vec3 const pt) const {
	Vec3 normal = pt - center_;
	normal.normalize();
	return normal;
}

inline bool raytracer::Sphere::hasIntersect(Ray ray, float& t) const {

	// Compute the coefficent in the quadtraic equation At^2 + Bt + C
	float A = pow(ray.getDirection().norm(), 2);
	float B = 2 * (ray.getDirection().dot(ray.getOrigin() - center_));
	float C = pow((ray.getOrigin() - center_).norm(), 2) - pow(radius_, 2);

	// Determines if a intersect exist or not
	float discriminant = pow(B, 2) - 4 * A * C;
	bool hasIntersect = discriminant >= 0;

	// Finds closest intersect if one exists
	if (hasIntersect) {
		float t1 = (-B + sqrt(discriminant)) / (2 * A);
		float t2 = (-B - sqrt(discriminant)) / (2 * A);

		if (isInsideObject(ray)) {
			// returns larger intersect if ray origin lies within Sphere
			t = (t1 > t2) ? t1 : t2;
		}
		else {
			// returns smaller intersect if ray origin lies outside Sohere
			t = (t1 < t2) ? t1 : t2;
		}
	}

	return hasIntersect;
}

#endif
