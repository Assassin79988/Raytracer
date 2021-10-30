#ifndef CONE_H_
#define CONE_H_

#include "DataTypes.h"
#include "Object.h"


namespace raytracer {
	class Cone;
}

class raytracer::Cone : public Object {
private:
	Vec3 increasingRadius_, center_;
	float angle_, heightMax_;
public:
	// Constructors
	Cone() { id = ++objectCount; }
	Cone(Vec3 increasingRadius, Vec3 center, float height, float angle) : increasingRadius_(increasingRadius), center_(center), heightMax_(height), angle_(angle) { id = ++objectCount; }
	~Cone() {}

	/* Overriden methods from base class */
	Vec3 getNormal(Vec3 const pt) const override;
	inline bool hasIntersect(Ray ray, float& t) const override;
	BoundingBox* createBoundingBox() const override;
	bool hasBoundingBox() const override;
};

Vec3 raytracer::Cone::getNormal(Vec3 const pt) const {
	Vec3 normal = (pt - center_) * increasingRadius_.dot(pt - center_) / (pt - center_).dot(pt - center_) - increasingRadius_;
	normal.normalize();
	return normal;
}

inline bool raytracer::Cone::hasIntersect(Ray ray, float& t) const {
	Vec3 rayOrigin = ray.getOrigin();
	Vec3 rayDir = ray.getDirection();

	// Compute the coefficent in the quadtraic equation At^2 + Bt + C
	float A = pow(rayDir.dot(increasingRadius_), 2) - pow(cos(angle_), 2);
	float B = 2.0f * (rayDir.dot(increasingRadius_) * (rayOrigin - center_).dot(increasingRadius_) - rayDir.dot(rayOrigin - center_) * pow(cos(angle_), 2));
	float C =pow((rayOrigin - center_).dot(increasingRadius_), 2) - (rayOrigin - center_).dot(rayOrigin - center_) * pow(cos(angle_), 2);
	
	// Determines if a intersect exist or not
	float discriminant = pow(B, 2) - 4 * A * C;
	bool hasIntersect = discriminant >= 0;

	// Finds closest intersect if one exists
	if (hasIntersect) {
		float t1 = (-B + sqrt(discriminant)) / (2 * A);
		float t2 = (-B - sqrt(discriminant)) / (2 * A);

		t = (t1 < t2) ? t1 : t2;

		Vec3 pt = rayOrigin + t * rayDir;
		float height = (pt - center_).dot(increasingRadius_);

		// Rejects intersects that are apart of the second cone or exceed the max height
		if (height < 0.0f || height > heightMax_) {
			hasIntersect = false;
		}
	}

	return hasIntersect;
}

/* Never had time to figure out how to get the bounding box for a Cone */
raytracer::BoundingBox* raytracer::Cone::createBoundingBox() const {
	return nullptr;
}

bool raytracer::Cone::hasBoundingBox() const {
	return false;
}

#endif
