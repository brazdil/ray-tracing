/*
 * CCamera.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <boost/shared_ptr.hpp>
#include <stdexcept>
#include "utils.h"
using namespace boost;

class Camera {
private:

	Vector3d mPosition;
	Vector3d mLookAt;
	double mFieldOfView;

	Vector3d mUp;
	Vector3d mSideways;
	Vector3d mDirection;

	void init();

public:

	Camera();
	Camera(const Vector3d& position, const Vector3d& look_at, double field_of_view);
	virtual ~Camera();

    Vector3d getDirection() const;
    Vector3d getLookAt() const;
    Vector3d getPosition() const;
    Vector3d getSideways() const;
    Vector3d getUp() const;
    double getFieldOfView() const;
};

typedef shared_ptr<Camera> pCamera;

#endif /* CAMERA_H_ */
