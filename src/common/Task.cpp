/*
 * Task.cpp
 *
 *  Created on: Mar 19, 2012
 *      Author: db538
 */

#include "Task.h"

#include <stdexcept>
#include <iostream>
#include <string>
#include "archive.h"
#include "archive_entry.h"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "XML.h"

using namespace boost;
using namespace std;

pBinaryData extract_file(pBinaryData input_file, string filename) {
	archive *file;
	archive_entry *file_entry;
	int res;

	// open the archive
	file = archive_read_new();
	archive_read_support_compression_none(file);
	archive_read_support_format_tar(file);
	res = archive_read_open_memory(file, input_file->data(), input_file->size());
	if (res != ARCHIVE_OK)
		throw runtime_error("The input file is not a TAR");

	// find the file
	bool found_file = false;
	while (archive_read_next_header(file, &file_entry) == ARCHIVE_OK) {
		if (!strcmp(archive_entry_pathname(file_entry), filename.c_str())) {
			found_file = true;
			break;
		}
	}

	if (!found_file)
		throw std::runtime_error(str(format("Couldn't find \"%s\" in the input file") % filename));

	uint64_t file_length = archive_entry_size(file_entry);
	pBinaryData data = create_binary_data(file_length);
	archive_read_data(file, (void*) data->data(), data->size());

	// finish
	res = archive_read_free(file);
	if (res != ARCHIVE_OK)
		throw runtime_error("The input file is not a TAR");

	return data;
}

Task::Task(pBinaryData input_file) {
	// Load the scene description XML
	pBinaryData file_xml_scene = extract_file(input_file, "scene.xml");
	XMLDocument xml_scene;
	int error = xml_scene.Parse(file_xml_scene->data());
	if (error != XML_SUCCESS)
		throw runtime_error(str(format("Could not parse the scene description: %s") % xml_scene.GetErrorStr1()));
	// Check root element's name - raytracing
	XMLElement* xml_root = xml_scene.RootElement();
	if (!xml_root || !XMLUtil::StringEqual(xml_root->Name(), "raytracing"))
		throw runtime_error("The input file does not contain scene description");

	mCamera = XML::parseCamera(xml_root);
	mScreen = XML::parseScreen(xml_root, mCamera);
	mRenderableObject = XML::parseObjects(xml_root);
}

Task::~Task() {
	// TODO Auto-generated destructor stub
}

pCamera Task::getCamera() {
	return mCamera;
}

pScreen Task::getScreen() {
	return mScreen;
}

pRenderable Task::getRenderableObject() {
	return mRenderableObject;
}
