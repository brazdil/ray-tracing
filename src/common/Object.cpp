/*
 * Object.cpp
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#include "Object.h"

Object::Object() {
}

Object::~Object() {
}

IntersectionPair Object::first_intersection(const Ray& ray) const {
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

Color Object::getColorAtIntersection(const Ray& ray, const Color& background_color, unsigned int ttl) const {
	if (ttl == 0)
		return background_color;

	try {
		Color result;
		IntersectionPair intersection = first_intersection(ray);



		return result;
	} catch (no_intersection_exception&) {
		return background_color;
	}
}

void Object::filter(list<const Object*>& result, bool (*fn)(const Object*)) const {
	if (fn(this))
		result.push_back(this);
}
