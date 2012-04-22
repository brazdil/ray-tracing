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
		BOOST_FOREACH(pRenderable obj, mRenderableObjects)
			try {
				if (first) {
					mBoundingBox = obj->bounding_box();
					first = false;
				} else
					mBoundingBox = mBoundingBox.merge(obj->bounding_box());
			} catch (not_renderable_exception&) { }
	}
}

Composite::~Composite() {
}

vector<IntersectionPair> Composite::ray_intersections(const Ray& ray) const {
	if (mRenderableObjects.empty())
		return vector<IntersectionPair>();

	vector<IntersectionPair> result;

	BOOST_FOREACH(pRenderable obj, mRenderableObjects)
		BOOST_FOREACH(IntersectionPair i, obj->ray_intersections(ray))
			result.push_back(i);

	return result;
}

const BoundingBox& Composite::bounding_box() const {
	if (mRenderableObjects.empty())
		throw not_renderable_exception();
	return mBoundingBox;
}

Vector3d Composite::normal(const Vector3d& point_on_surface) const {
	throw std::runtime_error("Composite doesn't have a normal");
}

vector<const Object*> Composite::filter(bool (*fn)(const Object*)) const {
	vector<const Object*> result;

	BOOST_FOREACH(pRenderable obj, mRenderableObjects)
		BOOST_FOREACH(const Object *filtered_obj, obj->filter(fn))
			result.push_back(filtered_obj);

	BOOST_FOREACH(pObject obj, mAssociatedObjects)
		BOOST_FOREACH(const Object *filtered_obj, obj->filter(fn))
			result.push_back(filtered_obj);

	return result;
}

pObject Composite::translate(const Vector3d& delta) const {
	vector<pRenderable> new_renderable_objects;
	vector<pObject> new_associated_objects;
	new_renderable_objects.reserve(mRenderableObjects.size());
	new_associated_objects.reserve(mAssociatedObjects.size());

	BOOST_FOREACH(pRenderable obj, mRenderableObjects)
		new_renderable_objects.push_back(boost::dynamic_pointer_cast<Renderable>(obj->translate(delta)));

	BOOST_FOREACH(pObject obj, mAssociatedObjects)
		new_associated_objects.push_back(obj->translate(delta));

	return pObject(new Composite(new_renderable_objects, new_associated_objects));
}

pObject Composite::scale(double factor) const {
	vector<pRenderable> new_renderable_objects;
	vector<pObject> new_associated_objects;
	new_renderable_objects.reserve(mRenderableObjects.size());
	new_associated_objects.reserve(mAssociatedObjects.size());

	BOOST_FOREACH(pRenderable obj, mRenderableObjects)
		new_renderable_objects.push_back(boost::dynamic_pointer_cast<Renderable>(obj->scale(factor)));

	BOOST_FOREACH(pObject obj, mAssociatedObjects)
		new_associated_objects.push_back(obj->scale(factor));

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

	BOOST_FOREACH(pRenderable obj, mRenderableObjects)
		output << obj->print_debug(indent + 2);

	BOOST_FOREACH(pObject obj, mAssociatedObjects)
		output << obj->print_debug(indent + 2);

	return output.str();
}
