#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Object.h"

constexpr auto epsilon = 0.0001f;

namespace raytracer {
	class Triangle;
}

class raytracer::Triangle : public Object {
private:
	Vec3 vertex1_, vertex2_, vertex3_;
    Vec3 normal_;

    Vec3 computeNormal(const Vec3&, const Vec3&, const Vec3&) const;
    float computeTriangleAera(const Vec3& v1, const Vec3& v2, const Vec3& v3) const;
public:
	Triangle() {}
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3) : vertex1_(v1), vertex2_(v2), vertex3_(v3) {
		normal_ = computeNormal(v1, v2, v3);
	};

    Vec3 getNormal(Vec3 const pt) const { return normal_; }
	inline bool hasIntersect(Ray ray, float& t) const;
    Vec3 baryCentric(const Vec3&) const;
};

inline bool raytracer::Triangle::hasIntersect(Ray ray, float& t) const {
    bool hasIntersect;

    if (ray.getDirection().dot(normal_) != 0) {
        t = -((ray.getOrigin() - vertex1_).dot(normal_)) / ray.getDirection().dot(normal_);
       
        Vec3 barycentric = baryCentric(ray.compute(t));

        bool conditionOne = abs((barycentric[0] + barycentric[1] + barycentric[2]) - 1) <= epsilon;
        bool conditionTwo = barycentric[0] >= epsilon && 1 - barycentric[0] >= epsilon;
        bool conditionThree = barycentric[1] >= epsilon && 1 - barycentric[0] >= epsilon;
        bool conditionFour = barycentric[2] >= epsilon && 1 - barycentric[0] >= epsilon;
        hasIntersect = conditionOne && conditionTwo && conditionThree && conditionFour;
    }
    else {
        hasIntersect = false;
    }

    return hasIntersect;
}

float raytracer::Triangle::computeTriangleAera(const Vec3& v1, const Vec3& v2, const Vec3& v3) const {
    return (((v1 - v2).cross(v3 - v2)).norm()) / 2.0f;
}

Vec3 raytracer::Triangle::baryCentric(const Vec3& point) const {
    // compuite the area of the various triangles
    // p1 = vertice 1, p2 = vertice 2, p3 = vertice 3 and x = point
    float p1p2p3 = computeTriangleAera(vertex1_, vertex2_, vertex3_);
    float p2p3x = computeTriangleAera(vertex2_, vertex3_, point);
    float p1p3x = computeTriangleAera(vertex1_, vertex3_, point);
    float p1p2x = computeTriangleAera(vertex1_, vertex2_, point);

    // computes the coefficents
    float alpha = p2p3x / p1p2p3;
    float beta = p1p3x / p1p2p3;
    float gamma = p1p2x / p1p2p3;

    return Vec3(alpha, beta, gamma);
}

Vec3 raytracer::Triangle::computeNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3) const {
    Vec3 v12 = p2 - p1;
    Vec3 v13 = p3 - p1;
    return v12.cross(v13);
}

#endif
