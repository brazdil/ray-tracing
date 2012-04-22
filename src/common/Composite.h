/*
 * Union.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef COMPOSITE_H_
#define COMPOSITE_H_

#include "Renderable.h"
#include <vector>

using namespace std;

class Composite: public Renderable {
private:

	vector<pRenderable> mRenderableObjects;
	vector<pObject> mAssociatedObjects;
	BoundingBox mBoundingBox;

	virtual vector<IntersectionPair> ray_intersections(const Ray& ray) const;
	virtual const BoundingBox& bounding_box() const;
	virtual Vector3d normal(const Vector3d& point_on_surface) const;

public:

	Composite(vector<pRenderable> renderable_objects, vector<pObject> associated_objects);
	virtual ~Composite();

	virtual vector<const Object*> filter(bool (*fn)(const Object*)) const;

	virtual pObject translate(const Vector3d& delta) const;
	virtual pObject scale(double factor) const;
	virtual std::string print_debug(unsigned int indent) const;
};

#endif /* COMPOSITE_H_ */
