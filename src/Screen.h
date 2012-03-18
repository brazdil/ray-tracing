/*
 * CScreenRect.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdexcept>
#include <Eigen/Dense>
using namespace Eigen;

#include "Camera.h"

/*
 * CScreen represents the screen rectangle. It contains the position of the top-left corner of the screen
 * in coordinates *relative* to the camera position, and a vector along its horizontal edge with length equivalent
 * to the width of the screen, and a vector along its vertical edge with length equivalent to the height of the screen.
 */
class Screen {
private:

	Camera mCamera;

	double mDistance;
	double mAspectRatio;
	Vector3d mTopLeftCorner;
	Vector3d mHorizontal;
	Vector3d mVertical;

public:

	Screen(const Camera& camera, double distance, double aspect_ratio);
	virtual ~Screen();

	Camera getCamera() const;
    Vector3d getTopLeftCorner() const;
    Vector3d getHorizontal() const;
    Vector3d getVertical() const;
    double getAspectRatio() const;
    double getDistance() const;
};

#endif /* SCREEN_H_ */
