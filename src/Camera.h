/*
 * CCamera.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <stdexcept>
#include <Eigen/Dense>
using namespace Eigen;

class Camera {
private:

	Vector3d mPosition;
	Vector3d mLookAt;
	double mViewingAngle;

	Vector3d mUp;
	Vector3d mSideways;
	Vector3d mDirection;

public:

	Camera();
	Camera(const Vector3d& position, const Vector3d& look_at, double viewing_angle);
	virtual ~Camera();

    Vector3d getDirection() const;
    Vector3d getLookAt() const;
    Vector3d getPosition() const;
    Vector3d getSideways() const;
    Vector3d getUp() const;
    double getViewingAngle() const;
};

#endif /* CAMERA_H_ */
