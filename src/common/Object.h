/*
 * Object.h
 *
 *  Created on: 22 Apr 2012
 *      Author: db538
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "utils.h"
#include <boost/shared_ptr.hpp>

using namespace std;

class Object;
typedef boost::shared_ptr<Object> pObject;

class Object {
public:
	Object();
	virtual ~Object();

	/*
	 * Translate
	 */
	virtual pObject translate(const Vector3d& delta) const = 0;

	/*
	 * Scale
	 */
	virtual pObject scale(double factor) const = 0;

	/*
	 * Print out information about the object
	 */
	virtual std::string print_debug(unsigned int indent = 0) const = 0;
};

#endif /* OBJECT_H_ */
