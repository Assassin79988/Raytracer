#ifndef QUAD_H_
#define QUAD_H_

#include "Object.h"
#include "Triangle.h"
#include "DataTypes.h"

namespace raytracer {
	class Quad;
}

class raytracer::Quad : public Object {
private:
	// Vertices are define in ccw order
	Vec3 vertex1_, vertex2_, vertex3_, vertex4_;
	// sub triangles
	Triangle t1_, t2_;
public:
	Quad() {}
	// Vertices are define in ccw order
	Quad(Vec3 vertex1, Vec3 vertex2, Vec3 vertex3, Vec3 vertex4) : vertex1_(vertex1), vertex2_(vertex2), vertex3_(vertex3), vertex4_(vertex4), t1_(Triangle(vertex1_, vertex2_, vertex3_)), t2_(Triangle(vertex1_, vertex4_, vertex3_)){}

	Triangle getTriangle1() { return t1_; }
	Triangle getTriangle2() { return t2_; }
	Vec3 getVertex1() { return vertex1_; }
	Vec3 getVertex2() { return vertex2_; }
	Vec3 getVertex3() { return vertex3_; }
	Vec3 getVertex4() { return vertex4_; }

	Vec3 getNormal(Vec3 const pt) const override;
	inline bool hasIntersect(Ray ray, float& t) const override;
	BoundingBox* createBoundingBox() const override;
	bool hasBoundingBox() const override;
};

Vec3 raytracer::Quad::getNormal(Vec3 const pt) const {
	return -(vertex2_ - vertex1_).cross(vertex3_ - vertex1_);
}

inline bool raytracer::Quad::hasIntersect(Ray ray, float& t) const {
	bool hasIntersect = t1_.hasIntersect(ray, t);

	if (!hasIntersect) {
		hasIntersect = t2_.hasIntersect(ray, t);
	}

	return hasIntersect;
}

raytracer::BoundingBox* raytracer::Quad::createBoundingBox() const {
	float xMax = fmax(vertex1_[0], fmax(vertex2_[0], fmax(vertex3_[0], vertex4_[0])));
	float xMin = fmin(vertex1_[0], fmin(vertex2_[0], fmin(vertex3_[0], vertex4_[0])));
	float yMax = fmax(vertex1_[1], fmax(vertex2_[1], fmax(vertex3_[1], vertex4_[1])));
	float yMin = fmin(vertex1_[1], fmin(vertex2_[1], fmin(vertex3_[1], vertex4_[1])));
	float zMax = fmax(vertex1_[2], fmax(vertex2_[2], fmax(vertex3_[2], vertex4_[2])));
	float zMin = fmin(vertex1_[2], fmin(vertex2_[2], fmin(vertex3_[2], vertex4_[2])));
	BoundingBox* boundingBox = new BoundingBox(Vec3(xMin, yMin, zMin), Vec3(xMax, yMax, zMax));
	return boundingBox;
}

bool raytracer::Quad::hasBoundingBox() const {
	return true;
}

#endif