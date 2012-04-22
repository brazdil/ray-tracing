/*
 * Light.h
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "Object.h"
#include "Color.h"

class Light: public Object {
private:
	static bool filter_light(const Object*);

public:
	Light();
	virtual ~Light();

	static vector<const Light*> filterLights(pObject object);
};

#endif /* LIGHT_H_ */
