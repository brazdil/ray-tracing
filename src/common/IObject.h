/*
 * IObject.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef IOBJECT_H_
#define IOBJECT_H_

#include "Ray.h"
#include "BoundingBox.h"
#include "Color.h"

#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

using namespace std;

class IObject {

	friend class Composite;

	/*
	 * Returns list of all intersections between the object and a ray.
	 * Each intersection is represented by the scalar multiplier of ray's direction vector.
	 * Should only return positive intersections!
	 */
	virtual vector<double> ray_intersections(Ray& ray) = 0;
	virtual BoundingBox& bounding_box() = 0;

public:

	virtual ~IObject();

	class no_intersection_exception : public std::exception {
	};

	Vector3d getFirstIntersection(Ray& ray);
	Color getColorAtIntersection(Ray& ray);
};

typedef boost::shared_ptr<IObject> pIObject;

#endif /* IOBJECT_H_ */
