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

	vector<IntersectionPair> intersections = ray_intersections(ray);
	if (intersections.empty())
		throw no_intersection_exception();

	IntersectionPair minimum = IntersectionPair(NULL, numeric_limits<double>::infinity());
	BOOST_FOREACH(IntersectionPair intersection, intersections)
		if (intersection.second < minimum.second)
			minimum = intersection;

	return minimum;
}

Color Object::getColorAtIntersection(const Ray& ray) const {
	IntersectionPair intersection = first_intersection(ray); // might throw no_intersection exception, let propagate
	return Color((unsigned char) 012, (unsigned char) 123, (unsigned char) 234);
}

vector<const Object*> Object::filter(bool (*fn)(const Object*)) const {
	vector<const Object*> result;
	if (fn(this)) result.push_back(this);
	return result;
}
