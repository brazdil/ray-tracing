/*
 * IObject.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "Object.h"
#include <vector>
#include <utility>
#include <stdexcept>

using namespace std;

class NonRenderable : public Object {
private:

	virtual void ray_intersections(const Ray &ray, list<IntersectionPair> &result) const;
	virtual const BoundingBox& bounding_box() const;

public:
	NonRenderable();
	virtual ~NonRenderable();
};

#endif /* RENDERABLE_H_ */
