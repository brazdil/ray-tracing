/*
 * IObject.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "Renderable.h"
#include <iostream>
Renderable::~Renderable() {

}

pair<const Renderable*, double> Renderable::getFirstIntersection(const Ray& ray) const {
	if (!bounding_box().intersects(ray))
		throw no_intersection_exception();

	vector< pair<const Renderable*, double> > intersections = ray_intersections(ray);
	if (intersections.empty())
		throw no_intersection_exception();

	pair<const Renderable*, double> minimum = pair<const Renderable*, double>(NULL, numeric_limits<double>::infinity());
	for (vector< pair<const Renderable*, double> >::iterator it = intersections.begin(); it < intersections.end(); it++) {
		if ((*it).second < minimum.second) {
			minimum = (*it);
		}
	}

	return minimum;
}

Color Renderable::getColorAtIntersection(const Ray& ray) const {
	pair<const Renderable*, double> intersection = getFirstIntersection(ray); // might throw no_intersection exception, let propagate
	return Color((unsigned char) 012, (unsigned char) 123, (unsigned char) 234);
}
