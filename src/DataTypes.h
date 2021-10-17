#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "opencv2/core/core.hpp"   

typedef float Scalar;
typedef Eigen::Matrix<Scalar, 3, 1> Vec3;
typedef Eigen::Matrix<Scalar, 2, 1> Vec2;
typedef	unsigned char uchar;

using Colour = cv::Vec3b; // RGB Value
static Colour red() { return Colour(255, 0, 0); }
static Colour green() { return Colour(0, 255, 0); }
static Colour blue() { return Colour(0, 0, 255); }
static Colour white() { return Colour(255, 255, 255); }
static Colour black() { return Colour(0, 0, 0); }

#endif
