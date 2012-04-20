/*
 * IObject.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "IObject.h"
#include <iostream>
IObject::~IObject() {

}

pair<const IObject*, double> IObject::getFirstIntersection(const Ray& ray) const {
	if (!bounding_box().intersects(ray))
		throw no_intersection_exception();

	vector< pair<const IObject*, double> > intersections = ray_intersections(ray);
	if (intersections.empty())
		throw no_intersection_exception();

	pair<const IObject*, double> minimum = pair<const IObject*, double>(NULL, numeric_limits<double>::infinity());
	for (vector< pair<const IObject*, double> >::iterator it = intersections.begin(); it < intersections.end(); it++) {
		if ((*it).second < minimum.second) {
			minimum = (*it);
		}
	}

	return minimum;
}

Color IObject::getColorAtIntersection(const Ray& ray) const {
	pair<const IObject*, double> intersection = getFirstIntersection(ray); // might throw no_intersection exception, let propagate
	return Color((unsigned char) 012, (unsigned char) 123, (unsigned char) 234);
}
