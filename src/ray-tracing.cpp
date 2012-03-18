//============================================================================
// Name        : ray-tracing.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Logger.h"

using namespace std;

#ifndef GTEST_TESTING

int main() {
	Logger* log = Logger::getInstance();
	log->enableProgressBar(true);
	log->setProgressBarMax(1000);

	log->println("Hello world1", Logger::CRUCIAL);
	log->println("Hello world2", Logger::INFORMATIVE);
	log->println("Hello world3", Logger::DETAILED);

	for (int i = 0; i <= 33; i++) {
		log->setAndPrintProgressBar(i * 43);
		sleep(1);
	}

	Logger::deleteInstance();
	return 0;
}

#endif
