#ifndef IMAGEPLANE_H_
#define IMAGEPLANE_H_

#include "DataTypes.h"

namespace raytracer {
	class ImagePlane;
}

class raytracer::ImagePlane {
private:
	// Lower left conor of the image in world coordinates
	Vec3 llc_;
	// Upper right conor of the image in world coordinates 
	Vec3 urc_;
	// Number of pixels in the y Direction
	float xCols_;
	// Number of pixels in the x Direction
	float yRows_;

public:
	ImagePlane() {};
	ImagePlane(int cols, int rows, Vec3 llc, Vec3 urc) : xCols_(cols), yRows_(rows), llc_(llc), urc_(urc) {}

	// Finds the pixel position in world space
	inline Vec3 generatePixelPos(float i, float j);

};

inline Vec3 raytracer::ImagePlane::generatePixelPos(float i, float j) {
	// FInds the x coordinate of the pixel
	float u = llc_[0] + (urc_(0) - llc_(0)) * i / xCols_;
	// Finds the y coordinate of the pixel
	float v = llc_[1] + (urc_(1) - llc_(1)) * j / yRows_;
	return Vec3(u, v, -1);
}

#endif
