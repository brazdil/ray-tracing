/*
 * XML.h
 *
 *  Created on: Mar 26, 2012
 *      Author: db538
 */

#ifndef XML_H_
#define XML_H_

#include "tinyxml2.h"
#include <Eigen/Dense>

#include "Ratio.h"
#include "Color.h"

#include "Camera.h"
#include "Screen.h"

#include "IObject.h"
#include "Sphere.h"
#include "Composite.h"

using namespace tinyxml2;
using namespace Eigen;

class XML {
private:
	static XMLElement* getPropertyElement(XMLElement* elem, const char* property, const char* type);
	static double parseUnsignedIntegerAttribute(XMLElement* elem, const char* name);
	static double parseDoubleAttribute(XMLElement* elem, const char* name);

public:

	static double parseDouble(XMLElement* xml, const char* name);
	static double parseAngle(XMLElement* xml, const char* name);
	static Vector3d parseVector3d(XMLElement* xml, const char* name);

	static Ratio parseRatio(XMLElement* xml, const char* name);
	static Color parseColor(XMLElement* xml, const char* name);

	static pCamera parseCamera(XMLElement* xml_root);
	static pScreen parseScreen(XMLElement* xml_root, pCamera camera);

	static pIObject parseObjects(XMLElement* xml_root);
	static pIObject parseComposite(XMLElement* xml_root);
	static pIObject parseSphere(XMLElement* xml);
};

#endif /* XML_H_ */
