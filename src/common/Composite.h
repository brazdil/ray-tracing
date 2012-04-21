/*
 * Union.h
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

	virtual vector< pair<const IObject*, double> > ray_intersections(const Ray& ray) const;
	virtual const BoundingBox& bounding_box() const;
	virtual Vector3d normal(const Vector3d& point_on_surface) const;

public:

	Composite(vector<pIObject> objects);
	virtual ~Composite();

	virtual pIObject translate(const Vector3d& delta) const;
	virtual pIObject scale(double factor) const;
	virtual std::string print_debug(unsigned int indent) const;
};

#endif /* COMPOSITE_H_ */
