/*
 * CScreenRect.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include <Eigen/Dense>

using namespace Eigen;
using namespace boost;

#include "Camera.h"
#include "Screen.h"
#include "Ratio.h"
#include "Color.h"

/*
 * CScreen represents the screen rectangle. It contains the position of the top-left corner of the screen
 * in coordinates *relative* to the camera position, and a vector along its horizontal edge with length equivalent
 * to the width of the screen, and a vector along its vertical edge with length equivalent to the height of the screen.
 */
class Screen {
private:

	pCamera mCamera;

	double mDistance;
	Ratio mAspectRatio;
	Color mBackgroundColor;

	Vector3d mTopLeftCorner;
	Vector3d mHorizontal;
	Vector3d mVertical;

	void init();

public:

	Screen(pCamera camera, double distance, Ratio aspect_ratio, Color background_color);
	virtual ~Screen();

	pCamera getCamera() const;
    Vector3d getTopLeftCorner() const;
    Vector3d getHorizontal() const;
    Vector3d getVertical() const;
    Ratio getAspectRatio() const;
    double getDistance() const;
};

typedef shared_ptr<Screen> pScreen;

#endif /* SCREEN_H_ */
