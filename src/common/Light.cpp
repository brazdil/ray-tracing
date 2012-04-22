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

	list<const Object*> temp;
	object->filter(temp, filter_light);

	result.reserve(temp.size());
	BOOST_FOREACH(const Object* obj, temp)
		result.push_back(dynamic_cast<const Light*>(obj));

	return result;
}
