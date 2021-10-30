#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "LightSource.h"

namespace raytracer {
	class PointLight;
}

class raytracer::PointLight : public LightSource {
public:
	PointLight() { position_ = DEFAULT_POSITION; }
	PointLight(Vec3 position) { position_ = position; }
	~PointLight() {}

	// Method to compute the light vector
	Vec3 computeLightVector(Vec3 intersection) const override;
};

Vec3 raytracer::PointLight::computeLightVector(Vec3 intersection) const {
	return position_ - intersection;
}

#endif

