/*
 * Union.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "Composite.h"
#include <stdexcept>

Composite::Composite(vector<pRenderable> renderable_objects, vector<pObject> associated_objects)
	: mRenderableObjects(renderable_objects), mAssociatedObjects(associated_objects) {

	if (mRenderableObjects.empty() && mAssociatedObjects.empty())
		throw std::invalid_argument("Composite can't be empty");

	if (!mRenderableObjects.empty()) {
		bool first = true;
		for(vector<pRenderable>::const_iterator it = mRenderableObjects.begin(); it < mRenderableObjects.end(); it++)
			try {
				if (first) {
					mBoundingBox = (*it)->bounding_box();
					first = false;
				} else
					mBoundingBox = mBoundingBox.merge((*it)->bounding_box());
			} catch (not_renderable_exception&) { }
	}
}

Composite::~Composite() {
}

vector< pair<const Renderable*, double> > Composite::ray_intersections(const Ray& ray) const {
	if (mRenderableObjects.empty())
		return vector< pair<const Renderable*, double> >();

	vector< vector< pair<const Renderable*, double> > > sub_intersections(mRenderableObjects.size());
	int total = 0;

	// get all sub_intersections and count how many there are
	for(vector<pRenderable>::const_iterator it = mRenderableObjects.begin(); it < mRenderableObjects.end(); it++) {
		vector< pair<const Renderable*, double> > it_intersections = (*it)->ray_intersections(ray);
		sub_intersections.push_back(it_intersections);
		total += it_intersections.size();
	}

	// merge them into one big intersection list
	vector< pair<const Renderable*, double> > result;
	result.reserve(total);
	for(vector< vector< pair<const Renderable*, double> > >::iterator it = sub_intersections.begin(); it < sub_intersections.end(); it++) {
		for (vector< pair<const Renderable*, double> >::iterator it2 = (*it).begin(); it2 < (*it).end(); it2++) {
			result.push_back((*it2));
		}
	}

	return result;
}

const BoundingBox& Composite::bounding_box() const {
	if (mRenderableObjects.empty())
		throw not_renderable_exception();
	return mBoundingBox;
}

Vector3d Composite::normal(const Vector3d& point_on_surface) const {
	throw std::runtime_error("Union doesn't have a normal");
}

pObject Composite::translate(const Vector3d& delta) const {
	vector<pRenderable> new_renderable_objects;
	vector<pObject> new_associated_objects;
	new_renderable_objects.reserve(mRenderableObjects.size());
	new_associated_objects.reserve(mAssociatedObjects.size());

	for (vector<pRenderable>::const_iterator it = mRenderableObjects.begin(); it < mRenderableObjects.end(); it++)
		new_renderable_objects.push_back(boost::dynamic_pointer_cast<Renderable>((*it)->translate(delta)));
	for (vector<pObject>::const_iterator it = mAssociatedObjects.begin(); it < mAssociatedObjects.end(); it++)
		new_associated_objects.push_back((*it)->translate(delta));

	return pObject(new Composite(new_renderable_objects, new_associated_objects));
}

pObject Composite::scale(double factor) const {
	vector<pRenderable> new_renderable_objects;
	vector<pObject> new_associated_objects;
	new_renderable_objects.reserve(mRenderableObjects.size());
	new_associated_objects.reserve(mAssociatedObjects.size());

	for (vector<pRenderable>::const_iterator it = mRenderableObjects.begin(); it < mRenderableObjects.end(); it++)
		new_renderable_objects.push_back(boost::dynamic_pointer_cast<Renderable>((*it)->scale(factor)));
	for (vector<pObject>::const_iterator it = mAssociatedObjects.begin(); it < mAssociatedObjects.end(); it++)
		new_associated_objects.push_back((*it)->scale(factor));

	return pObject(new Composite(new_renderable_objects, new_associated_objects));
}

std::string Composite::print_debug(unsigned int indent) const {
	std::ostringstream output;
	for (int i = 0; i < indent; ++i)
		output << " ";

	output << "Composite:"
		   << " renderable=" << mRenderableObjects.size()
		   << " associated=" << mAssociatedObjects.size()
		   << std::endl;

	for (vector<pRenderable>::const_iterator it = mRenderableObjects.begin(); it < mRenderableObjects.end(); it++)
		output << (*it)->print_debug(indent + 2);
	for (vector<pObject>::const_iterator it = mAssociatedObjects.begin(); it < mAssociatedObjects.end(); it++)
		output << (*it)->print_debug(indent + 2);

	return output.str();
}
