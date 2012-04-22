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

void NonRenderable::ray_intersections(const Ray &ray, list<IntersectionPair> &result) const {
}

const BoundingBox& NonRenderable::bounding_box() const {
	return BoundingBox::null;
}
