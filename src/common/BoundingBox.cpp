/*
 * BoundingBox.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "BoundingBox.h"
#include <stdexcept>

const BoundingBox BoundingBox::null;

BoundingBox::BoundingBox()
	: mNull(true) {
}

BoundingBox::BoundingBox(double xmin, double xmax, double ymin, double ymax, double zmin, double zmax)
	: mNull(false),
	  mXMin(xmin),
	  mXMax(xmax),
	  mYMin(ymin),
	  mYMax(ymax),
	  mZMin(zmin),
	  mZMax(zmax) {
	if (mXMin >= mXMax || mYMin >= mYMax || mZMin >= mZMax)
		throw std::invalid_argument("BoundingBox: min boundaries must be less than max boundaries");
}

BoundingBox::BoundingBox(const BoundingBox* copy)
	: mNull(copy->mNull),
	  mXMin(copy->mXMin),
	  mXMax(copy->mXMax),
	  mYMin(copy->mYMin),
	  mYMax(copy->mYMax),
	  mZMin(copy->mZMin),
	  mZMax(copy->mZMax) {
}

BoundingBox::~BoundingBox() {
}

BoundingBox BoundingBox::merge(const BoundingBox& another) const {
	if (this->mNull)
		return another;

	if (another.mNull)
		return BoundingBox(this);

	return BoundingBox(
				(this->mXMin < another.mXMin) ? this->mXMin : another.mXMin,
				(this->mXMax > another.mXMax) ? this->mXMax : another.mXMax,
				(this->mYMin < another.mYMin) ? this->mYMin : another.mYMin,
				(this->mYMax > another.mYMax) ? this->mYMax : another.mYMax,
				(this->mZMin < another.mZMin) ? this->mZMin : another.mZMin,
				(this->mZMax > another.mZMax) ? this->mZMax : another.mZMax);
}

bool BoundingBox::intersects(const Ray& ray) const {
	if (mNull)
		return false;

	switch (ray.getClassification())
	{
	case Ray::MMM:
		// side(R,HD) < 0 or side(R,FB) > 0 or side(R,EF) > 0 or side(R,DC) < 0 or side(R,CB) < 0 or side(R,HE) > 0 to miss

		if ((ray.x() < mXMin) || (ray.y() < mYMin) || (ray.z() < mZMin) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMax < 0) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMax < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMin < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMax > 0))
			return false;

		return true;

	case Ray::MMP:
		// side(R,HD) < 0 or side(R,FB) > 0 or side(R,HG) > 0 or side(R,AB) < 0 or side(R,DA) < 0 or side(R,GF) > 0 to miss

		if ((ray.x() < mXMin) || (ray.y() < mYMin) || (ray.z() > mZMax) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMax < 0) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMin < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMin < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMax > 0))
			return false;

		return true;

	case Ray::MPM:
		// side(R,EA) < 0 or side(R,GC) > 0 or side(R,EF) > 0 or side(R,DC) < 0 or side(R,GF) < 0 or side(R,DA) > 0 to miss

		if ((ray.x() < mXMin) || (ray.y() > mYMax) || (ray.z() < mZMin) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMin < 0) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMax < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMax < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMin > 0))
			return false;

		return true;

	case Ray::MPP:
		// side(R,EA) < 0 or side(R,GC) > 0 or side(R,HG) > 0 or side(R,AB) < 0 or side(R,HE) < 0 or side(R,CB) > 0 to miss

		if ((ray.x() < mXMin) || (ray.y() > mYMax) || (ray.z() > mZMax) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMin < 0) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMin < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMax < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMin > 0))
			return false;

		return true;

	case Ray::PMM:
		// side(R,GC) < 0 or side(R,EA) > 0 or side(R,AB) > 0 or side(R,HG) < 0 or side(R,CB) < 0 or side(R,HE) > 0 to miss

		if ((ray.x() > mXMax) || (ray.y() < mYMin) || (ray.z() < mZMin) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMax < 0) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMax < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMin < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMax > 0))
			return false;

		return true;

	case Ray::PMP:
		// side(R,GC) < 0 or side(R,EA) > 0 or side(R,DC) > 0 or side(R,EF) < 0 or side(R,DA) < 0 or side(R,GF) > 0 to miss

		if ((ray.x() > mXMax) || (ray.y() < mYMin) || (ray.z() > mZMax) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMax < 0) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMin < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMin < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMax > 0))
			return false;

		return true;

	case Ray::PPM:
		// side(R,FB) < 0 or side(R,HD) > 0 or side(R,AB) > 0 or side(R,HG) < 0 or side(R,GF) < 0 or side(R,DA) > 0 to miss

		if ((ray.x() > mXMax) || (ray.y() > mYMax) || (ray.z() < mZMin) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMin < 0) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMin > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMax < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMax < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMin > 0))
			return false;

		return true;

	case Ray::PPP:
		// side(R,FB) < 0 or side(R,HD) > 0 or side(R,DC) > 0 or side(R,EF) < 0 or side(R,HE) < 0 or side(R,CB) > 0 to miss

		if ((ray.x() > mXMax) || (ray.y() > mYMax) || (ray.z() > mZMax) ||
			(ray.R0() + ray.i() * mYMax - ray.j() * mXMin < 0) ||
			(ray.R0() + ray.i() * mYMin - ray.j() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMin - ray.k() * mXMax > 0) ||
			(ray.R1() + ray.i() * mZMax - ray.k() * mXMin < 0) ||
			(ray.R3() - ray.k() * mYMin + ray.j() * mZMax < 0) ||
			(ray.R3() - ray.k() * mYMax + ray.j() * mZMin > 0))
			return false;

		return true;
	}

	return false;
}
