/*
 * Ratio.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: db538
 */

#include "Ratio.h"
#include "utils.h"
#include <stdexcept>

Ratio::Ratio(unsigned int num, unsigned int den)
	: mNumerator(num), mDenominator(den) {
	init();
}

Ratio::Ratio(tinyxml2::XMLElement* elem) {
	// Check the element's name
	if (!elem || !tinyxml2::XMLUtil::StringEqual(elem->Name(), "ratio"))
		throw std::invalid_argument("Not a \"ratio\" element");

	// Get the values
	mNumerator = parseUnsignedIntegerAttribute("numerator", elem);
	mDenominator = parseUnsignedIntegerAttribute("denominator", elem);

	init();
}

Ratio::~Ratio() {

}

void Ratio::init() {
	if (mDenominator == 0)
		throw std::invalid_argument("Division by zero");
}

unsigned int Ratio::getNumerator() {
	return mNumerator;
}

unsigned int Ratio::getDenominator() {
	return mDenominator;
}

double Ratio::getDouble() {
	return (double) mNumerator / (double) mDenominator;
}

