#ifndef OBJECT_H_
#define OBJECT_H_

#include "DataTypes.h"
#include "Ray.h"
#include "Material.h"
#include "BoundingBox.h"
#include <math.h>
#include <iostream>

// Keeps track of the number of objects
int objectCount = 0;

namespace raytracer {
	class Object;
}


class raytracer::Object {
protected:
	// Objects Material
	Material* material_ = new Material();
	// Objects ID
	int id;
public:
	/* Getters / Setters */
	virtual Material* getMaterial() { return material_; }
	virtual void setMaterial(Material* material) { material_ = material; }
	virtual int getID() { return id; }

	// Returns the normal vector at a point pt
	virtual Vec3 getNormal(Vec3 const pt) const = 0;
	// Determine if a ray and the object has a intersection or not
	virtual inline bool hasIntersect(Ray ray, float& t) const = 0;
	// Creates a bouinding box for the object
	virtual BoundingBox* createBoundingBox() const = 0;
	// Returns if a bounding box can be made or not
	virtual bool hasBoundingBox() const = 0;

	// Print the id of the object
	friend std::ostream& operator<<(std::ostream& os, const Object& s) {
		os << "I am Object " << s.id;
		return os;
	}
};

#endif
