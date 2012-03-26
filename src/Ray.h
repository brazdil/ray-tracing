/*
 * Ray.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef RAY_H_
#define RAY_H_

#include "utils.h"

class Ray {
public:

	enum Classification
	{ MMM, MMP, MPM, MPP, PMM, PMP, PPM, PPP };

	Ray(const Vector3d& origin, const Vector3d& direction);
	virtual ~Ray();

	const Vector3d& getOrigin();
	const Vector3d& getDirection();
	Classification getClassification();
	Vector3d getPointOnRay(double multiplier);

	// origin
	double x();
	double y();
	double z();

	// direction
	double i();
	double j();
	double k();

	// Plücker coordinates
	double R0();
	double R1();
	double R3();

private:

	Vector3d mOrigin;
	Vector3d mDirection;
	Classification mClassification;

	double _x, _y, _z;
	double _i, _j, _k;
	double _R0, _R1, _R3;

};

#endif /* RAY_H_ */
