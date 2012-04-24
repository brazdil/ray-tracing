/*
 * Sphere.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "Sphere.h"
#include <limits>

using namespace tinyxml2;

Sphere::Sphere(pMaterial material)
	: mCenter(0.0, 0.0, 0.0), mRadius(1.0), mNormalOutside(true), mMaterial(material) {
	init();
}

Sphere::Sphere(pMaterial material, Vector3d center, double radius, bool normal_outside)
	: mCenter(center), mRadius(radius), mNormalOutside(normal_outside), mMaterial(material) {
	init();
}

Sphere::~Sphere() {
}

void Sphere::init() {
	if (mRadius < 0) {
		// flip inside out
		mRadius = -mRadius;
		mNormalOutside = !mNormalOutside;
	}

	mBoundingBox = BoundingBox(mCenter.data()[0] - mRadius,
				  	  	       mCenter.data()[0] + mRadius,
				  	  	       mCenter.data()[1] - mRadius,
				  	  	       mCenter.data()[1] + mRadius,
				  	  	       mCenter.data()[2] - mRadius,
				  	  	       mCenter.data()[2] + mRadius);
}

void Sphere::ray_intersections(const Ray &ray, list<IntersectionPair> &result) const {
	Vector3d vec_center_to_ray_point = ray.getOrigin() - mCenter;
	const double a = ray.getDirection().dot(ray.getDirection());
	const double b = vec_center_to_ray_point.dot(2.0 * ray.getDirection());
	const double c = vec_center_to_ray_point.dot(vec_center_to_ray_point) - mRadius * mRadius;
	const double d = b*b - 4*a*c;

	double r;
	if (isZero(d)) {
		// one intersection
		insert_intersection(IntersectionPair(this, (-b) / (2*a)), result);
	} else if (d > 0.0) {
		// two intersections
		insert_intersection(IntersectionPair(this, (-b + sqrt(d)) / (2*a)), result);
		insert_intersection(IntersectionPair(this, (-b - sqrt(d)) / (2*a)), result);
	}
}

const BoundingBox& Sphere::bounding_box() const {
	return mBoundingBox;
}

Vector3d Sphere::getNormal(const Vector3d& point_on_surface) const {
	Vector3d unit_normal = mNormalOutside ? point_on_surface - mCenter : mCenter - point_on_surface;
	unit_normal.normalize();
	return unit_normal;
}

pMaterial Sphere::getMaterial() const {
	return mMaterial;
}

pObject Sphere::translate(const Vector3d& delta) const {
	return pObject(new Sphere(mMaterial, mCenter + delta, mRadius, mNormalOutside));
}

pObject Sphere::scale(double factor) const {
	return pObject(new Sphere(mMaterial, mCenter * factor, mRadius * factor, mNormalOutside));
}

// rotate center (and top/side points)

std::string Sphere::print_debug(unsigned int indent) const {
	std::ostringstream output;
	for (int i = 0; i < indent; ++i)
		output << " ";

	output << "Sphere: center=" << toString(mCenter)
		   << " radius=" << mRadius
		   << " normal_out=" << mNormalOutside
		   << " material=" << mMaterial->getName()
		   << std::endl;

	return output.str();
}
