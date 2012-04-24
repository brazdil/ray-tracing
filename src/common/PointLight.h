/*
 * PointLight.h
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#ifndef POINTLIGHT_H_
#define POINTLIGHT_H_

#include "Light.h"

class PointLight: public Light {

	const Vector3d mPosition;
	const Color mIntensity;

public:
	PointLight(const Color& intensity);
	PointLight(const Vector3d& position, const Color& intensity);
	virtual ~PointLight();

	virtual void addDiffuseIntensity(Color& result, const Vector3d& hit_point, const Vector3d& normal, pMaterial material, pObject root_object) const;

	virtual pObject translate(const Vector3d& delta) const;
	virtual pObject scale(double factor) const;
	virtual std::string print_debug(unsigned int indent) const;
};

#endif /* POINTLIGHT_H_ */
