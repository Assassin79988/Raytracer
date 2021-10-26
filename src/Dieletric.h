#ifndef DIELETRIC_H_
#define DIELETRIC_H_

#include "DataTypes.h"
#include "Ray.h"
#include "Material.h"
#include <math.h>
#include <iostream>

namespace raytracer {
	class Dieletric;
}

class raytracer::Dieletric : public Material {
private:
	float ir_;
public:
	Dieletric() {}
};


#endif
