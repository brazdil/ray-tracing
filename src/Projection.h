/*
 * CProjection.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef PROJECTION_H_
#define PROJECTION_H_

#include "Screen.h"

class Projection {
private:

	Screen mScreen;

public:
	Projection(const Screen& screen);
	virtual ~Projection();
};

#endif /* PROJECTION_H_ */
