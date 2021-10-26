#ifndef TORUS_H_
#define TORUS_H_

#include <cmath>
#include "Object.h"
#include "DataTypes.h"

namespace raytracer {
	class Torus;
}

class raytracer::Torus : public Object {
private:
	// innerRaduis (cenetr to cenetr of tude) > outerRadius (radius of tube)
	float innerRadius_, outerRadius_;
	Vec3 center_, rotation_;

	int solveQuartic(float A, float B, float C, float D, float E, float* t) const;
	int solveCubic(float A, float B, float C, float D, float* t) const;
public:
	Torus() {}
	Torus(Vec3 center, float innerRadius, float outerRadius) : innerRadius_(innerRadius), outerRadius_(outerRadius), center_(center) {}

	Vec3 getNormal(Vec3 const pt) const override;
	inline bool hasIntersect(Ray ray, float& t) const override;
	BoundingBox* createBoundingBox() const override;
	bool hasBoundingBox() const override;
};

int raytracer::Torus::solveCubic(float A, float B, float C, float D, float* t) const {
	return 0;
}

int raytracer::Torus::solveQuartic(float A, float B, float C, float D, float E, float* t) const {

	return 0;
}

Vec3 raytracer::Torus::getNormal(Vec3 const pt) const {
	return Vec3(0, 0, 0);
}

inline bool raytracer::Torus::hasIntersect(Ray ray, float& t) const {
	// computes the coeffcient for the quartic equation in the form At^4 + Bt^3 + Ct^2 + Dt + E
	Vec3 rayOrigin = ray.getOrigin();
	Vec3 rayDirection = ray.getDirection();

	float A = pow(rayDirection.dot(rayDirection), 2);
	float B = 4 * rayDirection.dot(rayDirection) * rayOrigin.dot(rayDirection);
	float C = 2 * rayDirection.dot(rayDirection) * (rayOrigin.dot(rayOrigin) + (pow(innerRadius_, 2) - pow(outerRadius_, 2))) + 4 * pow(rayOrigin.dot(rayDirection), 2) - 4 * pow(innerRadius_, 2) * (pow(rayDirection[0],2) + pow(rayDirection[1], 2));
	float D = 4 * rayOrigin.dot(rayDirection) * (rayOrigin.dot(rayOrigin) + (pow(innerRadius_, 2) - pow(outerRadius_, 2))) - 8*pow(innerRadius_, 2) * (rayOrigin[0] * rayDirection[0] + rayOrigin[1] * rayDirection[1]);
	float E = pow((rayOrigin.dot(rayOrigin) + (pow(innerRadius_, 2) - pow(outerRadius_, 2))), 2) - 4 * pow(innerRadius_, 2) * (pow(rayOrigin[0], 2) + pow(rayOrigin[1], 2));

	float intersections[4];

	int numRoots = solveQuartic(A, B, C, D, E, intersections);

	t = fmin(intersections[0], fmin(intersections[1], fmin(intersections[2], intersections[3])));

	return false;
}

raytracer::BoundingBox* raytracer::Torus::createBoundingBox() const {
	return new BoundingBox();
}

bool raytracer::Torus::hasBoundingBox() const {
	return true;
}

#endif
