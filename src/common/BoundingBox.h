/*
 * BoundingBox.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "Ray.h"

class BoundingBox {

	double mXMin, mXMax;
	double mYMin, mYMax;
	double mZMin, mZMax;

public:

	BoundingBox();
	BoundingBox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax);
	virtual ~BoundingBox();

	BoundingBox merge(const BoundingBox& another) const;
	bool intersects(const Ray& ray) const;

//	double getXMin() const;
//	double getXMax() const;
//	double getYMin() const;
//	double getYMax() const;
//	double getZMin() const;
//	double getZMax() const;
};

#endif /* BOUNDINGBOX_H_ */
