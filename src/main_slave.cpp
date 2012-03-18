//============================================================================
// Name        : ray-tracing.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include "Logger.h"
#include "Slave.h"

using namespace std;

int main(int argc, const char** argv) {
	int arg_log;
	string arg_master;
	unsigned short arg_port;

	po::options_description desc;
	desc.add_options()
	    ("help", "show this help message")
	    ("log", po::value<int>(&arg_log)->default_value(2), "level of logging (0=quiet, 1=crucial, 2=normal, 3=verbose)")
	    ("master", po::value<string>(&arg_master)->default_value("localhost"), "master to connect to")
	    ("port", po::value<unsigned short>(&arg_port)->default_value(29517), "port to connect to")
    ;

	po::positional_options_description pos_opt;
	pos_opt
		.add("master", 1)
		.add("port", 2);

	po::variables_map var_map;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(pos_opt).run(), var_map);
	po::notify(var_map);

	if (var_map.count("help")) {
	    cout << desc << std::endl;
	    return EXIT_SUCCESS;
	}

	Logger log;
	log.setLevel(arg_log);
	log.setTitle("slave");
	log.println(boost::format("Logging level set to %d") % log.getLevel(), Logger::DETAILED);

	Slave slave(log, arg_master, arg_port);
	slave.run();

	return EXIT_SUCCESS;
}
