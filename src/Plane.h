#ifndef PLANE_H_
#define PLANE_H_

#include "Object.h"
#include <iostream>

namespace raytracer {
	class Plane;
}

class raytracer::Plane : public Object {
private:
	// Default normal vector and point
	const Vec3 DEFAULT_NORMAL = Vec3(0.0, 0.0, 0.0);
	const Vec3 DEFAULT_POINT = Vec3(0.0, 0.0, 0.0);

	// Normal vector to the plane 
	Vec3 normal_;
	// A point on the plane
	Vec3 point_;

public:
	// Constructors
	Plane() : point_(DEFAULT_POINT), normal_(DEFAULT_NORMAL) { id = ++objectCount; }
	Plane(Vec3 point, Vec3 normal) : point_(point), normal_(normal) { id = ++objectCount; }
	~Plane() {}

	/* Getters/Setters */
	Vec3 getPoint() const { return point_; }
	Vec3 getNormal() const { return normal_; }
	void setPoint(Vec3 point) { point_ = point; }
	void setNormal(Vec3 normal) { normal_ = normal; }

	/* Overriden methods from base class */
	Vec3 getNormal(Vec3 const pt) const override { return getNormal(); }
	inline bool hasIntersect(Ray ray, float& t) const override;
	// infinte planes can't have a bounding box so I just return nullptr to check for plane 
	// and i'll exclude them form my BVH.
	virtual BoundingBox* createBoundingBox() const { return nullptr; }
	bool hasBoundingBox() const { return false; }

	Plane operator=(const Plane& b);
};

raytracer::Plane raytracer::Plane::operator=(const Plane& b) {
	this->point_ = b.getPoint();
	this->normal_ = b.getNormal();

	return *this;
}

inline bool raytracer::Plane::hasIntersect(Ray ray, float& t) const {
	// Checks for a division by zero
	bool hasIntersect = ray.getDirection().dot(normal_) != 0;

	// Find intersect if one exist
	if (hasIntersect) {
		t = (1.0f / ray.getDirection().dot(normal_)) * (point_ - ray.getOrigin()).dot(normal_);
	}

	return hasIntersect;

}

#endif
