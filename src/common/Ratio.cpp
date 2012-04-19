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
	if (mDenominator == 0)
		throw std::invalid_argument("Division by zero");
}

Ratio::~Ratio() {

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

