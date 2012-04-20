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
	const BoundingBox mBoundingBox;

	virtual vector< pair<const IObject*, double> > ray_intersections(const Ray& ray) const;
	virtual const BoundingBox& bounding_box() const;
	virtual Vector3d normal(const Vector3d& point_on_surface) const;

public:
	Sphere();
	virtual ~Sphere();
};

#endif /* SPHERE_H_ */
