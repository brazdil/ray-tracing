/*
 * CProjection.h
 *
 *  Created on: Mar 16, 2012
 *      Author: db538
 */

#ifndef CPROJECTION_H_
#define CPROJECTION_H_

#include "CScreen.h"

class CProjection {
private:

	CScreen mScreen;

public:
	CProjection(const CScreen& screen);
	virtual ~CProjection();
};

#endif /* CPROJECTION_H_ */
