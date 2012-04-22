/*
 * Union.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "Composite.h"
#include <stdexcept>

Composite::Composite(vector<pObject> objects)
	: mObjects(objects) {

	if (mObjects.empty())
		throw std::invalid_argument("Composite can't be empty");

	BOOST_FOREACH(pObject obj, mObjects)
		mBoundingBox = mBoundingBox.merge(obj->bounding_box());
}

Composite::~Composite() {
}

vector<IntersectionPair> Composite::ray_intersections(const Ray& ray) const {
	vector<IntersectionPair> result;

	BOOST_FOREACH(pObject obj, mObjects)
		BOOST_FOREACH(IntersectionPair i, obj->ray_intersections(ray))
			result.push_back(i);

	return result;
}

const BoundingBox& Composite::bounding_box() const {
	return mBoundingBox;
}

vector<const Object*> Composite::filter(bool (*fn)(const Object*)) const {
	vector<const Object*> result;

	BOOST_FOREACH(pObject obj, mObjects)
		BOOST_FOREACH(const Object *filtered_obj, obj->filter(fn))
			result.push_back(filtered_obj);

	return result;
}

pObject Composite::translate(const Vector3d& delta) const {
	vector<pObject> new_objects;
	new_objects.reserve(mObjects.size());

	BOOST_FOREACH(pObject obj, mObjects)
		new_objects.push_back(obj->translate(delta));

	return pObject(new Composite(new_objects));
}

pObject Composite::scale(double factor) const {
	vector<pObject> new_objects;
	new_objects.reserve(mObjects.size());

	BOOST_FOREACH(pObject obj, mObjects)
		new_objects.push_back(obj->scale(factor));

	return pObject(new Composite(new_objects));
}

std::string Composite::print_debug(unsigned int indent) const {
	std::ostringstream output;
	for (int i = 0; i < indent; ++i)
		output << " ";

	output << "Composite:"
		   << " objects=" << mObjects.size()
		   << std::endl;

	BOOST_FOREACH(pObject obj, mObjects)
		output << obj->print_debug(indent + 2);

	return output.str();
}
