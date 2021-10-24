#ifndef OBJECT_H_
#define OBJECT_H_

#include "DataTypes.h"
#include "Ray.h"
#include "Material.h"
#include "Metal.h"
#include "Dieletric.h"
#include "BoundingBox.h"
#include <math.h>
#include <iostream>

namespace raytracer {
	class Object;
}

class raytracer::Object {
private:
	Material* material_ = new Dieletric();
public:
	int id;
	virtual Material* getMaterial() { return material_; };
	virtual void setMaterial(Material* material) { material_ = material; }

	virtual Vec3 getNormal(Vec3 const pt) const = 0;
	inline virtual bool hasIntersect(Ray ray, float& t) const = 0;
	virtual BoundingBox* createBoundingBox() const = 0;
	virtual bool hasBoundingBox() const = 0;
	friend std::ostream& operator<<(std::ostream& os, const Object& s) {
		os << "I am Object " << s.id;
		return os;
	}
};

#endif
