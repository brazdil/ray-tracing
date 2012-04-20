/*
 * Slave.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#include "Slave.h"

const char Slave::mVersion = 1;

Slave::ThreadSemaphore::ThreadSemaphore(unsigned int n)
	: mAvailable(n), mMaximum(n) {
	if (mMaximum == 0)
		throw std::invalid_argument("Need at least one thread to compute results");
}

void Slave::ThreadSemaphore::release() {
    lock lk(mMonitor);
    while (mAvailable == mMaximum)
        mBufferNotFull.wait(lk);
    ++mAvailable;
    mBufferNotEmpty.notify_one();
}

void Slave::ThreadSemaphore::acquire() {
    lock lk(mMonitor);
    while (mAvailable == 0)
        mBufferNotEmpty.wait(lk);
    --mAvailable;
    mBufferNotFull.notify_one();
}

void Slave::ThreadSemaphore::acquire_all() {
	for (unsigned int i = 0; i < mMaximum; ++i)
		acquire();
}

void Slave::ThreadSemaphore::release_all() {
	for (unsigned int i = 0; i < mMaximum; ++i)
		release();
}

void Slave::ThreadSemaphore::wait_and_reset() {
	acquire_all();
	release_all();
}

Slave::Result::Result() {

}

Slave::Result::Result(unsigned int _col, unsigned int _row, Color _color)
	: col(_col), row(_row), color(_color) {
}

Slave::ResultSender::ResultSender(pLogger logger, socket_ptr sock, unsigned int image_height, unsigned int first_col, unsigned int first_row, unsigned int max_threads)
	: mLogger(logger),
	  mSocket(sock),
	  mImageHeight(image_height),
	  mExpectedCol(first_col),
	  mExpectedRow(first_row),
	  mWaitingResults(max_threads) {
	if (mExpectedRow > mImageHeight)
		throw std::invalid_argument("First row higher than image height");
}

void Slave::ResultSender::send_result(unsigned int col, unsigned int row, Color color) {
	// get a lock on the object
	boost::mutex::scoped_lock lock(mObjectLock);

	// put result in the queue
	mLogger->println(boost::format("Queuing %dx%d, expecting %dx%d") % col % row % mExpectedCol % mExpectedRow, Logger::DETAILED);
	mWaitingResults.push_back(Result(col, row, color));

	// try to send as many results as possible
	bool sent_something;
	do {
		sent_something = false;

		// go through all the waiting results
		for (std::vector<Result>::iterator it = mWaitingResults.begin(); it < mWaitingResults.end(); it++) {
			if (((*it).col == mExpectedCol) && ((*it).row == mExpectedRow)) {
				mLogger->println(boost::format("Sending %dx%d") % (*it).col % (*it).row, Logger::DETAILED);

				// send it to the master
				boost::array<char, 4> buffer_result;
				buffer_result[0] = CommProtocol::MSG_OK;
				buffer_result[1] = (*it).color.getUnsignedCharRed();
				buffer_result[2] = (*it).color.getUnsignedCharGreen();
				buffer_result[3] = (*it).color.getUnsignedCharBlue();
				boost::asio::write(*mSocket, boost::asio::buffer(buffer_result, sizeof(buffer_result)));

				// remove it from the queue
				mWaitingResults.erase(it);

				// increment expected pixel
				mExpectedRow++;
				if (mExpectedRow >= mImageHeight) {
					mExpectedRow = 0;
					mExpectedCol++;
				}

				// indicate that sending was successful and break
				sent_something = true;
				break;
			}
		}

	} while (sent_something);
}

Slave::Slave(pLogger logger, string host, unsigned short port, unsigned int max_threads)
	: mLogger(logger), mMaster_Name(host), mMaster_Port(port), mMaxThreads(max_threads) {

	mLogger->println("SLAVE", Logger::DETAILED);
	mLogger->println(boost::format("\tHostname:       %s") % mMaster_Name, Logger::DETAILED);
	mLogger->println(boost::format("\tPort:           %d") % mMaster_Port, Logger::DETAILED);
	mLogger->println(boost::format("\tMax threads:    %d") % mMaxThreads, Logger::DETAILED);

	if (mMaxThreads == 0)
		throw std::invalid_argument("Need at least one thread");
}

Slave::~Slave() {
}

void Slave::compute_pixel(socket_ptr sock, pTask task, pThreadSemaphore thread_semaphore, pResultSender result_sender, unsigned int col, unsigned int row, unsigned int image_width) {
	mLogger->println(boost::format("Computing %dx%d") % col % row, Logger::DETAILED);

	Color result = task->getScreen()->getBackgroundColor();
	try {
		result = task->getObject()->getColorAtIntersection(
			task->getScreen()->getRay(image_width, col, row));
	} catch (IObject::no_intersection_exception&) {
		// no need to handle
	}

	result_sender->send_result(col, row, result);
//	mLogger->incrementAndPrintProgressBar();
	thread_semaphore->release();
}

void Slave::protocol_send_handshake(socket_ptr sock) {
	mLogger->println(boost::format("Sending handshake"), Logger::DETAILED);

	boost::array<char, sizeof(CommProtocol::HANDSHAKE)> buffer_handshake;
	memcpy(buffer_handshake.c_array(), CommProtocol::HANDSHAKE, sizeof(CommProtocol::HANDSHAKE));
	buffer_handshake[sizeof(CommProtocol::HANDSHAKE) - 1] = (signed char) Slave::mVersion;
	boost::asio::write(*sock, boost::asio::buffer(buffer_handshake, sizeof(CommProtocol::HANDSHAKE)));
}

void Slave::protocol_initialize(socket_ptr sock, uint32_t& input_file_length, uint32_t& image_width, uint32_t& image_height) {
	mLogger->println(boost::format("Waiting for initialization"), Logger::DETAILED);

	boost::system::error_code error;
	boost::array<char, 3 * sizeof(uint32_t)> buffer_init;

	size_t length = sock->read_some(boost::asio::buffer(buffer_init), error);
	if (error == boost::asio::error::eof || length != 3 * sizeof(uint32_t))
		throw std::runtime_error(boost::str(boost::format("Unexpected EOF or wrong length")));
	else if (error)
		throw boost::system::system_error(error);

	memcpy(&input_file_length, buffer_init.c_array(), sizeof(uint32_t));
	memcpy(&image_width, buffer_init.c_array() + sizeof(uint32_t), sizeof(uint32_t));
	memcpy(&image_height, buffer_init.c_array() + 2 * sizeof(uint32_t), sizeof(uint32_t));

	input_file_length = ntohl(input_file_length);
	image_width = ntohl(image_width);
	image_height = ntohl(image_height);

	mLogger->println(boost::format("Input file length: %d") % input_file_length, Logger::DETAILED);
	mLogger->println(boost::format("Image width: %d") % image_width, Logger::DETAILED);
	mLogger->println(boost::format("Image height: %d") % image_height, Logger::DETAILED);
}

pBinaryData Slave::protocol_download_input_file(socket_ptr sock, uint32_t input_file_length) {
	boost::system::error_code error;

	mLogger->println(boost::format("Downloading the input file"), Logger::INFORMATIVE);
	mLogger->enableProgressBar(true);
	mLogger->setProgressBarMax(input_file_length);
	mLogger->setProgressBarValue(0);
	mLogger->printProgressBar(true);

	pBinaryData input_file = create_binary_data(input_file_length);
	boost::array<char, 1024> buffer_input;
	size_t input_file_fill = 0;

	while (input_file_fill < input_file_length) {
		size_t length = sock->read_some(boost::asio::buffer(buffer_input), error);
		if (error == boost::asio::error::eof || length > input_file_length - input_file_fill || length > buffer_input.size())
			throw std::runtime_error(boost::str(boost::format("Unexpected EOF or wrong length")));
		else if (error)
			throw boost::system::system_error(error);

		memcpy(input_file->data() + input_file_fill, buffer_input.c_array(), length);

		input_file_fill += length;
		mLogger->incrementAndPrintProgressBar(length);
	}
	mLogger->enableProgressBar(false);

	return input_file;
}

pTask Slave::protocol_parse_input_file(socket_ptr sock, pBinaryData input_file) {
	mLogger->println(boost::format("Parsing the input file"), Logger::INFORMATIVE);

	pTask task;
	boost::array<char, 1> buffer_message;

	try {
		task = pTask(new Task(input_file));
	} catch (std::exception& ex) {
		mLogger->println(ex.what(), Logger::ERROR);

		mLogger->println(boost::format("Sending error message"), Logger::DETAILED);
		buffer_message[0] = CommProtocol::MSG_ERR;
		boost::asio::write(*sock, boost::asio::buffer(buffer_message, sizeof(buffer_message)));

		throw std::runtime_error("Could not parse the input file");
	}

	mLogger->println(boost::format("Sending acknowledgment"), Logger::DETAILED);
	buffer_message[0] = CommProtocol::MSG_ACK;
	boost::asio::write(*sock, boost::asio::buffer(buffer_message, sizeof(buffer_message)));

	return task;
}

void Slave::protocol_event_loop(socket_ptr sock, pTask task, uint32_t image_width, uint32_t image_height) {
	mLogger->println(boost::format("Entering task loop"), Logger::DETAILED);

	uint32_t col_from;
	uint32_t col_to;
	uint32_t first_row;

	bool finished = false;
	while (!finished) {
		char msg = protocol_event_loop_get_message(sock);

		switch(msg) {
		case CommProtocol::MSG_FINISHED:
			mLogger->println(boost::format("Task finished"), Logger::INFORMATIVE);
			finished = true;
			break;
		case CommProtocol::MSG_WAIT:
			mLogger->println(boost::format("Waiting..."), Logger::DETAILED);
			break;
		case CommProtocol::MSG_TASK:
			mLogger->println(boost::format("Awaiting task"), Logger::DETAILED);
			protocol_event_loop_get_task(sock, task, col_from, col_to, first_row);
			protocol_event_loop_solve_task(sock, task, image_width, image_height, col_from, col_to, first_row);
			break;
		default:
			throw std::runtime_error(boost::str(boost::format("Unknown message from master")));
		};
	}
}

char Slave::protocol_event_loop_get_message(socket_ptr sock) {
	boost::system::error_code error;
	boost::array<char, 1> buffer_message;

	mLogger->println(boost::format("Waiting for message"), Logger::DETAILED);

	size_t length = sock->read_some(boost::asio::buffer(buffer_message), error);
	if (error == boost::asio::error::eof || length != sizeof(char))
		throw std::runtime_error(boost::str(boost::format("Unexpected EOF or wrong length")));
	else if (error)
		throw boost::system::system_error(error);

	return buffer_message[0];
}

void Slave::protocol_event_loop_get_task(socket_ptr sock, pTask task, uint32_t& col_from, uint32_t& col_to, uint32_t& first_row) {
	boost::system::error_code error;
	boost::array<char, 3 * sizeof(uint32_t)> buffer_task;

	size_t length = sock->read_some(boost::asio::buffer(buffer_task), error);
	if (error == boost::asio::error::eof || length != sizeof(buffer_task))
		throw std::runtime_error(boost::str(boost::format("Unexpected EOF or wrong length")));
	else if (error)
		throw boost::system::system_error(error);

	memcpy(&col_from, buffer_task.c_array(), sizeof(uint32_t));
	memcpy(&col_to, buffer_task.c_array() + sizeof(uint32_t), sizeof(uint32_t));
	memcpy(&first_row, buffer_task.c_array() + 2 * sizeof(uint32_t), sizeof(uint32_t));

	col_from = ntohl(col_from);
	col_to = ntohl(col_to);
	first_row = ntohl(first_row);
}

void Slave::protocol_event_loop_solve_task(socket_ptr sock, pTask task, uint32_t image_width, uint32_t image_height, uint32_t col_from, uint32_t col_to, uint32_t first_row) {
	pThreadSemaphore thread_semaphore = pThreadSemaphore(new ThreadSemaphore(mMaxThreads));
	pResultSender result_sender = pResultSender(new ResultSender(mLogger, sock, image_height, col_from, first_row, mMaxThreads));

	mLogger->println(boost::format("Solving task: %d-%d") % col_from % (col_to - 1), Logger::INFORMATIVE);
//	mLogger->enableProgressBar(true);
//	mLogger->setProgressBarMax((col_to - col_from) * image_height - first_row);
//	mLogger->setProgressBarValue(0);

	for (unsigned int col = col_from; col < col_to; ++col) {
		for (unsigned int row = (col == col_from ? first_row : 0); row < image_height; ++row) {
			thread_semaphore->acquire();
			boost::thread t(boost::bind(&Slave::compute_pixel, this, sock, task, thread_semaphore, result_sender, col, row, image_width));
		}
	}

	thread_semaphore->wait_and_reset(); 	// wait for everyone to finish
//	mLogger->enableProgressBar(false);
}

void Slave::run() {
	try {
		boost::asio::io_service io_service;

		uint32_t input_file_length;
		uint32_t image_width;
		uint32_t image_height;

		pTask task;

		// get the master's endpoint
		string port = boost::str(boost::format("%d") % mMaster_Port);
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(tcp::v4(), mMaster_Name, port);
		tcp::resolver::iterator iterator = resolver.resolve(query);

		mLogger->println(boost::format("Connecting to %s") % mMaster_Name, Logger::INFORMATIVE);
		socket_ptr sock = socket_ptr(new tcp::socket(io_service));
		boost::asio::connect(*sock, iterator);

		protocol_send_handshake(sock);
		protocol_initialize(sock, input_file_length, image_width, image_height);
		{	// explicit block to free the input file memory after parsing
			pBinaryData input_file = protocol_download_input_file(sock, input_file_length);
			task = protocol_parse_input_file(sock, input_file);
		}
		if (image_height != task->getScreen()->getImageHeight(image_width))
			throw std::runtime_error("Image height doesn't fit what server claims");
		protocol_event_loop(sock, task, image_width, image_height);

	} catch (std::exception& e) {
		mLogger->println(e.what(), Logger::ERROR);
	}
}
