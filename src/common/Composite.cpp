/*
 * Union.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "Composite.h"
#include <stdexcept>

Composite::Composite(vector<pIObject> objects)
	: mObjects(objects) {

	if (mObjects.empty())
		throw std::invalid_argument("Composite can't be empty");

	vector<pIObject>::iterator it = mObjects.begin();
	mBoundingBox = (*it)->bounding_box();
	for (; it < mObjects.end(); it++)
		mBoundingBox = mBoundingBox.merge((*it)->bounding_box());
}

Composite::~Composite() {
}

vector< pair<const IObject*, double> > Composite::ray_intersections(const Ray& ray) const {
	vector< vector< pair<const IObject*, double> > > sub_intersections(mObjects.size());
	int total = 0;

	// get all sub_intersections and count how many there are
	for(vector<pIObject>::const_iterator it = mObjects.begin(); it < mObjects.end(); it++) {
		vector< pair<const IObject*, double> > it_intersections = (*it)->ray_intersections(ray);
		sub_intersections.push_back(it_intersections);
		total += it_intersections.size();
	}

	// merge them into one big intersection list
	vector< pair<const IObject*, double> > result;
	result.reserve(total);
	for(vector< vector< pair<const IObject*, double> > >::iterator it = sub_intersections.begin(); it < sub_intersections.end(); it++) {
		for (vector< pair<const IObject*, double> >::iterator it2 = (*it).begin(); it2 < (*it).end(); it2++) {
			result.push_back((*it2));
		}
	}

	return result;
}

const BoundingBox& Composite::bounding_box() const {
	return mBoundingBox;
}

Vector3d Composite::normal(const Vector3d& point_on_surface) const {
	throw std::runtime_error("Union doesn't have a normal");
}

pIObject Composite::translate(const Vector3d& delta) const {
	vector<pIObject> new_sub_objects;
	new_sub_objects.reserve(mObjects.size());

	for (vector<pIObject>::const_iterator it = mObjects.begin(); it < mObjects.end(); it++)
		new_sub_objects.push_back((*it)->translate(delta));

	return pIObject(new Composite(new_sub_objects));
}

pIObject Composite::scale(double factor) const {
	vector<pIObject> new_sub_objects;
	new_sub_objects.reserve(mObjects.size());

	for (vector<pIObject>::const_iterator it = mObjects.begin(); it < mObjects.end(); it++)
		new_sub_objects.push_back((*it)->scale(factor));

	return pIObject(new Composite(new_sub_objects));
}

