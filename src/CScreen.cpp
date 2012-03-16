/*
 * CScreenRect.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#include "CScreen.h"
#include <iostream>

CScreen::CScreen(const CCamera& camera, double distance, double aspect_ratio)
	: mAspectRatio(aspect_ratio), mDistance(distance), mCamera(camera) {
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

Vector3d CScreen::getTopLeftCorner() const {
    return mTopLeftCorner;
}

Vector3d CScreen::getHorizontal() const {
    return mHorizontal;
}

double CScreen::getAspectRatio() const {
    return mAspectRatio;
}

double CScreen::getDistance() const {
    return mDistance;
}

Vector3d CScreen::getVertical() const {
    return mVertical;
}

CCamera CScreen::getCamera() const {
	return mCamera;
}

CScreen::~CScreen() {
}

