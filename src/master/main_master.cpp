//============================================================================
// Name        : ray-tracing.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "Master.h"

using namespace std;

int main(int argc, const char** argv) {
	int arg_log;
	unsigned short arg_port;
	unsigned int arg_width;
	unsigned int arg_division;
	string arg_input;
	string arg_output;
	unsigned int arg_realtime = 0;

	po::options_description desc;
	desc.add_options()
	    ("help", "show this help message")
	    ("log", po::value<int>(&arg_log)->default_value(2), "level of logging (0=quiet, 1=crucial, 2=normal, 3=verbose)")
	    ("port", po::value<unsigned short>(&arg_port)->default_value(29517), "port to listen on for workers")
	    ("width", po::value<unsigned int>(&arg_width)->default_value(800), "width of the produced picture")
	    ("work-division", po::value<unsigned int>(&arg_division)->default_value(32), "number of parts the work should be divided in")
	    ("input-file", po::value<string>(&arg_input), "path to file with scene description")
    	("output-file", po::value<string>(&arg_output), "where to save the produced picture")
#ifndef NO_REALTIME
    	("realtime", po::value<unsigned int>(&arg_realtime)->default_value(1), "interval of realtime updates in seconds, 0 = off")
#endif
    ;

	po::positional_options_description pos_opt;
	pos_opt
		.add("input-file", 1)
		.add("output-file", 2);

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
	logger->setTitle("master");
	logger->println(boost::format("Logging level set to %d") % logger->getLevel(), Logger::DETAILED);

	if (!var_map.count("input-file") || !var_map.count("output-file")) {
		logger->println("You need to specify the input and output files", Logger::ERROR);
		return EXIT_FAILURE;
	}

	Master master(logger, arg_port, arg_width, arg_division, arg_input, arg_output, arg_realtime);
	master.run();

	return EXIT_SUCCESS;
}
