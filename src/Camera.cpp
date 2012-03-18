/*
 * Camera.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#include "Camera.h"

#include <iostream>
#include "extra_math.h"

Camera::Camera() {

}

Camera::Camera(const Vector3d& position, const Vector3d& look_at, double viewing_angle)
	: mPosition(position), mLookAt(look_at), mViewingAngle(viewing_angle) {

	if ((viewing_angle <= 0.0) || (viewing_angle >= M_PI))
		throw std::invalid_argument("Viewing angle has to be between 0 and pi");

	mDirection = mLookAt - mPosition;
	mDirection.normalize();

	// The up vector is determined by computing how a z-pos vector must have been rotated
	// to align with the direction vector.

	// (i) Rotate the direction vector into the yz plane

	double direction_x = mDirection.dot(Vector3d::UnitX());
	double direction_z = mDirection.dot(Vector3d::UnitZ());

	double angle_y = 0.0;
	if (isZero(direction_x)) {
		if (isZero(direction_z))
			throw std::invalid_argument("Can't have camera looking straight up or down");
		else if (direction_z > 0.0)
			angle_y = M_PI;
		else
			angle_y = -M_PI;
	} else
		angle_y = atan(direction_x / direction_z);

	Vector3d direction_yz = AngleAxis<double>(-angle_y, Vector3d::UnitY()) * mDirection;

	// (ii) Align the vector in yz plane with z axis

	double direction_yz_x = direction_yz.dot(Vector3d::UnitX());
	double direction_yz_y = direction_yz.dot(Vector3d::UnitY());
	double direction_yz_z = direction_yz.dot(Vector3d::UnitZ());

	if (!isZero(direction_yz_x))
		throw std::runtime_error("Failed to rotate the direction vector into yz plane");
	else if (isZero(direction_yz_z))
		throw std::invalid_argument("Can't have camera looking straight up or down");

	double angle_x = -atan(direction_yz_y / direction_yz_z);

	// (iii) Rotate the y-pos vector by the same amount the direction vector had to rotate from z-pos.

	mUp = AngleAxis<double>(angle_y, Vector3d::UnitY()) *
			(AngleAxis<double>(angle_x, Vector3d::UnitX()) *
				Vector3d::UnitY());
	mUp.normalize();

	// Compute the sideways vector
	mSideways = mUp.cross(mDirection);
	mSideways.normalize();
}

Vector3d Camera::getDirection() const {
    return mDirection;
}

Vector3d Camera::getLookAt() const {
    return mLookAt;
}

Vector3d Camera::getPosition() const {
    return mPosition;
}

Vector3d Camera::getSideways() const {
    return mSideways;
}

Vector3d Camera::getUp() const {
    return mUp;
}

double Camera::getViewingAngle() const
{
    return mViewingAngle;
}

Camera::~Camera() {
}

