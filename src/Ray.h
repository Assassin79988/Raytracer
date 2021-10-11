#ifndef RAY_H_
#define RAY_H_

#include "DataTypes.h"

namespace raytracer {
	class Ray;
}

class raytracer::Ray {
private:
	/* The origin point of the ray */
	Vec3 origin_;
	/* The direction of the ray */
	Vec3 direction_;
public:
	/* Constructors */
	Ray() {}
	Ray(Vec3 origin, Vec3 direction) : origin_(origin), direction_(direction) {}

	/* Getters and Setters */
	Vec3 getOrigin() { return origin_; }
	Vec3 getDirection() { return direction_; }
	void setOrigin(Vec3 origin) { origin_ = origin; }
	void setDirection(Vec3 direction) { direction_ = direction; }

	/* Finds point on a ray at a distance t from the origin */
	Vec3 compute(float t) { return origin_ + t * direction_; }
};

#endif