/*
 * math.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef EXTRA_MATH_H_
#define EXTRA_MATH_H_

#define _USE_MATH_DEFINES

#include <cmath>
#include <limits>
#include <Eigen/Dense>
using namespace Eigen;

const double ZERO_THRESHOLD = (8.0 * std::numeric_limits<double>::epsilon());

inline bool isZero(double x) {
	if ((x < ZERO_THRESHOLD) && (x > -ZERO_THRESHOLD))
		return true;
	else return false;
}

inline bool isEqual(double x, double y) {
	return isZero(x - y);
}

inline bool isEqual(const Vector3d& v1, const Vector3d& v2) {
	double v1_x = v1.dot(Vector3d::UnitX());
	double v1_y = v1.dot(Vector3d::UnitY());
	double v1_z = v1.dot(Vector3d::UnitZ());

	double v2_x = v2.dot(Vector3d::UnitX());
	double v2_y = v2.dot(Vector3d::UnitY());
	double v2_z = v2.dot(Vector3d::UnitZ());

	return isEqual(v1_x, v2_x) && isEqual(v1_y, v2_y) && isEqual(v1_z, v2_z);
}

#endif /* EXTRA_MATH_H_ */
