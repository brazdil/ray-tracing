/*
 * SolidMaterial.h
 *
 *  Created on: 23 Apr 2012
 *      Author: db538
 */

#ifndef SOLIDMATERIAL_H_
#define SOLIDMATERIAL_H_

#include "Material.h"

class SolidMaterial: public Material {
	Color mDiffuse, mSpecular, mTransmit;
	double mRefractiveIndex;

public:
	SolidMaterial(std::string name, Color diffuse, Color specular, Color transmit, double refractive_index);
	virtual ~SolidMaterial();

	virtual Color getDiffuse() const;
	virtual Color getSpecular() const;
	virtual Color getTransmit() const;
	virtual double getRefractiveIndex() const;
};

#endif /* SOLIDMATERIAL_H_ */
