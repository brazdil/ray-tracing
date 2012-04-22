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
	// check settings
	double reflect = portion_reflection();
	double refract = portion_refraction();
	if (reflect < 0.0)
		throw std::runtime_error(str(format("Material \"%s\" can't have negative reflection coefficient") % mName));
	if (reflect > 1.0)
		throw std::runtime_error(str(format("Material \"%s\" can't have reflection coefficient greater than 1.0") % mName));
	if (refract < 0.0)
		throw std::runtime_error(str(format("Material \"%s\" can't have negative refraction coefficient") % mName));
	if (refract > 1.0)
		throw std::runtime_error(str(format("Material \"%s\" can't have refraction coefficient greater than 1.0") % mName));
	if (reflect + refract > 1.0)
		throw std::runtime_error(str(format("Material \"%s\" can't have the sum of reflection and refraction coefficients greater than 1.0") % mName));
}

Material::~Material() {
}

double Material::portion_material() const {
	return 1.0 - portion_reflection() - portion_refraction();
}
