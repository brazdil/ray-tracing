/*
 * SolidMaterial.cpp
 *
 *  Created on: 23 Apr 2012
 *      Author: db538
 */

#include "SolidMaterial.h"

SolidMaterial::SolidMaterial(std::string name, Color diffuse, Color specular, Color transmit, double refractive_index)
	: Material(name), mDiffuse(diffuse), mSpecular(specular), mTransmit(transmit), mRefractiveIndex(refractive_index) {
}

SolidMaterial::~SolidMaterial() {
}

Color SolidMaterial::getDiffuse() const {
	return mDiffuse;
}

Color SolidMaterial::getSpecular() const {
	return mSpecular;
}

Color SolidMaterial::getTransmit() const {
	return mTransmit;
}

double SolidMaterial::getRefractiveIndex() const {
	return mRefractiveIndex;
}
