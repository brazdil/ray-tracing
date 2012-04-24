/*
 * PointLight.cpp
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#include "PointLight.h"
#include "Color.h"
#include <iostream>

PointLight::PointLight(const Color& intensity)
	: mPosition(0, 0, 0), mIntensity(intensity) {
}

PointLight::PointLight(const Vector3d& position, const Color& intensity)
	: mPosition(position), mIntensity(intensity) {
}

PointLight::~PointLight() {
}

void PointLight::addDiffuseIntensity(Color& result, const Vector3d& hit_point, const Vector3d& normal, pMaterial material, pObject root_object) const {
	if (root_object->isIntersected(mPosition, hit_point))
		return;
	else {
		Vector3d point_to_light = mPosition - hit_point;
		point_to_light.normalize();
		double cos_incident_angle = point_to_light.dot(normal);
		if (cos_incident_angle < 0.0)
			return;
		else
			// Id * kd * (L . N)
			result += (material->getDiffuse() * mIntensity) * cos_incident_angle;
	}
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
