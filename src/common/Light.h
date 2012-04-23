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

	static list<const Light*> filterLights(pObject object);
};

#endif /* LIGHT_H_ */
