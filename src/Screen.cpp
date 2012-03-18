/*
 * ScreenRect.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#include "Screen.h"
#include <iostream>

Screen::Screen(const Camera& camera, double distance, double aspect_ratio)
	: mCamera(camera), mDistance(distance), mAspectRatio(aspect_ratio) {
	if (distance <= 0.0)
		throw std::invalid_argument("Distance has to be positive");
	if (aspect_ratio <= 0.0)
		throw std::invalid_argument("Aspect ratio has to be positive");

	double viewing_angle = camera.getViewingAngle() * 0.5;
	Vector3d direction = camera.getDirection();
	Vector3d screen_center = distance * direction;
	Vector3d left_edge = (distance / cos(viewing_angle)) * (AngleAxis<double>(-viewing_angle, camera.getUp()) * direction);
	Vector3d sideways = left_edge - screen_center;
	Vector3d upwards = camera.getUp() * sideways.norm() / aspect_ratio;

	mTopLeftCorner = left_edge + upwards;
	mHorizontal = -2.0 * sideways;
	mVertical = -2.0 * upwards;
}

Vector3d Screen::getTopLeftCorner() const {
    return mTopLeftCorner;
}

Vector3d Screen::getHorizontal() const {
    return mHorizontal;
}

double Screen::getAspectRatio() const {
    return mAspectRatio;
}

double Screen::getDistance() const {
    return mDistance;
}

Vector3d Screen::getVertical() const {
    return mVertical;
}

Camera Screen::getCamera() const {
	return mCamera;
}

Screen::~Screen() {
}

