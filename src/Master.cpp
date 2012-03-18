/*
 * Master.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#include "Master.h"

Master::Master(Logger& logger,
		       unsigned short port,
		       unsigned int image_width,
		       unsigned int work_division,
		       string input_file,
		       string output_file)
	: mLogger(logger),
	  mPort(port),
	  mOutputFile(output_file),
	  mWorkDivision(work_division),
	  mImageWidth(image_width) {

	mLogger.println("MASTER", Logger::DETAILED);
	mLogger.println(boost::format("\tPort:           %d") % mPort, Logger::DETAILED);
	mLogger.println(boost::format("\tImage width:    %d") % mImageWidth, Logger::DETAILED);
	mLogger.println(boost::format("\tWork division:  %d") % work_division, Logger::DETAILED);
	mLogger.println(boost::format("\tInput file:     %s") % input_file, Logger::DETAILED);
	mLogger.println(boost::format("\tOutput file:    %s") % mOutputFile, Logger::DETAILED);

	mLogger.println("Checking the scene description", Logger::DETAILED);
	// TODO: get height from scene data
	mImageHeight = image_width * 9 / 16;
}

Master::~Master() {
}

void Master::handle_connection(socket_ptr sock) {
	boost::system::error_code error;

	tcp::endpoint client = sock->remote_endpoint(error);
	if (error) throw boost::system::system_error(error);
	mLogger.println(boost::format("New connection from %s") % client.address().to_string(), Logger::INFORMATIVE);
}

void Master::run() {
	mLogger.println(boost::format("Starting master on port %d...") % mPort, Logger::INFORMATIVE);

	// create a queue with image parts
	mLogger.println("Creating work queue", Logger::DETAILED);
	unsigned int col_width = mImageWidth / mWorkDivision;
	for (unsigned int i = 0; i < mWorkDivision; ++i) {
		ImagePart* part = new ImagePart();
		part->col_from = i * col_width;
		if (i < mWorkDivision - 1) part->col_to = (i + 1) * col_width;
		else                       part->col_to = mImageWidth;
		mPartsQueue.push(part);
	}

	// set up progress bar
	mLogger.enableProgressBar(true);
	mLogger.setProgressBarMax((long) mImageWidth * (long) mImageHeight);
	mLogger.setProgressBarValue(0);
	mLogger.printProgressBar();

	try {
		boost::asio::io_service io_service;
		tcp::acceptor acc(io_service, tcp::endpoint(tcp::v4(), mPort));

		for (;;) {
			socket_ptr sock(new tcp::socket(io_service));
			acc.accept(*sock);
		    boost::thread t(boost::bind(&Master::handle_connection, this, sock));
		  }

	} catch (std::exception& e) {
		mLogger.println(e.what(), Logger::ERROR);
	}
}
