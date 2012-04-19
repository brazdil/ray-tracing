/*
 * IObject.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "IObject.h"

IObject::~IObject() {

}

Vector3d IObject::getFirstIntersection(Ray& ray) {
	if (!bounding_box().intersects(ray))
		throw no_intersection_exception();

	vector<double> intersections = ray_intersections(ray);
	if (intersections.empty())
		throw no_intersection_exception();

	double minimum = numeric_limits<double>::infinity();
	for (vector<double>::iterator it = intersections.begin(); it < intersections.end(); it++) {
		if ((*it) < minimum)
			minimum = (*it);
	}

	return ray.getPointOnRay(minimum);
}

Color IObject::getColorAtIntersection(Ray& ray) {
	Vector3d intersection = getFirstIntersection(ray);
	return Color((unsigned char) 012, (unsigned char) 123, (unsigned char) 234);
}
