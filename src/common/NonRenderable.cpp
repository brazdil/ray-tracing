/*
 * IObject.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "NonRenderable.h"
#include <stdexcept>

NonRenderable::NonRenderable() {

}

NonRenderable::~NonRenderable() {

}

vector<IntersectionPair> NonRenderable::ray_intersections(const Ray& ray) const {
	return vector<IntersectionPair>();
}

const BoundingBox& NonRenderable::bounding_box() const {
	return BoundingBox::null;
}
