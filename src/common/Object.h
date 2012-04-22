/*
 * Object.h
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Ray.h"
#include "BoundingBox.h"
#include "Color.h"

#include "utils.h"
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

using namespace std;

class Object;
typedef boost::shared_ptr<Object> pObject;

class SurfaceObject;
typedef pair<const SurfaceObject*, double> IntersectionPair;

class Object {
private:

	friend class Composite;

	/*
	 * Return list of all intersections between the object and a ray.
	 * Each intersection is represented by the scalar multiplier of ray's direction vector, and the ELEMENTARY object it hit.
	 * Should only return positive intersections!
	 */
	virtual vector<IntersectionPair> ray_intersections(const Ray& ray) const = 0;

	/*
	 * Return axis-aligned bounding box of the object
	 */
	virtual const BoundingBox& bounding_box() const = 0;

	/*
	 * Returns first incident SurfaceObject and the point of intersection
	 */
	IntersectionPair first_intersection(const Ray& ray) const;
public:
	Object();
	virtual ~Object();

	class no_intersection_exception : public std::exception { };

	/**
	 * Returns color of object that is hit by a ray, or throws no_intersection_exception.
	 */
	Color getColorAtIntersection(const Ray& ray) const;

	/*
	 * Object passes "this" to the filter function, which should return true/false
	 * whether it should be included in the result. The only difference is Composite,
	 * which aggregates results of its sub-objects.
	 */
	virtual vector<const Object*> filter(bool (*fn)(const Object*)) const;

	/*
	 * Translate
	 */
	virtual pObject translate(const Vector3d& delta) const = 0;

	/*
	 * Scale
	 */
	virtual pObject scale(double factor) const = 0;

	/*
	 * Print out information about the object
	 */
	virtual std::string print_debug(unsigned int indent = 0) const = 0;
};

#endif /* OBJECT_H_ */
