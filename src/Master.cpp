/*
 * Master.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#include "Master.h"

const char Master::mVersion = 1;

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

void Master::accept_connections() {
	boost::asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), mPort));

	for (;;) {
		socket_ptr sock(new tcp::socket(io_service));
		acceptor.accept(*sock);
		{
			boost::mutex::scoped_lock lock(mWorkersLock);
			boost::thread* t = new boost::thread(boost::bind(&Master::handle_connection, this, sock));
			mWorkersList.push_back(t);
		}
	}
}

void Master::handle_connection(socket_ptr sock) {

	try {
		boost::system::error_code error;

		tcp::endpoint client = sock->remote_endpoint(error);
		if (error) throw boost::system::system_error(error);
		string client_address = client.address().to_string();
		mLogger.println(boost::format("New connection from %s") % client_address, Logger::INFORMATIVE);

		mLogger.println(boost::format("%s - Waiting for handshake") % client_address, Logger::DETAILED);
		boost::array<char, sizeof(CommProtocol::HANDSHAKE)> buffer_handshake;
		size_t length = sock->read_some(boost::asio::buffer(buffer_handshake), error);
		if (error == boost::asio::error::eof || length != sizeof(CommProtocol::HANDSHAKE))
			throw std::runtime_error(boost::str(boost::format("%s - Unexpected EOF or wrong length") % client_address));
		else if (error)
			throw boost::system::system_error(error);

		mLogger.println(boost::format("%s - Checking handshake") % client_address, Logger::DETAILED);
		size_t i = 0;
		const char* buffer_a = buffer_handshake.c_array();
		for (; i < sizeof(CommProtocol::HANDSHAKE) - 1; i++)
			if (buffer_a[i] != CommProtocol::HANDSHAKE[i])
				throw std::runtime_error(boost::str(boost::format("%s - Handshake error") % client_address));

		mLogger.println(boost::format("%s - Checking version") % client_address, Logger::DETAILED);
		unsigned char version = (unsigned char) buffer_handshake[sizeof(CommProtocol::HANDSHAKE) - 1];
		if (version != Master::mVersion)
			throw std::runtime_error(boost::str(boost::format("%s - Version mismatch (master: %d, slave: %d)") % client_address % (int) Master::mVersion % (int) version));

		mLogger.println(boost::format("%s - Initialization") % client_address, Logger::DETAILED);
		uint32_t init_length_n = htonl(1234567);
		uint32_t image_width_n = htonl(mImageWidth);
		uint32_t image_height_n = htonl(mImageHeight);
		boost::array<char, 3 * sizeof(uint32_t)> buffer_init;
		memcpy(buffer_init.c_array(), &init_length_n, sizeof(uint32_t));
		memcpy(buffer_init.c_array() + sizeof(uint32_t), &image_width_n, sizeof(uint32_t));
		memcpy(buffer_init.c_array() + 2 * sizeof(uint32_t), &image_height_n, sizeof(uint32_t));
		boost::asio::write(*sock, boost::asio::buffer(buffer_init, 3 * sizeof(uint32_t)));
		// TODO: send the scene

		mLogger.println(boost::format("%s - Waiting for acknowledgment") % client_address, Logger::DETAILED);
		boost::array<char, 1> buffer_message;
		length = sock->read_some(boost::asio::buffer(buffer_message), error);
		if (error == boost::asio::error::eof || length != sizeof(char))
			throw std::runtime_error(boost::str(boost::format("%s - Unexpected EOF or wrong length") % client_address));
		else if (error)
			throw boost::system::system_error(error);
		if (buffer_message[0] != CommProtocol::MSG_ACK)
			throw std::runtime_error(boost::str(boost::format("%s - Slave failed to initialize") % client_address));

		mLogger.println(boost::format("%s - Task assignment loop") % client_address, Logger::DETAILED);
		bool task_finished = false;
		while (!task_finished) {
			// find a task for the slave
			PartialTask *task = NULL;
			{
				boost::mutex::scoped_lock lock(mTasksLock);
				for (vector<PartialTask*>::iterator it = mTaskList.begin(); it < mTaskList.end(); it++)
					if ((*it)->state == PartialTask::PENDING) {
						task = *it;
						task->state = PartialTask::IN_PROGRESS;
						break;
					}
			}

			if (task) {
				long pixels_finished = 0;
				try {
					mLogger.println(boost::format("%s - Assigning partial task (%d-%d)") % client_address % task->col_from % task->col_to, Logger::DETAILED);
					buffer_message[0] = CommProtocol::MSG_TASK;
					boost::asio::write(*sock, boost::asio::buffer(buffer_message, sizeof(char)));
					boost::array<char, 2 * sizeof(uint32_t)> buffer_task;
					uint32_t col_from = htonl(task->col_from);
					uint32_t col_to = htonl(task->col_to);
					memcpy(buffer_task.c_array(), &col_from, sizeof(uint32_t));
					memcpy(buffer_task.c_array() + sizeof(uint32_t), &col_to, sizeof(uint32_t));
					boost::asio::write(*sock, boost::asio::buffer(buffer_task, 2 * sizeof(uint32_t)));

					mLogger.println(boost::format("%s - Receiving results") % client_address, Logger::DETAILED);
					for (unsigned int col = task->col_from; col < task->col_to; ++col) {
						for (unsigned int row = 0; row < mImageHeight; ++row) {
							boost::array<char, 4> buffer_result;
							length = sock->read_some(boost::asio::buffer(buffer_result), error);
							if (error == boost::asio::error::eof || length != sizeof(buffer_result))
								throw std::runtime_error(boost::str(boost::format("%s - Unexpected EOF or wrong length") % client_address));
							else if (error)
								throw boost::system::system_error(error);
							if (buffer_result[0] != CommProtocol::MSG_OK)
								throw std::runtime_error(boost::str(boost::format("%s - Error in computation of %dx%d") % client_address % col % row));

							pixels_finished++;
							mLogger.incrementAndPrintProgressBar();
						}
						{
							boost::mutex::scoped_lock lock(mTasksLock);
							task->col_from++;
						}
						pixels_finished = 0;
					}

					mLogger.println(boost::format("%s - Partial task finished") % client_address, Logger::DETAILED);
					{
						boost::mutex::scoped_lock lock(mTasksLock);
						task->state = PartialTask::FINISHED;
					}
				} catch (std::exception& e) {
					mLogger.println(boost::format("%s - Error while working on task, putting it back in the queue") % client_address, Logger::DETAILED);
					{
						boost::mutex::scoped_lock lock(mTasksLock);
						task->state = PartialTask::PENDING;
					}
					mLogger.incrementAndPrintProgressBar(-pixels_finished, true);
					throw e;
				}
			} else {
				{
					boost::mutex::scoped_lock lock(mTasksLock);
					task_finished = true;
					for (vector<PartialTask*>::iterator it = mTaskList.begin(); it < mTaskList.end(); it++)
						if ((*it)->state != PartialTask::FINISHED) {
							task_finished = false;
							break;
						}
				}
				if (task_finished) {
					mLogger.println(boost::format("%s - Closing connection") % client_address, Logger::DETAILED);
					buffer_message[0] = CommProtocol::MSG_FINISHED;
					boost::asio::write(*sock, boost::asio::buffer(buffer_message, sizeof(char)));
				} else {
					mLogger.println(boost::format("%s - Waiting") % client_address, Logger::DETAILED);
					buffer_message[0] = CommProtocol::MSG_WAIT;
					boost::asio::write(*sock, boost::asio::buffer(buffer_message, sizeof(char)));
					boost::this_thread::sleep(boost::posix_time::seconds(2));
				}
			}
		}
	} catch (std::exception& e) {
		mLogger.println(e.what(), Logger::ERROR);
	}
}

void Master::run() {
	mLogger.println(boost::format("Starting master on port %d...") % mPort, Logger::INFORMATIVE);

	// create a queue with image parts
	mLogger.println("Creating work queue", Logger::DETAILED);
	unsigned int col_width = mImageWidth / mWorkDivision;
	for (unsigned int i = 0; i < mWorkDivision; ++i) {
		PartialTask* part = new PartialTask();
		part->col_from = i * col_width;
		if (i < mWorkDivision - 1) part->col_to = (i + 1) * col_width;
		else                       part->col_to = mImageWidth;
		mTaskList.push_back(part);
	}

	// clear the workers list
	mWorkersList.clear();

	// set up progress bar
	mLogger.enableProgressBar(true);
	mLogger.setProgressBarMax((long) mImageWidth * (long) mImageHeight);
	mLogger.setProgressBarValue(0);
	mLogger.printProgressBar();

	// start accepting of connections
	boost::thread t(boost::bind(&Master::accept_connections, this));

	// wait for everyone to finish
	bool task_finished = false;
	while (!task_finished) {
		{
			boost::mutex::scoped_lock lock(mTasksLock);
			task_finished = true;
			for (vector<PartialTask*>::iterator it = mTaskList.begin(); it < mTaskList.end(); it++)
				if ((*it)->state != PartialTask::FINISHED) {
					task_finished = false;
					break;
				}
		}
		boost::this_thread::sleep(boost::posix_time::seconds(2));
	}
	// lock workers and keep locked, so that the acceptor can't accept anyone else
	boost::mutex::scoped_lock lock(mWorkersLock);
	for (vector<boost::thread*>::iterator it = mWorkersList.begin(); it < mWorkersList.end(); it++) {
		(*it)->join();
		delete *it;
	}

	// destroy the tasks in work queue
	for (vector<PartialTask*>::iterator it = mTaskList.begin(); it < mTaskList.end(); it++)
		delete *it;
}
