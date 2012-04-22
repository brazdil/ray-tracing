/*
 * IObject.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "Object.h"
#include "Ray.h"
#include "BoundingBox.h"
#include "Color.h"

#include <vector>
#include <utility>
#include <stdexcept>

using namespace std;

class Renderable;
typedef boost::shared_ptr<Renderable> pRenderable;

typedef pair<const Renderable*, double> IntersectionPair;

class Renderable : public Object {

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
	 * Return normal (unit) at given point (assumes its on the surface)
	 */
	virtual Vector3d normal(const Vector3d& point_on_surface) const = 0;

public:

	virtual ~Renderable();

	class no_intersection_exception : public std::exception { };
	class not_renderable_exception : public std::exception { };

	/*
	 * Returns first incident elementary object and the point of intersection
	 */
	pair<const Renderable*, double> getFirstIntersection(const Ray& ray) const;

	Color getColorAtIntersection(const Ray& ray) const;
};

#endif /* RENDERABLE_H_ */
