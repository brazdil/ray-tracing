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

#include "Sphere.h"
#include "Composite.h"

using namespace tinyxml2;
using namespace Eigen;

class XML {
private:
	static std::string getProperty(XMLElement* elem, std::string property, std::string type);
	static std::string getParameter(XMLElement *elem, std::string type);
	static unsigned char getHex(char hex);

public:

	static double parseDouble(XMLElement* xml, std::string property);
	static double parseDouble(XMLElement* xml);
	static double parseDouble(std::string value);

	static double parseAngle(XMLElement* xml, std::string property);
	static double parseAngle(XMLElement* xml);
	static double parseAngle(std::string value);

	static Vector3d parseVector3d(XMLElement* xml, std::string name);
	static Vector3d parseVector3d(XMLElement* xml);
	static Vector3d parseVector3d(std::string value);

	static Ratio parseRatio(XMLElement* xml, std::string property);
	static Ratio parseRatio(XMLElement* xml);
	static Ratio parseRatio(std::string value);

	static Color parseColor(XMLElement* xml, std::string property);
	static Color parseColor(XMLElement* xml);
	static Color parseColor(std::string value);

	static pCamera parseCamera(XMLElement* xml_root);
	static pScreen parseScreen(XMLElement* xml_root, pCamera camera);
	static pRenderable parseObjects(XMLElement* xml_root);

	static pRenderable parseObjectOrOperation(XMLElement* xml);
	static pRenderable parseObject_Sphere(XMLElement* xml);
	static pRenderable parseOperation_Translate(XMLElement* xml);
	static pRenderable parseOperation_Scale(XMLElement* xml);
};

#endif /* XML_H_ */
