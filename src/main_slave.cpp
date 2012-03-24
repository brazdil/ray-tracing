//============================================================================
// Name        : ray-tracing.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/thread.hpp>
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
	unsigned int arg_threads;

	po::options_description desc;
	desc.add_options()
	    ("help", "show this help message")
	    ("log", po::value<int>(&arg_log)->default_value(2), "level of logging (0=quiet, 1=crucial, 2=normal, 3=verbose)")
	    ("hostname", po::value<string>(&arg_master)->default_value("localhost"), "master to connect to")
	    ("port", po::value<unsigned short>(&arg_port)->default_value(29517), "port to connect to")
	    ("threads", po::value<unsigned int>(&arg_threads)->default_value(boost::thread::hardware_concurrency() + 1), "number of threads to spawn")
    ;

	po::positional_options_description pos_opt;
	pos_opt
		.add("hostname", 1)
		.add("port", 2);

	po::variables_map var_map;
    po::store(po::command_line_parser(argc, argv).
              options(desc).positional(pos_opt).run(), var_map);
	po::notify(var_map);

	if (var_map.count("help")) {
	    cout << desc << std::endl;
	    return EXIT_SUCCESS;
	}

	pLogger logger = pLogger(new Logger());
	logger->setLevel(arg_log);
	logger->setTitle("slave");
	logger->println(boost::format("Logging level set to %d") % logger->getLevel(), Logger::DETAILED);

	Slave slave(logger, arg_master, arg_port, arg_threads);
	slave.run();

	return EXIT_SUCCESS;
}
