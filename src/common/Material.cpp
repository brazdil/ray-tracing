/*
 * IMaterial.cpp
 *
 *  Created on: 20 Apr 2012
 *      Author: db538
 */

#include "Material.h"
#include <stdexcept>

using namespace boost;

Material::Material(std::string name)
	: mName(name) {
}

Material::~Material() {
}

std::string Material::getName() {
	return mName;
}
