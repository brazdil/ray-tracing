/*
 * Union.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef UNION_H_
#define UNION_H_

#include "IObject.h"
#include <vector>

using namespace std;

class Union: public IObject {
private:

	vector<pIObject> mObjects;
	BoundingBox mBoundingBox;

	virtual vector< pair<const IObject*, double> > ray_intersections(const Ray& ray) const;
	virtual const BoundingBox& bounding_box() const;
	virtual Vector3d normal(const Vector3d& point_on_surface) const;

public:

	Union(vector<pIObject> objects);
	virtual ~Union();

};

#endif /* UNION_H_ */
