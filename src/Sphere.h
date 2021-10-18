#ifndef SPHERE_H_
#define SPHERE_H_

#include "Object.h"
#include <iostream>

namespace raytracer {
	class Sphere;
}

class raytracer::Sphere : public Object {
private:
	const Vec3 DEFAULT_CENTER = Vec3(0.0, 0.0, 0.0);
	const float DEFAULT_RADIUS = 0.0;

	Vec3 center_;
	float radius_;
public:
	Sphere() : center_(DEFAULT_CENTER), radius_(DEFAULT_RADIUS) {}
	Sphere(Vec3 center, float radius) : center_(center), radius_(radius) {}

	Vec3 getCenter() const { return center_; }
	float getRadius() const { return radius_; }
	void setCenter(Vec3 center) { center_ = center; }
	void setRadius(float radius) { radius_ = radius; }

	Vec3 getNormal(Vec3 const pt) const override;
	inline bool hasIntersect(Ray ray, float& t) const override;
	BoundingBox* createBoundingBox() const;
	bool hasBoundingBox() const { return true; }

	Sphere operator=(const Sphere& b) {
		this->center_ = b.getCenter();
		this->radius_ = b.getRadius();

		return *this;
	}
};
raytracer::BoundingBox* raytracer::Sphere::createBoundingBox() const {
	Vec3 center = center_;
	Vec3 radius = Vec3(radius_, radius_, radius_);

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

		t = (t1 < t2) ? t1 : t2;
	}

	return hasIntersect;
}

#endif
