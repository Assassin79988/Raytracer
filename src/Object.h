#ifndef OBJECT_H_
#define OBJECT_H_

#include "DataTypes.h"
#include "Ray.h"
#include <math.h>

namespace raytracer {
	class Object;
}

class raytracer::Object {
public:
	virtual Vec3 getNormal(Vec3 const pt) const = 0;
	inline virtual bool hasIntersect(Ray ray, float& t) const = 0;
};

#endif
