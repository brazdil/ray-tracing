/*
 * XML.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#include "XML.h"
#include <stdexcept>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

using namespace boost;

XMLElement* XML::getPropertyElement(XMLElement* elem, const char* property, const char* type) {
	XMLElement* subelem = elem->FirstChildElement(property);
	if (!subelem)
		throw std::invalid_argument(str(format("XML parse expected \"%s\"") % property));
	XMLElement* subelem_type = subelem->FirstChildElement(type);
	if (!subelem_type)
		throw std::invalid_argument(str(format("XML parse expected \"%s.%s\"") % property % type));
	return subelem_type;

}

double XML::parseUnsignedIntegerAttribute(XMLElement* elem, const char* name) {
	const XMLAttribute* attr = elem->FindAttribute(name);
	if (!attr)
		throw std::invalid_argument(str(format("Could not find attribute \"%s\"") % name));
	unsigned int res;
	if (attr->QueryUnsignedValue(&res) != XML_NO_ERROR)
		throw std::invalid_argument(str(format("Attribute \"%s\" is not of type unsigned int") % name));
	return res;
}

double XML::parseDoubleAttribute(XMLElement* elem, const char* name) {
	const XMLAttribute* attr = elem->FindAttribute(name);
	if (!attr)
		throw std::invalid_argument(str(format("Could not find attribute \"%s\"") % name));
	double res;
	if (attr->QueryDoubleValue(&res) != XML_NO_ERROR)
		throw std::invalid_argument(str(format("Attribute \"%s\" is not of type double") % name));
	return res;
}

double XML::parseDouble(XMLElement* xml, const char* name) {
	return	parseDoubleAttribute(
				getPropertyElement(xml, name, "double"),
				"value");
}

double XML::parseAngle(XMLElement* xml, const char* name) {
	XMLElement* elem = getPropertyElement(xml, name, "angle");

	// Get the type
	bool units_degree;
	const XMLAttribute* attr_units = elem->FindAttribute("units");
	if (!attr_units)
		throw std::invalid_argument("Attribute \"units\" is not specified");
	if (XMLUtil::StringEqual(attr_units->Value(), "deg"))
		units_degree = true;
	else if (XMLUtil::StringEqual(attr_units->Value(), "rad"))
		units_degree = false;
	else
		throw std::invalid_argument("Attribute \"units\" has to be set to either \"deg\" or \"rad\"");

	// Get the value
	double res = parseDoubleAttribute(elem, "value");
	if (units_degree)
		res = M_PI * res / 180.0;
	return res;
}

Vector3d XML::parseVector3d(XMLElement* xml, const char* name) {
	XMLElement* elem = getPropertyElement(xml, name, "vector3");

	Vector3d res;
	res << 	parseDoubleAttribute(elem, "x"),
			parseDoubleAttribute(elem, "y"),
			parseDoubleAttribute(elem, "z");
	return res;
}

Ratio XML::parseRatio(XMLElement* xml, const char* name) {
	XMLElement* elem = getPropertyElement(xml, name, "ratio");

	return	Ratio(
				parseUnsignedIntegerAttribute(elem, "numerator"),
				parseUnsignedIntegerAttribute(elem, "denominator"));
}

Color XML::parseColor(XMLElement* xml, const char* name) {
	XMLElement* elem = getPropertyElement(xml, name, "color");

	return 	Color(
				(float) parseUnsignedIntegerAttribute(elem, "r") / 255.0f,
				(float) parseUnsignedIntegerAttribute(elem, "g") / 255.0f,
				(float) parseUnsignedIntegerAttribute(elem, "b") / 255.0f);
}

pCamera XML::parseCamera(XMLElement* xml_root) {
	XMLElement* xml_elem_camera = xml_root->FirstChildElement("camera");
	if (!xml_elem_camera)
		throw std::runtime_error("Scene description doesn't contain camera information");

	return pCamera(
				new Camera(
					parseVector3d(xml_elem_camera, "position"),
					parseVector3d(xml_elem_camera, "look-at"),
					parseAngle(xml_elem_camera, "field-of-view")));
}

pScreen XML::parseScreen(XMLElement* xml_root, pCamera camera) {
	XMLElement* xml_elem_screen = xml_root->FirstChildElement("screen");
	if (!xml_elem_screen)
		throw std::runtime_error("Scene description doesn't contain screen information");

	return pScreen(
				new Screen(
					camera,
					parseDouble(xml_elem_screen, "distance"),
					parseRatio(xml_elem_screen, "aspect-ratio"),
					parseColor(xml_elem_screen, "background-color")));
}

pIObject XML::parseObjects(XMLElement* xml_root) {
	XMLElement* xml_elem_objects = xml_root->FirstChildElement("objects");
	if (!xml_elem_objects)
		throw std::runtime_error("Scene description doesn't contain objects information");

	XMLElement* xml_elem_composite = xml_elem_objects->FirstChildElement("composite");
	if (!xml_elem_composite)
		throw std::runtime_error("Objects part of XML needs to contain a composite first");

	return parseComposite(xml_elem_composite);
}

pIObject XML::parseComposite(XMLElement* xml_elem_composite) {
	vector<pIObject> sub_objects;

	XMLElement* xml_sub_elem = xml_elem_composite->FirstChildElement();
	while (xml_sub_elem) {

		if (XMLUtil::StringEqual(xml_sub_elem->Name(), "composite"))
			sub_objects.push_back(parseComposite(xml_sub_elem));
		else if (XMLUtil::StringEqual(xml_sub_elem->Name(), "sphere"))
			sub_objects.push_back(parseSphere(xml_sub_elem));

		xml_sub_elem = xml_sub_elem->NextSiblingElement();
	}

	return pIObject(new Composite(sub_objects));
}

pIObject XML::parseSphere(XMLElement* xml_elem_sphere) {
	return pIObject(
				new Sphere(
					parseVector3d(xml_elem_sphere, "center"),
					parseDouble(xml_elem_sphere, "radius")));
}
