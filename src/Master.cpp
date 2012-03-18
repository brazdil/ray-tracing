/*
 * Master.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#include "Master.h"
#include "CommProtocol.h"

const unsigned char Master::mVersion = 1;

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
	string client_address = client.address().to_string();
	mLogger.println(boost::format("New connection from %s") % client_address, Logger::INFORMATIVE);

	mLogger.println("Waiting for handshake", Logger::DETAILED);
    boost::array<char, CommProtocol_HandshakeLength> buffer;
    size_t length = sock->read_some(boost::asio::buffer(buffer), error);
    if (error == boost::asio::error::eof || length != CommProtocol_HandshakeLength)
    	throw std::runtime_error(boost::str(boost::format("Unexpected EOF from %s") % client_address));
    else if (error)
      throw boost::system::system_error(error);

    mLogger.println("Checking handshake", Logger::DETAILED);
    size_t i = 0;
    const char* buffer_a = buffer.c_array();
    for (; i < sizeof(CommProtocol_HandshakeStart); i++)
    	if (buffer_a[i] != CommProtocol_HandshakeStart[i])
    		throw std::runtime_error(boost::str(boost::format("Error in handshake from %s") % client_address));
    unsigned char version = (unsigned char) *(buffer_a + i);
    if (version != Master::mVersion)
    	throw std::runtime_error(boost::str(boost::format("Version of %s doesn't match master's") % client_address));

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
