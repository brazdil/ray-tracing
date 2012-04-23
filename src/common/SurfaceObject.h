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
private:
	/*
	 * Return normal (unit) at given point (assumes its on the surface)
	 */
	virtual Vector3d normal(const Vector3d& point_on_surface) const = 0;

	/*
	 * Returns the material of the object
	 */
	virtual pMaterial material() const = 0;

public:
	SurfaceObject();
	virtual ~SurfaceObject();
};

#endif /* SURFACEOBJECT_H_ */
