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

using namespace std;

binary_data extract_file(binary_data input_file, string filename) {
	archive *file;
	archive_entry *file_entry;
	int res;

	// open the archive
	file = archive_read_new();
	archive_read_support_compression_none(file);
	archive_read_support_format_tar(file);
	res = archive_read_open_memory(file, input_file->data(), input_file->size());
	if (res != ARCHIVE_OK)
		throw std::runtime_error("The input file is not a TAR");

	// find the file
	bool found_file = false;
	while (archive_read_next_header(file, &file_entry) == ARCHIVE_OK) {
		if (!strcmp(archive_entry_pathname(file_entry), filename.c_str())) {
			found_file = true;
			break;
		}
		printf("%s\n", archive_entry_pathname(file_entry));
	}

	if (!found_file)
		throw std::runtime_error(boost::str(boost::format("Couldn't find \"%s\" in the input file") % filename));

	uint64_t file_length = archive_entry_size(file_entry);
	binary_data data = create_binary_data(file_length);
	archive_read_data(file, (void*) data->data(), data->size());

	// finish
	res = archive_read_free(file);
	if (res != ARCHIVE_OK)
		throw std::runtime_error("The input file is not a TAR");

	return data;
}

Task::Task(binary_data input_file) {
	binary_data scene_xml = extract_file(input_file, "scene.xml");
}

Task::~Task() {
	// TODO Auto-generated destructor stub
}

