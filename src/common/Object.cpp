/*
 * Object.cpp
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#include "Object.h"
#include <limits>

Object::Object() {
}

Object::~Object() {
}

IntersectionPair Object::getFirstIntersection(const Ray& ray) const {
	if (!bounding_box().intersects(ray))
		throw no_intersection_exception();

	list<IntersectionPair> intersections;
	ray_intersections(ray, intersections);
	if (intersections.empty())
		throw no_intersection_exception();

	IntersectionPair minimum = IntersectionPair(NULL, numeric_limits<double>::infinity());
	BOOST_FOREACH(IntersectionPair intersection, intersections)
		if (intersection.second < minimum.second)
			minimum = intersection;

	return minimum;
}

bool Object::isIntersected(const Vector3d& v1, const Vector3d& v2) const {
	// use ray from v1 to v2

	Vector3d line = v2 - v1;
	Ray ray(v1, line);
	double line_length = line.dot(ray.getDirection()); // direction is normalized

	try {
		return isStrictlyLessThan(getFirstIntersection(ray).second, line_length);
	} catch (no_intersection_exception&) {
		return false;
	}
}

void Object::filter(list<const Object*>& result, bool (*fn)(const Object*)) const {
	if (fn(this))
		result.push_back(this);
}
