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

double Ray::x() {
	return _x;
}

double Ray::y() {
	return _y;
}

double Ray::z() {
	return _z;
}

double Ray::i() {
	return _i;
}

double Ray::j() {
	return _j;
}

double Ray::k() {
	return _k;
}

double Ray::R0() {
	return _R0;
}

double Ray::R1() {
	return _R1;
}

double Ray::R3() {
	return _R3;
}

const Vector3d& Ray::getOrigin() {
	return mOrigin;
}

const Vector3d& Ray::getDirection() {
	return mDirection;
}

Ray::Classification Ray::getClassification() {
	return mClassification;
}

Vector3d Ray::getPointOnRay(double multiplier) {
	if (multiplier < 0.0)
		throw std::invalid_argument("Ray: multiplier must be positive");

	return mOrigin + multiplier * mDirection;
}

