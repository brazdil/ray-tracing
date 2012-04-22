/*
 * Sphere.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "Renderable.h"

class Sphere: public Renderable {
private:

	Vector3d mCenter;
	double mRadius;
	bool mNormalOutside;
	BoundingBox mBoundingBox;

	virtual vector< pair<const Renderable*, double> > ray_intersections(const Ray& ray) const;
	virtual const BoundingBox& bounding_box() const;
	virtual Vector3d normal(const Vector3d& point_on_surface) const;

	void init();
public:
	Sphere();
	Sphere(Vector3d center, double radius, bool normal_outside);
	virtual ~Sphere();

	virtual pObject translate(const Vector3d& delta) const;
	virtual pObject scale(double factor) const;
	virtual std::string print_debug(unsigned int indent) const;
};

#endif /* SPHERE_H_ */
