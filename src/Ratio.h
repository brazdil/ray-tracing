/*
 * Ratio.h
 *
 *  Created on: Mar 20, 2012
 *      Author: db538
 */

#ifndef RATIO_H_
#define RATIO_H_

#include "tinyxml2.h"

class Ratio {
private:
	unsigned int mNumerator;
	unsigned int mDenominator;

	void init();

public:
	Ratio(unsigned int num, unsigned int den);
	Ratio(tinyxml2::XMLElement* elem);
	virtual ~Ratio();

	unsigned int getNumerator();
	unsigned int getDenominator();
	double getDouble();
};

#endif /* RATIO_H_ */
