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

	const Vector3d& getOrigin() const;
	const Vector3d& getDirection() const;
	Classification getClassification() const;
	Vector3d getPointOnRay(double multiplier) const;

	// origin
	double x() const;
	double y() const;
	double z() const;

	// direction
	double i() const;
	double j() const;
	double k() const;

	// Plücker coordinates
	double R0() const;
	double R1() const;
	double R3() const;

private:

	Vector3d mOrigin;
	Vector3d mDirection;
	Classification mClassification;

	double _x, _y, _z;
	double _i, _j, _k;
	double _R0, _R1, _R3;

};

#endif /* RAY_H_ */
