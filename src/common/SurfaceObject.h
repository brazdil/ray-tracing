/*
 * SurfaceObject.h
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#ifndef SURFACEOBJECT_H_
#define SURFACEOBJECT_H_

#include "Object.h"

class SurfaceObject: public Object {
protected:
	void insert_intersection(IntersectionPair intersection, list<IntersectionPair> &result_list) const;
public:
	SurfaceObject();
	virtual ~SurfaceObject();

	/*
	 * Return normal (unit) at given point (assumes its on the surface)
	 */
	virtual Vector3d getNormal(const Vector3d& point_on_surface) const = 0;

	/*
	 * Returns the material of the object
	 */
	virtual pMaterial getMaterial() const = 0;
};

#endif /* SURFACEOBJECT_H_ */
