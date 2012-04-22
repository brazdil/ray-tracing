/*
 * Object.cpp
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#include "Object.h"

Object::Object() {
}

Object::~Object() {
}

vector<const Object*> Object::filter(bool (*fn)(const Object*)) const {
	vector<const Object*> result;
	if (fn(this)) result.push_back(this);
	return result;
}
