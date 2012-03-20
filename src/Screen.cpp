/*
 * ScreenRect.cpp
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#include "Screen.h"
#include <iostream>

Ratio::Ratio(unsigned int nom, unsigned int den)
	: mNominator(nom), mDenominator(den) {
	if (mDenominator == 0)
		throw std::invalid_argument("Division by zero");
}

Ratio::~Ratio() {

}

double Ratio::getNominator() {
	return mNominator;
}

double Ratio::getDenominator() {
	return mDenominator;
}

double Ratio::getDouble() {
	return (double) mNominator / (double) mDenominator;
}

Screen::Screen(Camera_ptr camera, double distance, Ratio aspect_ratio)
	: mCamera(camera), mDistance(distance), mAspectRatio(aspect_ratio) {
	if (mDistance <= 0.0)
		throw std::invalid_argument("Distance has to be positive");
	if (mAspectRatio.getNominator() == 0)
		throw std::invalid_argument("Can't have zero ratio");

	double viewing_angle = mCamera->getViewingAngle() * 0.5;
	Vector3d direction = mCamera->getDirection();
	Vector3d screen_center = mDistance * direction;
	Vector3d left_edge = (mDistance / cos(viewing_angle)) * (AngleAxis<double>(-viewing_angle, mCamera->getUp()) * direction);
	Vector3d sideways = left_edge - screen_center;
	Vector3d upwards = mCamera->getUp() * sideways.norm() / mAspectRatio.getDouble();

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

Ratio Screen::getAspectRatio() const {
    return mAspectRatio;
}

double Screen::getDistance() const {
    return mDistance;
}

Vector3d Screen::getVertical() const {
    return mVertical;
}

Camera_ptr Screen::getCamera() const {
	return mCamera;
}

Screen::~Screen() {
}

