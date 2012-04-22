/*
 * Light.cpp
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#include "Light.h"

Light::Light() {
}

Light::~Light() {
}

bool Light::filter_light(const Object* obj) {
	const Light *l = dynamic_cast<const Light*>(obj);
	return (l != NULL);
}

vector<const Light*> Light::filterLights(pObject object) {
	vector<const Light*> result;

	vector<const Object*> filtered = object->filter(filter_light);
	result.reserve(filtered.size());

	BOOST_FOREACH(const Object* obj, filtered)
		result.push_back(dynamic_cast<const Light*>(obj));

	return result;
}
