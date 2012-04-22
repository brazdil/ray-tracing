/*
 * Color.h
 *
 *  Created on: Mar 20, 2012
 *      Author: db538
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "tinyxml2.h"

class Color {
private:

	float mR, mG, mB;

	void init();

public:
	Color();
	Color(float r, float g, float b);
	Color(unsigned char r, unsigned char g, unsigned char b);
	virtual ~Color();

	unsigned char getUnsignedCharRed() const;
	unsigned char getUnsignedCharGreen() const;
	unsigned char getUnsignedCharBlue() const;
};

#endif /* COLOR_H_ */
