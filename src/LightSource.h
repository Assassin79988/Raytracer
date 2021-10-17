#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "DataTypes.h"

namespace raytracer {
	class LightSource;
}

class raytracer::LightSource {
protected:
	const Vec3 DEFAULT_POSITION = Vec3(0.0, 0.0, 0.0);
	Vec3 position_;
public:
	virtual Vec3 getPosition() { return position_; }

	virtual Vec3 computeLightVector(Vec3 intersection) const = 0;
	//virtual Colour getLightColor() = 0;
};

#endif
