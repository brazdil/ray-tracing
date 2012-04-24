/*
 * SurfaceObject.cpp
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#include "SurfaceObject.h"

SurfaceObject::SurfaceObject() {
}

SurfaceObject::~SurfaceObject() {
}

void SurfaceObject::insert_intersection(IntersectionPair intersection, list<IntersectionPair> &result_list) const {
	if (!isZero(intersection.second) && intersection.second > 0.0)
		result_list.push_back(intersection);
}
