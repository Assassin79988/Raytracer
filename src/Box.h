#ifndef BOX_H_
#define BOX_H_

#include "Datatypes.h"
#include "Object.h"
#include "BoundingBox.h"

namespace raytracer {
	class Box;
}

class raytracer::Box : public Object {
private:
	// upper right conner
	Vec3 urc_;
	// lower left conner
	Vec3 llc_;
public:
	// Constructures
	Box() { id = ++objectCount; }
	Box(Vec3 llc, Vec3 urc) : llc_(llc), urc_(urc) { id = ++objectCount; }
	~Box() {}

	/* Overriden methods from base class */
	Vec3 getNormal(Vec3 const pt) const override;
	inline  bool hasIntersect(Ray ray, float& t) const override;
	BoundingBox* createBoundingBox() const override { return new BoundingBox(llc_, urc_); }
	bool hasBoundingBox() const override { return true; };
};

Vec3 raytracer::Box::getNormal(Vec3 const pt) const {

	if (fabs(pt[0] - llc_[0]) < 0.001) {
		return Vec3(-1.0, 0.0, 0.0);
	}
	else if (fabs(pt[0] - urc_[0]) < 0.001) {
		return Vec3(1.0, 0.0, 0.0);
	}
	else if (fabs(pt[1] - llc_[1]) < 0.001) {
		return Vec3(0.0, -1.0, 0.0);
	}
	else if (fabs(pt[1] - urc_[1]) < 0.001) {
		return Vec3(0.0, 1.0, 0.0);
	}
	else if (fabs(pt[2] - llc_[2]) < 0.001) {
		return Vec3(0.0, 0.0, -1.0);
	}
	else if (fabs(pt[2] - urc_[2]) < 0.001) {
		return Vec3(0.0, 0.0, 1.0);
	}
	return Vec3(0.0, 0.0, 0.0);
}

inline bool raytracer::Box::hasIntersect(Ray ray, float& t) const {
	float tMax = FLT_MAX;
	float tMin = -FLT_MAX;
	for (int i = 0; i < 3; ++i) {
		if (fabs(ray.getDirection()[i]) < 0.001) {
			if (ray.getOrigin()[i] < llc_[i] || ray.getOrigin()[i] > urc_[i]) {
				return false;
			}
		}
		else {
			float t0 = fmin((llc_[i] - ray.getOrigin()[i]) / ray.getDirection()[i], (urc_[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);
			float t1 = fmax((llc_[i] - ray.getOrigin()[i]) / ray.getDirection()[i], (urc_[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);

			if (t0 > t1) {
				float temp = t0;
				t0 = t1;
				t1 = temp;
			}
			if (t0 > tMin) {
				tMin = t0;
			}
			if (t1 < tMax) {
				tMax = t1;
			}

			if (tMin > tMax || tMax < 0) {
				return false;
			}
		}
	}

	t = tMin;

	return true;
}

#endif
