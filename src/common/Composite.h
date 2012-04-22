/*
 * Union.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef COMPOSITE_H_
#define COMPOSITE_H_

#include "Object.h"
#include <vector>

using namespace std;

class Composite: public Object {
private:

	vector<pObject> mObjects;
	BoundingBox mBoundingBox;

	virtual vector<IntersectionPair> ray_intersections(const Ray& ray) const;
	virtual const BoundingBox& bounding_box() const;

public:

	Composite(vector<pObject> objects);
	virtual ~Composite();

	virtual vector<const Object*> filter(bool (*fn)(const Object*)) const;

	virtual pObject translate(const Vector3d& delta) const;
	virtual pObject scale(double factor) const;
	virtual std::string print_debug(unsigned int indent) const;
};

#endif /* COMPOSITE_H_ */
