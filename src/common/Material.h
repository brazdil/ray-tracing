/*
 * Material.h
 *
 *  Created on: 20 Apr 2012
 *      Author: db538
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

class Material {
private:
	std::string mName;

public:
	Material(std::string name);
	virtual ~Material();

	virtual double portion_reflection() const = 0;
	virtual double portion_refraction() const = 0;
	double portion_material() const;
};

#endif /* MATERIAL_H_ */
