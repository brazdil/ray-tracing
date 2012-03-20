/*
 * math.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef EXTRA_MATH_H_
#define EXTRA_MATH_H_

#define _USE_MATH_DEFINES

#include <cmath>
#include <limits>
#include <stdexcept>
#include <cstdio>
#include <Eigen/Dense>
#include "tinyxml2.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace Eigen;

const double ZERO_THRESHOLD = (8.0 * std::numeric_limits<double>::epsilon());

inline bool isZero(double x) {
	if ((x < ZERO_THRESHOLD) && (x > -ZERO_THRESHOLD))
		return true;
	else return false;
}

inline bool isEqual(double x, double y) {
	return isZero(x - y);
}

inline bool isEqual(const Vector3d& v1, const Vector3d& v2) {
	double v1_x = v1.dot(Vector3d::UnitX());
	double v1_y = v1.dot(Vector3d::UnitY());
	double v1_z = v1.dot(Vector3d::UnitZ());

	double v2_x = v2.dot(Vector3d::UnitX());
	double v2_y = v2.dot(Vector3d::UnitY());
	double v2_z = v2.dot(Vector3d::UnitZ());

	return isEqual(v1_x, v2_x) && isEqual(v1_y, v2_y) && isEqual(v1_z, v2_z);
}

inline double parseUnsignedIntegerAttribute(const char* name, tinyxml2::XMLElement* elem) {
	const tinyxml2::XMLAttribute* attr = elem->FindAttribute(name);
	if (!attr)
		throw std::invalid_argument(boost::str(boost::format("Could not find attribute \"%s\"") % name));
	unsigned int res;
	if (attr->QueryUnsignedValue(&res) != tinyxml2::XML_NO_ERROR)
		throw std::invalid_argument(boost::str(boost::format("Attribute \"%s\" is not of type unsigned int") % name));
	return res;
}

inline double parseDoubleAttribute(const char* name, tinyxml2::XMLElement* elem) {
	const tinyxml2::XMLAttribute* attr = elem->FindAttribute(name);
	if (!attr)
		throw std::invalid_argument(boost::str(boost::format("Could not find attribute \"%s\"") % name));
	double res;
	if (attr->QueryDoubleValue(&res) != tinyxml2::XML_NO_ERROR)
		throw std::invalid_argument(boost::str(boost::format("Attribute \"%s\" is not of type double") % name));
	return res;
}

inline double parseDoubleValue(tinyxml2::XMLElement* elem) {
	tinyxml2::XMLNode* node = elem->FirstChild();
	if (!node)
		throw std::invalid_argument(boost::str(boost::format("The element doesn't contain a value")));
	double res;
	if (sscanf(node->Value(), "%lf", &res) != 1)
		throw std::invalid_argument(boost::str(boost::format("The element's value is not of type double")));
	return res;
}

inline double parseAngle(tinyxml2::XMLElement* elem) {
	// Check the element's name
	if (!elem || !tinyxml2::XMLUtil::StringEqual(elem->Name(), "angle"))
		throw std::invalid_argument("Not an \"angle\" element");

	// Get the type
	bool units_degree;
	const tinyxml2::XMLAttribute* attr_units = elem->FindAttribute("units");
	if (!attr_units)
		throw std::invalid_argument("Attribute \"units\" is not specified");
	if (tinyxml2::XMLUtil::StringEqual(attr_units->Value(), "deg"))
		units_degree = true;
	else if (tinyxml2::XMLUtil::StringEqual(attr_units->Value(), "rad"))
		units_degree = false;
	else
		throw std::invalid_argument("Attribute \"units\" has to be set to either \"deg\" or \"rad\"");

	// Get the value
	double res = parseDoubleAttribute("value", elem);
	if (units_degree)
		res = M_PI * res / 180.0;
	return res;
}

inline Vector3d parseVector3d(tinyxml2::XMLElement* elem) {
	// Check the element's name
	if (!elem || !tinyxml2::XMLUtil::StringEqual(elem->Name(), "vector3"))
		throw std::invalid_argument("Not a \"vector3\" element");
	Vector3d res;
	res << 	parseDoubleAttribute("x", elem),
			parseDoubleAttribute("y", elem),
			parseDoubleAttribute("z", elem);
	return res;
}

#endif /* EXTRA_MATH_H_ */
