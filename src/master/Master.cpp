/*
 * Master.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#include <fstream>

#include "Master.h"

const char Master::mVersion = 1;

Master::Master(pLogger logger,
		       unsigned short port,
		       unsigned int image_width,
		       unsigned int work_division,
		       string input_file,
		       string output_file,
		       unsigned int realtime)
	: mLogger(logger),
	  mPort(port),
	  mOutputFile(output_file),
	  mWorkDivision(work_division),
	  mImageWidth(image_width),
      mRealtime(realtime) {

	if (mWorkDivision > mImageWidth)
		mWorkDivision = mImageWidth;

	mLogger->println("MASTER", Logger::DETAILED);
	mLogger->println(format("\tPort:           %d") % mPort, Logger::DETAILED);
	mLogger->println(format("\tImage width:    %d") % mImageWidth, Logger::DETAILED);
	mLogger->println(format("\tWork division:  %d") % mWorkDivision, Logger::DETAILED);
	mLogger->println(format("\tInput file:     %s") % input_file, Logger::DETAILED);
	mLogger->println(format("\tOutput file:    %s") % mOutputFile, Logger::DETAILED);

	mLogger->println("Loading the scene description", Logger::DETAILED);
	try {
		std::ifstream file(input_file.c_str(), std::ios_base::in|std::ios_base::ate);

		size_t file_length = file.tellg();
		if (file_length > (uint32_t) 0xFFFFFFFFFFFFFFFF)
			throw std::invalid_argument("Input file too large (max 4GB");

		file.seekg(0, std::ios_base::beg);
		file.clear();

		mInputFile = create_binary_data(file_length);
		file.read(mInputFile->data(), mInputFile->size());
	} catch (std::exception& e) {
		throw std::runtime_error("Could not load the input file");
	}
	Task task(mInputFile);

	// compute height from scene data
	Ratio aspect_ratio = task.getScreen()->getAspectRatio();
	mImageHeight = mImageWidth * aspect_ratio.getDenominator() / aspect_ratio.getNumerator();

	// create ResultManager
	mResultManager = pResultManager(new ResultManager(mImageWidth, mImageHeight, mRealtime));
}

Master::~Master() {
}

void Master::accept_connections() {
	asio::io_service io_service;
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), mPort));

	for (;;) {
		socket_ptr sock(new tcp::socket(io_service));
		acceptor.accept(*sock);
		{
			mutex::scoped_lock lock(mWorkersLock);
			pThread t = pThread(new boost::thread(bind(&Master::handle_connection, this, sock)));
			mWorkersList.push_back(t);
		}
	}
}

Master::array_handshake Master::protocol_get_handshake(socket_ptr sock, string client_address) {
	mLogger->println(format("%s - Waiting for handshake") % client_address, Logger::DETAILED);

	system::error_code error;
	array_handshake buffer_handshake;

	size_t length = sock->read_some(asio::buffer(buffer_handshake), error);
	if (error == asio::error::eof || length != sizeof(CommProtocol::HANDSHAKE))
		throw std::runtime_error(str(format("%s - Unexpected EOF or wrong length") % client_address));
	else if (error)
		throw system::system_error(error);

	return buffer_handshake;
}

void Master::protocol_check_handshake(string client_address, array_handshake handshake) {
	mLogger->println(format("%s - Checking handshake") % client_address, Logger::DETAILED);

	size_t i = 0;
	const char* buffer_a = handshake.c_array();
	for (; i < sizeof(CommProtocol::HANDSHAKE) - 1; i++)
		if (buffer_a[i] != CommProtocol::HANDSHAKE[i])
			throw std::runtime_error(str(format("%s - Handshake error") % client_address));

	mLogger->println(format("%s - Checking version") % client_address, Logger::DETAILED);

	unsigned char version = (unsigned char) handshake[sizeof(CommProtocol::HANDSHAKE) - 1];
	if (version != Master::mVersion)
		throw std::runtime_error(str(format("%s - Version mismatch (master: %d, slave: %d)") % client_address % (int) Master::mVersion % (int) version));
}

void Master::protocol_initialize(socket_ptr sock, string client_address) {
	mLogger->println(format("%s - Initialization") % client_address, Logger::DETAILED);

	uint32_t input_file_length = mInputFile->size(); // size bound checked in constructor
	uint32_t input_file_length_n = htonl(input_file_length);
	uint32_t image_width_n = htonl(mImageWidth);
	uint32_t image_height_n = htonl(mImageHeight);

	array<char, 3 * sizeof(uint32_t)> buffer_init;
	memcpy(buffer_init.c_array(), &input_file_length_n, sizeof(uint32_t));
	memcpy(buffer_init.c_array() + sizeof(uint32_t), &image_width_n, sizeof(uint32_t));
	memcpy(buffer_init.c_array() + 2 * sizeof(uint32_t), &image_height_n, sizeof(uint32_t));

	asio::write(*sock, asio::buffer(buffer_init, 3 * sizeof(uint32_t)));
}

void Master::protocol_upload_input_file(socket_ptr sock, string client_address) {
	mLogger->println(format("%s - Uploading input file") % client_address, Logger::DETAILED);

	size_t input_file_length = mInputFile->size();
	uint32_t input_file_sent = 0;
	boost::array<char, 1024> buffer_data;
	while (input_file_sent < input_file_length) {
		if (input_file_length - input_file_sent < 1024) {
			memcpy(buffer_data.c_array(), mInputFile->data() + input_file_sent, input_file_length - input_file_sent);
			asio::write(*sock, asio::buffer(buffer_data, input_file_length - input_file_sent));
		} else {
			memcpy(buffer_data.c_array(), mInputFile->data() + input_file_sent, 1024);
			asio::write(*sock, asio::buffer(buffer_data, 1024));
		}
		input_file_sent += 1024;
	}
}

void Master::protocol_wait_for_acknowledgment(socket_ptr sock, string client_address) {
	mLogger->println(format("%s - Waiting for acknowledgment") % client_address, Logger::DETAILED);

	system::error_code error;
	array<char, 1> buffer_message;

	size_t length = sock->read_some(asio::buffer(buffer_message), error);
	if (error == asio::error::eof || length != sizeof(char))
		throw std::runtime_error(str(format("%s - Unexpected EOF or wrong length") % client_address));
	else if (error)
		throw system::system_error(error);

	if (buffer_message[0] != CommProtocol::MSG_ACK)
		throw std::runtime_error(str(format("%s - Slave failed to initialize") % client_address));
}

void Master::protocol_assignment_loop(socket_ptr sock, string client_address) {
	mLogger->println(format("%s - Task assignment loop") % client_address, Logger::DETAILED);

	bool task_finished = false;
	while (!task_finished) {
		// find a task for the slave
		pPartialTask task;
		{
			mutex::scoped_lock lock(mTasksLock);
			for (vector<pPartialTask>::iterator it = mTaskList.begin(); it < mTaskList.end(); it++)
				if ((*it)->state == PartialTask::PENDING) {
					task = *it;
					task->state = PartialTask::IN_PROGRESS;
					break;
				}
		}

		if (task) {
			try {
				protocol_assignment_loop_assign_task(sock, client_address, task);
				protocol_assignment_loop_get_results(sock, client_address, task);
				mLogger->println(format("%s - Partial task finished") % client_address, Logger::DETAILED);
				{
					mutex::scoped_lock lock(mTasksLock);
					task->state = PartialTask::FINISHED;
				}
			} catch (std::exception& e) {
				mLogger->println(format("%s - Error while working on task, putting it back in the queue") % client_address, Logger::DETAILED);
				{
					mutex::scoped_lock lock(mTasksLock);
					task->state = PartialTask::PENDING;
				}
				throw e;
			}
		} else {
			{
				mutex::scoped_lock lock(mTasksLock);
				task_finished = true;
				for (vector<pPartialTask>::iterator it = mTaskList.begin(); it < mTaskList.end(); it++)
					if ((*it)->state != PartialTask::FINISHED) {
						task_finished = false;
						break;
					}
			}
			if (task_finished) {
				protocol_assignment_loop_send_finished(sock, client_address);
			} else {
				protocol_assignment_loop_send_wait(sock, client_address);
				this_thread::sleep(posix_time::seconds(2));
			}
		}
	}
}

void Master::protocol_assignment_loop_assign_task(socket_ptr sock, string client_address, pPartialTask task) {
	mLogger->println(format("%s - Assigning partial task (%d-%d)") % client_address % task->col_from % task->col_to, Logger::DETAILED);

	array<char, 1> buffer_message;

	buffer_message[0] = CommProtocol::MSG_TASK;
	asio::write(*sock, asio::buffer(buffer_message, sizeof(char)));

	array<char, 3 * sizeof(uint32_t)> buffer_task;
	uint32_t col_from = htonl(task->col_from);
	uint32_t col_to = htonl(task->col_to);
	uint32_t first_row = htonl(task->first_row);
	memcpy(buffer_task.c_array(), &col_from, sizeof(uint32_t));
	memcpy(buffer_task.c_array() + sizeof(uint32_t), &col_to, sizeof(uint32_t));
	memcpy(buffer_task.c_array() + 2 * sizeof(uint32_t), &first_row, sizeof(uint32_t));
	asio::write(*sock, asio::buffer(buffer_task, 3 * sizeof(uint32_t)));
}

void Master::protocol_assignment_loop_get_results(socket_ptr sock, string client_address, pPartialTask task) {
	mLogger->println(format("%s - Receiving results") % client_address, Logger::DETAILED);

	system::error_code error;

	for (unsigned int col = task->col_from; col < task->col_to; ++col) {
		for (unsigned int row = (col == task->col_from ? task->first_row : 0); row < mImageHeight; ++row) {
			array<char, 4> buffer_result;

			size_t length = sock->read_some(asio::buffer(buffer_result), error);
			if (error == asio::error::eof || length != sizeof(buffer_result))
				throw std::runtime_error(str(format("%s - Unexpected EOF or wrong length") % client_address));
			else if (error)
				throw system::system_error(error);

			if (buffer_result[0] != CommProtocol::MSG_OK)
				throw std::runtime_error(str(format("%s - Error in computation of %dx%d") % client_address % col % row));

			{
				mutex::scoped_lock lock(mTasksLock);
				task->first_row++;
			}

			mResultManager->setPixel(col, row, (unsigned char) buffer_result[1], (unsigned char) buffer_result[2], (unsigned char) buffer_result[3]);
			mLogger->incrementAndPrintProgressBar();
		}
		{
			mutex::scoped_lock lock(mTasksLock);
			task->col_from++;
			task->first_row = 0;
		}
	}
}

void Master::protocol_assignment_loop_send_finished(socket_ptr sock, string client_address) {
	mLogger->println(format("%s - Closing connection") % client_address, Logger::DETAILED);
	array<char, 1> buffer_message;

	buffer_message[0] = CommProtocol::MSG_FINISHED;
	asio::write(*sock, asio::buffer(buffer_message, sizeof(char)));
}

void Master::protocol_assignment_loop_send_wait(socket_ptr sock, string client_address) {
	mLogger->println(format("%s - Waiting") % client_address, Logger::DETAILED);
	array<char, 1> buffer_message;

	buffer_message[0] = CommProtocol::MSG_WAIT;
	asio::write(*sock, asio::buffer(buffer_message, sizeof(char)));
}

void Master::handle_connection(socket_ptr sock) {
	try {
		system::error_code error;

		tcp::endpoint client = sock->remote_endpoint(error);
		if (error) throw system::system_error(error);
		string client_address = client.address().to_string();
		mLogger->println(format("New connection from %s") % client_address, Logger::INFORMATIVE);

		array_handshake handshake = protocol_get_handshake(sock, client_address);
		protocol_check_handshake(client_address, handshake);
		protocol_initialize(sock, client_address);
		protocol_upload_input_file(sock, client_address);
		protocol_wait_for_acknowledgment(sock, client_address);
		protocol_assignment_loop(sock, client_address);

	} catch (std::exception& e) {
		mLogger->println(e.what(), Logger::ERROR);
	}
}

void Master::run() {
	mLogger->println(format("Starting master on port %d...") % mPort, Logger::INFORMATIVE);

	// create a queue with image parts
	mLogger->println("Creating work queue", Logger::DETAILED);
	unsigned int col_width = mImageWidth / mWorkDivision;
	for (unsigned int i = 0; i < mWorkDivision; ++i) {
		pPartialTask part = pPartialTask(new PartialTask());
		part->col_from = i * col_width;
		if (i < mWorkDivision - 1) part->col_to = (i + 1) * col_width;
		else                       part->col_to = mImageWidth;
		mTaskList.push_back(part);
	}

	// clear the workers list
	mWorkersList.clear();

	// set up progress bar
	mLogger->enableProgressBar(true);
	mLogger->setProgressBarMax((long) mImageWidth * (long) mImageHeight);
	mLogger->setProgressBarValue(0);
	mLogger->printProgressBar(true);

	// start realtime window
	if (mRealtime)
		mResultManager->startWindow();

	// start accepting of connections
	thread t(bind(&Master::accept_connections, this));

	// wait for everyone to finish
	bool task_finished = false;
	while (!task_finished) {
		{
			mutex::scoped_lock lock(mTasksLock);
			task_finished = true;
			for (vector<pPartialTask>::iterator it = mTaskList.begin(); it < mTaskList.end(); it++)
				if ((*it)->state != PartialTask::FINISHED) {
					task_finished = false;
					break;
				}
		}
		this_thread::sleep(posix_time::seconds(2));
	}

	// lock the data structures, so that no worker can change it any more
	mutex::scoped_lock lock_workers(mWorkersLock);
	for (vector<pThread>::iterator it = mWorkersList.begin(); it < mWorkersList.end(); it++) {
		(*it)->join();
	}
	mutex::scoped_lock lock_tasks(mTasksLock);

	// save result
	mResultManager->saveResult(mOutputFile);
}
