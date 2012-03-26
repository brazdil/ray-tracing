/*
 * Sphere.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "Sphere.h"
#include <stdexcept>

using namespace tinyxml2;

Sphere::Sphere(Vector3d center, double radius)
	: mCenter(center), mRadius(radius), mNormalOutside(true) {
	init();
}

void Sphere::init() {
	if (mRadius < 0.0)
		throw std::invalid_argument("Sphere radius has to be positive");

	mBoundingBox = BoundingBox(
					mCenter.data()[0] - mRadius,
					mCenter.data()[0] + mRadius,
					mCenter.data()[1] - mRadius,
					mCenter.data()[1] + mRadius,
					mCenter.data()[2] - mRadius,
					mCenter.data()[2] + mRadius);
}

Sphere::~Sphere() {

}

vector<double> Sphere::ray_intersections(Ray& ray) {
	Vector3d vec_center_to_ray_point = ray.getOrigin() - mCenter;
	double a = ray.getDirection().dot(ray.getDirection());
	double b = vec_center_to_ray_point.dot(2.0 * ray.getDirection());
	double c = vec_center_to_ray_point.dot(vec_center_to_ray_point) - mRadius * mRadius;
	double d = b*b - 4*a*c;

	vector<double> intersections(2);

	if (d == 0) {
		// one intersection
		intersections.push_back((-b) / (2*a));
	} else if (d > 0) {
		// two intersections
		intersections.push_back((-b + sqrt(d)) / (2*a));
		intersections.push_back((-b - sqrt(d)) / (2*a));
	}

	return intersections;
}

BoundingBox& Sphere::bounding_box() {
	return mBoundingBox;
}
