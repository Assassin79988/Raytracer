#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Ray.h"
#include "DataTypes.h"
#include "Camera.h"

namespace raytracer {
	class PerspectiveCamera;
}

class raytracer::PerspectiveCamera : public Camera {
public:
	// Constructors
	PerspectiveCamera();
	PerspectiveCamera(Vec3 position, Vec3 point, Vec3 up);
	~PerspectiveCamera() {}

	// Generate a ray at a given pixel on the image plane
	inline Ray generateRay(Vec3 const& pt) const override;
};

raytracer::PerspectiveCamera::PerspectiveCamera() {

}

raytracer::PerspectiveCamera::PerspectiveCamera(Vec3 position, Vec3 point, Vec3 up) {
	position_ = position; 

	direction_ = point - position_;
	direction_.normalize();

	right_ = up.cross(direction_);
	right_.normalize();

	up_ = right_.cross(direction_);
	up_.normalize();
}

inline raytracer::Ray raytracer::PerspectiveCamera::generateRay(Vec3 const& pt) const {
	Vec3 origin = position_;
	Vec3 direction = direction_ + pt[0] * right_ + pt[1] * up_;
	direction.normalize();
	
	return Ray(origin, direction);
}

#endif
