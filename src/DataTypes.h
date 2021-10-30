#ifndef DATATYPES_H_
#define DATATYPES_H_

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include "opencv2/core/core.hpp"   

#define M_PI 3.14159265358979323846
#define EPSILON 0.0001f

/* Data type from Eigen */
typedef float Scalar;
typedef Eigen::Matrix<Scalar, 4, 1> Vec4;
typedef Eigen::Matrix<Scalar, 3, 1> Vec3;
typedef Eigen::Matrix<Scalar, 2, 1> Vec2;
typedef Eigen::Matrix<Scalar, 3, 3> Mat3;
typedef Eigen::Matrix<Scalar, 4, 4> Mat4;
typedef	unsigned char uchar;

/* Data types from OpenCV */
using Colour = cv::Vec3b; // BGR Value
static Colour red() { return Colour(0, 0, 255); }
static Colour green() { return Colour(0, 255, 0); }
static Colour blue() { return Colour(255, 0, 0); }
static Colour white() { return Colour(255, 255, 255); }
static Colour black() { return Colour(0, 0, 0); }

#endif
