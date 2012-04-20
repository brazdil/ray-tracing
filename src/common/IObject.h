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
#include <utility>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

using namespace std;

class IObject;
typedef boost::shared_ptr<IObject> pIObject;

class IObject {

	friend class Composite;

	/*
	 * Return list of all intersections between the object and a ray.
	 * Each intersection is represented by the scalar multiplier of ray's direction vector, and the ELEMENTARY object it hit.
	 * Should only return positive intersections!
	 */
	virtual vector< pair<const IObject*, double> > ray_intersections(const Ray& ray) const = 0;

	/*
	 * Return axis-aligned bounding box of the object
	 */
	virtual const BoundingBox& bounding_box() const = 0;

	/*
	 * Return normal (unit) at given point (assumes its on the surface)
	 */
	virtual Vector3d normal(const Vector3d& point_on_surface) const = 0;

public:

	virtual ~IObject();

	class no_intersection_exception : public std::exception {
	};

	/*
	 * Returns first incident elementary object and the point of intersection
	 */
	pair<const IObject*, double> getFirstIntersection(const Ray& ray) const;

	Color getColorAtIntersection(const Ray& ray) const;

	/*
	 * Translate
	 */
	virtual pIObject translate(const Vector3d& delta) const = 0;

	/*
	 * Scale
	 */
	virtual pIObject scale(double factor) const = 0;
};

#endif /* IOBJECT_H_ */
