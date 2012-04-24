/*
 * Light.h
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "NonRenderable.h"
#include "Color.h"
#include <list>

class Light: public NonRenderable {
private:
	static bool filter_light(const Object*);
public:
	Light();
	virtual ~Light();

	virtual void addDiffuseIntensity(Color& result, const Vector3d& hit_point, const Vector3d& normal, pMaterial material, pObject root_object) const = 0;

	static list<const Light*> filterLights(pObject object);
};

#endif /* LIGHT_H_ */
