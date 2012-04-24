/*
 * Ray.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "Ray.h"
#include <stdexcept>

Ray::Ray(const Vector3d& origin, const Vector3d& direction)
	: mOrigin(origin), mDirection(direction) {

	mDirection.normalize();

	_x = mOrigin.data()[0];
	_y = mOrigin.data()[1];
	_z = mOrigin.data()[2];

	_i = mDirection.data()[0];
	_j = mDirection.data()[1];
	_k = mDirection.data()[2];

	_R0 = _x * _j - _i * _y;
	_R1 = _x * _k - _i * _z;
	_R3 = _y * _k - _j * _z;

	if(i() < 0) {
		if(j() < 0) {
			if(k() < 0)
				mClassification = MMM;
			else
				mClassification = MMP;
		} else {
			if(k() < 0)
				mClassification = MPM;
			else
				mClassification = MPP;
		}
	} else {
		if(j() < 0) {
			if(k() < 0)
				mClassification = PMM;
			else
				mClassification = PMP;
		} else {
			if(k() < 0)
				mClassification = PPM;
			else
				mClassification = PPP;
		}
	}
}

Ray::~Ray() {
}

double Ray::x() const {
	return _x;
}

double Ray::y() const {
	return _y;
}

double Ray::z() const {
	return _z;
}

double Ray::i() const {
	return _i;
}

double Ray::j() const {
	return _j;
}

double Ray::k() const {
	return _k;
}

double Ray::R0() const {
	return _R0;
}

double Ray::R1() const {
	return _R1;
}

double Ray::R3() const {
	return _R3;
}

const Vector3d& Ray::getOrigin() const {
	return mOrigin;
}

const Vector3d& Ray::getDirection() const {
	return mDirection;
}

Ray::Classification Ray::getClassification() const {
	return mClassification;
}

Vector3d Ray::getPointOnRay(double multiplier) const {
	if (multiplier < 0.0)
		throw std::invalid_argument("Ray: multiplier must be positive");

	return mOrigin + multiplier * mDirection;
}

