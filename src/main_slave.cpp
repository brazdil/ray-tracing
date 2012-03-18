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

using namespace std;

int main(int argc, const char*[] argv) {
	Logger* log = Logger::getInstance();

	log->println("Starting slave", Logger::)

	po::options_description desc;
	desc.add_options()
	    ("help", "show this help message")
	    ("log", po::value<int>(), "level of logging (0 = quiet, 1 = only crucial, 2 = normal, 3 = verbose)");

	Logger::deleteInstance();
	return 0;
}
