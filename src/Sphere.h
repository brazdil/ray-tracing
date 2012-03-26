/*
 * Sphere.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "IObject.h"

class Sphere: public IObject {
private:

	Vector3d mCenter;
	double mRadius;
	bool mNormalOutside;
	BoundingBox mBoundingBox;

	void init();

	virtual vector<double> ray_intersections(Ray& ray);
	virtual BoundingBox& bounding_box();

public:
	Sphere(Vector3d center, double radius);
	virtual ~Sphere();
};

#endif /* SPHERE_H_ */
