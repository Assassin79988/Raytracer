#ifndef PLANE_H_
#define PLANE_H_

#include "Object.h"
#include <iostream>

namespace raytracer {
	class Plane;
}

class raytracer::Plane : public Object {
private:
	const Vec3 DEFAULT_NORMAL = Vec3(0.0, 0.0, 0.0);
	const Vec3 DEFAULT_POINT = Vec3(0.0, 0.0, 0.0);

	Vec3 normal_;
	Vec3 point_;

public:
	Plane() : point_(DEFAULT_POINT), normal_(DEFAULT_NORMAL) {}
	Plane(Vec3 point, Vec3 normal) : point_(point), normal_(normal) {}

	Vec3 getPoint() const { return point_; }
	Vec3 getNormal() const { return normal_; }
	/* Parameters there for polymorphism */
	Vec3 getNormal(Vec3 const pt) const override { return getNormal(); }
	inline bool hasIntersect(Ray ray, float& t) const override;

	Plane operator=(const Plane& b) {
		this->point_ = b.getPoint();
		this->normal_ = b.getNormal();

		return *this;
	}
};

inline bool raytracer::Plane::hasIntersect(Ray ray, float& t) const {
	bool hasIntersect = ray.getDirection().dot(normal_) != 0;
	
	if (hasIntersect) {
		t = (1.0f / ray.getDirection().dot(normal_)) * (point_ - ray.getOrigin()).dot(normal_);
	}

	return hasIntersect;

}

#endif
