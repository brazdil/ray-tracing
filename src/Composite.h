/*
 * Composite.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef COMPOSITE_H_
#define COMPOSITE_H_

#include "IObject.h"
#include <vector>

using namespace std;

class Composite: public IObject {
private:

	vector<pIObject> mObjects;
	BoundingBox mBoundingBox;

	virtual vector<double> ray_intersections(Ray& ray);
	virtual BoundingBox& bounding_box();

public:

	Composite(vector<pIObject> objects);
	virtual ~Composite();

};

#endif /* COMPOSITE_H_ */
