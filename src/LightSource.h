#ifndef LIGHTSOURCE_H_
#define LIGHTSOURCE_H_

#include "DataTypes.h"

namespace raytracer {
	class LightSource;
}

class raytracer::LightSource {
protected:
	// default light position
	const Vec3 DEFAULT_POSITION = Vec3(0.0, 0.0, 0.0);
	// lights position
	Vec3 position_;
	// light's color
	Colour colour_ = white();
public:
	/*  Getters / Setters */
	virtual Vec3 getPosition() { return position_; }
	virtual void setPosition(Vec3 position) { position_ = position; }
	virtual Colour getColour() { return colour_; }
	virtual void setColour(Colour colour) { colour_ = colour; }

	// Function to find the light vector
	virtual Vec3 computeLightVector(Vec3 intersection) const = 0;
};

#endif
