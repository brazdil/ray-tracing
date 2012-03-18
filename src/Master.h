/*
 * Master.h
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#ifndef MASTER_H_
#define MASTER_H_

#include "Logger.h"
#include <queue>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class Master {
private:

	struct ImagePart {
		int col_from;
		int col_to;
	};

	static const unsigned char mVersion = 1;

	Logger& mLogger;
	unsigned short mPort;
	string mOutputFile;
	unsigned int mWorkDivision;
	unsigned int mImageWidth;
	unsigned int mImageHeight;
	std::queue<ImagePart*> mPartsQueue;

	void handle_connection(socket_ptr sock);

public:
	Master(Logger& logger,
	       unsigned short port,
	       unsigned int image_width,
	       unsigned int work_division,
		   string input_file,
		   string output_file);
	virtual ~Master();
	void run();
};

#endif /* MASTER_H_ */
