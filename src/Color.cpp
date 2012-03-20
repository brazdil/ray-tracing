/*
 * Color.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: db538
 */

#include "Color.h"
#include "utils.h"
#include <stdexcept>

Color::Color()
	: mR(0.0f), mG(0.0f), mB(0.0f) {
	init();
}

Color::Color(float r, float g, float b)
	: mR(r), mG(g), mB(b) {
	init();
}

Color::Color(unsigned int r, unsigned int g, unsigned int b)
	: mR((float)r / 255.0f), mG((float)g / 255.0f), mB((float)b / 255.0f) {
	init();
}

Color::Color(tinyxml2::XMLElement* elem) {
	// Check the element's name
	if (!elem || !tinyxml2::XMLUtil::StringEqual(elem->Name(), "color"))
		throw std::invalid_argument("Not a \"color\" element");

	// Get the values
	mR = (double) parseUnsignedIntegerAttribute("r", elem) / 255.0f;
	mG = (double) parseUnsignedIntegerAttribute("g", elem) / 255.0f;
	mB = (double) parseUnsignedIntegerAttribute("b", elem) / 255.0f;

	init();
}

void Color::init() {
	if (mR < 0.0 || mR > 1.0 ||
		mG < 0.0 || mG > 1.0 ||
		mB < 0.0 || mB > 1.0)
		throw std::invalid_argument("Color channel out of bounds");
}

Color::~Color() {
	// TODO Auto-generated destructor stub
}

