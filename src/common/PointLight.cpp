/*
 * PointLight.cpp
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#include "PointLight.h"
#include "Color.h"

PointLight::PointLight(const Color& intensity)
	: mPosition(0, 0, 0), mIntensity(intensity) {
}

PointLight::PointLight(const Vector3d& position, const Color& intensity)
	: mPosition(position), mIntensity(intensity) {
}

PointLight::~PointLight() {
}

pObject PointLight::translate(const Vector3d& delta) const {
	return pObject(new PointLight(mPosition + delta, mIntensity));
}

pObject PointLight::scale(double factor) const {
	return pObject(new PointLight(mPosition * factor, mIntensity));
}

std::string PointLight::print_debug(unsigned int indent) const {
	std::ostringstream output;
	for (int i = 0; i < indent; ++i)
		output << " ";

	output << "PointLight: position=" << toString(mPosition)
		   << " intensity=" << toString(mIntensity)
		   << std::endl;

	return output.str();
}
