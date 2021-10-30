#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Object.h"
#include "config.h"

namespace raytracer {
	class Triangle;
}

class raytracer::Triangle : public Object {
private:
    // Stores the vertices of the triangle
	Vec3 vertex1_, vertex2_, vertex3_;
    // Stores the normal vectors at each of the vertices
    Vec3 vn1_, vn2_, vn3_;
    // Stroes the face normal
    Vec3 faceNormal_;

    // Computes the face normal of the triangle
    Vec3 computeFaceNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3) const;
    // Computes the area of the triangle defined by three points
    float computeTriangleAera(const Vec3& v1, const Vec3& v2, const Vec3& v3) const;
public:
    // Construtors
	Triangle() { id = ++objectCount; }
    Triangle(Vec3 v1, Vec3 v2, Vec3 v3);
    Triangle(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 vn1, Vec3 vn2, Vec3 vn3) : vertex1_(v1), vertex2_(v2), vertex3_(v3), vn1_(vn1), vn2_(vn2), vn3_(vn3) {
        faceNormal_ = computeFaceNormal(v1, v2, v3);
    };
    // Deconstrutor
    ~Triangle() {}

    // gets the normal vector at a given point
    Vec3 getNormal(Vec3 const pt) const;
    // getter for the face normal
    Vec3 getFaceNormal() const { return faceNormal_; }
    // getter for the first vertex normal
    Vec3 getVn1() const { return vn1_; }
    // getter for the second vertex normal
    Vec3 getVn2() const { return vn2_; }
    // getter for the third vertex normal
    Vec3 getVn3() const { return vn3_; }
    // getter for the first vertex
    Vec3 getV1() const { return vertex1_; }
    // getter for the second vertex
    Vec3 getV2() const { return vertex2_; }
    // getter for the third vertex
    Vec3 getV3() const { return vertex3_; }

    // setter for the first vertex normal
    void setVn1(Vec3 vn1) {
        vn1_[0] = vn1[0];
        vn1_[1] = vn1[1];
        vn1_[2] = vn1[2];
    }
    // setter for the second vertex normal
    void setVn2(Vec3 vn2) {
        vn2_[0] = vn2[0];
        vn2_[1] = vn2[1];
        vn2_[2] = vn2[2];
    }
    // setter for the third vertex normal
    void setVn3(Vec3 vn3) {
        vn3_[0] = vn3[0];
        vn3_[1] = vn3[1];
        vn3_[2] = vn3[2];
    }

    // Returns the baryCentric coordinates for a given point
    Vec3 baryCentric(const Vec3& point) const;

    /* Overriden method from base class */
	inline bool hasIntersect(Ray ray, float& t) const override;
    BoundingBox* createBoundingBox() const override;
    bool hasBoundingBox() const override { return true; }
    Colour getTextureAt(const Vec3& pt) const {
        return black();
    }
};

raytracer::Triangle::Triangle(Vec3 v1, Vec3 v2, Vec3 v3) : vertex1_(v1), vertex2_(v2), vertex3_(v3) {
    // Computes the face normal
    faceNormal_ = computeFaceNormal(v1, v2, v3);

    // Sets vertex normals to face normal until reassigned
    vn1_ = faceNormal_;
    vn2_ = faceNormal_;
    vn3_ = faceNormal_;

    // Gives objects a unique ID
    id = ++objectCount;
};

// Note: function assumes point pt lies on the triangle
Vec3 raytracer::Triangle::getNormal(Vec3 const pt) const {
    if (PHONG_SHADING) {
        // Phong Shading i.e. interpolates the vertex normals to find the normal at pt
        Vec3 barycentric = baryCentric(pt);
        Vec3 normal = barycentric[0] * vn1_ + barycentric[1] * vn2_ + barycentric[2] * vn3_;
        return normal;
    }
    else if (FLAT_SHADING) {
        // Flat Shading i.e. returns face normal for each pt
        return faceNormal_;
    }
}

raytracer::BoundingBox* raytracer::Triangle::createBoundingBox() const {
    // Finds the Max and Min values and creates the AABB
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
    // Checks if ray is perpendicular to the plane containing the triangle
    if (ray.getDirection().dot(faceNormal_) != 0) {
        // Computes the ray-plane intersection with the plane containing the triangle
        t = -((ray.getOrigin() - vertex1_).dot(faceNormal_)) / ray.getDirection().dot(faceNormal_);
       
        // computes the baryCenteric coordinates of the intersection point
        Vec3 barycentric = baryCentric(ray.compute(t));

        // Checks if intersection point lies in triangle or not
        bool conditionOne = fabs((barycentric[0] + barycentric[1] + barycentric[2]) - 1) <= EPSILON;
        bool conditionTwo = barycentric[0] >= 0 && 1 - barycentric[0] >= EPSILON;
        bool conditionThree = barycentric[1] >= 0 && 1 - barycentric[0] >= EPSILON;
        bool conditionFour = barycentric[2] >= 0 && 1 - barycentric[0] >= EPSILON;
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

Vec3 raytracer::Triangle::computeFaceNormal(const Vec3& p1, const Vec3& p2, const Vec3& p3) const {
    Vec3 v12 = p2 - p1;
    Vec3 v13 = p3 - p1;
    return v13.cross(v12);
}

#endif
