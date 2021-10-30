#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "DataTypes.h"
#include "Object.h"
#include <float.h>

namespace raytracer {
	class BoundingBox;
}

class raytracer::BoundingBox {
private:
	// max coordinates
	Vec3 max_;
	// min coordinates
	Vec3 min_;
public:
	// Constructures
	BoundingBox() {}
	BoundingBox(Vec3 min, Vec3 max) : max_(max), min_(min) {}
	~BoundingBox();

	/* Getters/ Setters*/
	Vec3 getMax() { return max_; }
	Vec3 getMin() { return min_; }
	void setMax(Vec3 max) { max_ = max; }
	void setMin(Vec3 min) { min_ = min; }

	/* Checks for intersect */
	inline bool hasIntersect(Ray ray) const;
};

inline bool raytracer::BoundingBox::hasIntersect(Ray ray) const {
	//bool hasIntersect = true;
	float tMax = FLT_MAX;
	float tMin = -FLT_MAX;
	for (int i = 0; i < 3; ++i) {
		if (fabs(ray.getDirection()[i]) < 0.001) {
			if (ray.getOrigin()[i] < min_[i] || ray.getOrigin()[i] > max_[i]) {
				return false;
			}
		}
		else {
			float t0 = fmin((min_[i] - ray.getOrigin()[i]) / ray.getDirection()[i], (max_[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);
			float t1 = fmax((min_[i] - ray.getOrigin()[i]) / ray.getDirection()[i], (max_[i] - ray.getOrigin()[i]) / ray.getDirection()[i]);

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

			//t = (tMin < 0) ? tMax : tMin;

			if (tMin > tMax || tMax < 0) {
				return false;
			}
		}
	}

	return true;
}
#endif
