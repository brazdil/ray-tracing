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
#include <boost/shared_ptr.hpp>
#include "Color.h"

class Material {
private:
	std::string mName;

public:
	Material(std::string name);
	virtual ~Material();

	std::string getName();

	virtual Color getDiffuse() const = 0;
	virtual Color getSpecular() const = 0;
	virtual Color getTransmit() const = 0;
	virtual double getRefractiveIndex() const = 0;
};

typedef boost::shared_ptr<Material> pMaterial;

#endif /* MATERIAL_H_ */
