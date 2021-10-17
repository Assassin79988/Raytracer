#ifndef CAMERA_H_
#define CAMERA_H_

#include "DataTypes.h"
#include "Ray.h"

namespace raytracer {
	class Camera;
}

class raytracer::Camera {
private:
	const Vec3 DEFAULT_POSITION = Vec3(0.0, 0.0, 0.0);
	// TODO: Rotation
	Vec3 position_;
	Vec3 rotation_;
public:
	Camera() : position_(DEFAULT_POSITION) {}
	Camera(Vec3 position) : position_(position) {}

	Vec3 getPosition() { return position_; }
	void setPosition(Vec3 position) { position_ = position; }

	inline Ray generateRay(Vec3 const& pt);
};

inline raytracer::Ray raytracer::Camera::generateRay(Vec3 const& pt) {
	Vec3 origin = position_;
	Vec3 direction = pt - origin;
	direction.normalize();

	return Ray(origin, direction);
}

#endif

