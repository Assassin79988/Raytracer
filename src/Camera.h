#ifndef CAMERA_H_
#define CAMERA_H_

#include "DataTypes.h"
#include "Ray.h"

namespace raytracer {
	class Camera;
}

class raytracer::Camera {
protected:
	// default position
	const Vec3 DEFAULT_POSITION = Vec3(0.0, 0.0, 0.0);
	// Camera position, direction, up vector and right vector
	Vec3 position_, direction_, up_, right_;
public:
	/* Getters and Setters */
	Vec3 getPosition() { return position_; }
	void setPosition(Vec3 position) { position_ = position; }
	Vec3 getDirection() { return direction_; }
	void setDirection(Vec3 direction) { direction_ = direction; }
	Vec3 getUp() { return up_; }
	void setUp(Vec3 up) { up_ = up; }
	Vec3 getRight() { return right_; }
	void setRight(Vec3 right) { right_ = right; }

	/* Generate Rays from the camera to the image plane */
	virtual inline Ray generateRay(Vec3 const& pt) const = 0;
};

#endif

