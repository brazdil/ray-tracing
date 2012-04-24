/*
 * Sphere.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "SurfaceObject.h"

class Sphere: public SurfaceObject {
private:

	Vector3d mCenter;
	double mRadius;
	bool mNormalOutside;
	BoundingBox mBoundingBox;
	pMaterial mMaterial;

	virtual void ray_intersections(const Ray &ray, list<IntersectionPair> &result) const;
	virtual const BoundingBox& bounding_box() const;

	void init();
public:
	Sphere(pMaterial material);
	Sphere(pMaterial material, Vector3d center, double radius, bool normal_outside);
	virtual ~Sphere();

	virtual pObject translate(const Vector3d& delta) const;
	virtual pObject scale(double factor) const;
	virtual std::string print_debug(unsigned int indent) const;

	virtual Vector3d getNormal(const Vector3d& point_on_surface) const;
	virtual pMaterial getMaterial() const;
};

#endif /* SPHERE_H_ */
