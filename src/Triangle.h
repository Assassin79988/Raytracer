#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Object.h"
#include "config.h"

constexpr auto epsilon = 0.00001f;

namespace raytracer {
	class Triangle;
}

class raytracer::Triangle : public Object {
private:
	Vec3 vertex1_, vertex2_, vertex3_;
    Vec3 vn1_, vn2_, vn3_;
    Vec3 faceNormal_;

    Vec3 computeNormal(const Vec3&, const Vec3&, const Vec3&) const;
    float computeTriangleAera(const Vec3& v1, const Vec3& v2, const Vec3& v3) const;
public:
	Triangle() {}
	Triangle(Vec3 v1, Vec3 v2, Vec3 v3) : vertex1_(v1), vertex2_(v2), vertex3_(v3) {
        faceNormal_ = computeNormal(v1, v2, v3);
        vn1_ = faceNormal_;
        vn2_ = faceNormal_;
        vn3_ = faceNormal_;
	};

    Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 vn1, Vec3 vn2, Vec3 vn3) : vertex1_(v1), vertex2_(v2), vertex3_(v3), vn1_(vn1), vn2_(vn2), vn3_(vn3) {
        faceNormal_ = computeNormal(v1, v2, v3);
    };

    //Vec3 getNormal(Vec3 const pt) const { return normal_; }
    Vec3 getNormal(Vec3 const pt) const;
    Vec3 getFaceNormal() const { return faceNormal_; }
    Vec3 getVn1() const { return vn1_; }
    Vec3 getVn2() const { return vn2_; }
    Vec3 getVn3() const { return vn3_; }
    Vec3 getV1() const { return vertex1_; }
    Vec3 getV2() const { return vertex2_; }
    Vec3 getV3() const { return vertex3_; }
    void setVn1(Vec3 vn1) {
        vn1_[0] = vn1[0];
        vn1_[1] = vn1[1];
        vn1_[2] = vn1[2];
    }
    void setVn2(Vec3 vn2) {
        vn2_[0] = vn2[0];
        vn2_[1] = vn2[1];
        vn2_[2] = vn2[2];
    }
    void setVn3(Vec3 vn3) {
        vn3_[0] = vn3[0];
        vn3_[1] = vn3[1];
        vn3_[2] = vn3[2];
    }

	inline bool hasIntersect(Ray ray, float& t) const;
    Vec3 baryCentric(const Vec3&) const;
    BoundingBox* createBoundingBox() const;
    bool hasBoundingBox() const { return true; }
};

Vec3 raytracer::Triangle::getNormal(Vec3 const pt) const {
    if (PHONG_SHADING) {
        Vec3 barycentric = baryCentric(pt);
        Vec3 normal = barycentric[0] * vn1_ + barycentric[1] * vn2_ + barycentric[2] * vn3_;
        return normal;
    }
    else if (FLAT_SHADING) {
        return faceNormal_;
    }
}

raytracer::BoundingBox* raytracer::Triangle::createBoundingBox() const {
    float xMax = fmax(vertex1_[0], fmax(vertex2_[0], vertex3_[0]));
    float xMin = fmin(vertex1_[0], fmin(vertex2_[0], vertex3_[0]));
    float yMax = fmax(vertex1_[1], fmax(vertex2_[1], vertex3_[1]));
    float yMin = fmin(vertex1_[1], fmin(vertex2_[1], vertex3_[1]));
    float zMax = fmax(vertex1_[2], fmax(vertex2_[2], vertex3_[2]));
    float zMin = fmin(vertex1_[2], fmin(vertex2_[2], vertex3_[2]));
    BoundingBox* boundingBox = new BoundingBox(Vec3(xMin, yMin, zMin), Vec3(xMax, yMax, zMax));
    return boundingBox;
}

inline bool raytracer::Triangle::hasIntersect(Ray ray, float& t) const {
    bool hasIntersect;

    if (ray.getDirection().dot(faceNormal_) != 0) {
        t = -((ray.getOrigin() - vertex1_).dot(faceNormal_)) / ray.getDirection().dot(faceNormal_);
       
        Vec3 barycentric = baryCentric(ray.compute(t));

        bool conditionOne = fabs((barycentric[0] + barycentric[1] + barycentric[2]) - 1) <= epsilon;
        bool conditionTwo = barycentric[0] >= 0 && 1 - barycentric[0] >= epsilon;
        bool conditionThree = barycentric[1] >= 0 && 1 - barycentric[0] >= epsilon;
        bool conditionFour = barycentric[2] >= 0 && 1 - barycentric[0] >= epsilon;
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
    return v13.cross(v12);
}

#endif
