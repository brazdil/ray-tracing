/*
 * IMaterial.h
 *
 *  Created on: 20 Apr 2012
 *      Author: db538
 */

#ifndef IMATERIAL_H_
#define IMATERIAL_H_

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

class IMaterial {
private:
	std::string mName;

public:
	IMaterial(std::string name);
	virtual ~IMaterial();

	virtual double portion_reflection() const = 0;
	virtual double portion_refraction() const = 0;
	double portion_material() const;
};

#endif /* IMATERIAL_H_ */
