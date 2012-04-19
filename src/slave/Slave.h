/*
 * Slave.h
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#ifndef SLAVE_H_
#define SLAVE_H_

#include "../common/Logger.h"
#include "../common/CommProtocol.h"
#include "../common/Task.h"

#include <boost/utility.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

class Slave {
private:
	static const char mVersion;

	class ThreadSemaphore : private boost::noncopyable {
	private:
	    unsigned int mAvailable;
	    unsigned int mMaximum;
	    boost::condition mBufferNotFull, mBufferNotEmpty;
	    boost::mutex mMonitor;

	public:
	    typedef boost::mutex::scoped_lock lock;

	    ThreadSemaphore(unsigned int n);

	    void release();
	    void acquire();
	    void acquire_all();
	    void release_all();
	    void wait_and_reset();
	};

	struct Result {
		unsigned int col, row;
		Color color;

		Result();
		Result(unsigned int _col, unsigned int _row, Color _color);
	};

	class ResultSender {
	private:
		pLogger mLogger;
		socket_ptr mSocket;
		unsigned int mImageHeight;
		unsigned int mExpectedCol, mExpectedRow;
		std::vector<Result> mWaitingResults;

		boost::mutex mObjectLock;

	public:
		ResultSender(pLogger logger, socket_ptr sock, unsigned int image_height, unsigned int first_col, unsigned int first_row, unsigned int max_threads);
		void send_result(unsigned int col, unsigned int row, Color color);
	};

	typedef shared_ptr<ThreadSemaphore> pThreadSemaphore;
	typedef shared_ptr<ResultSender> pResultSender;

	pLogger mLogger;
	string mMaster_Name;
	unsigned short mMaster_Port;
	unsigned int mMaxThreads;

	void compute_pixel(socket_ptr sock, pTask task, pThreadSemaphore thread_semaphore, pResultSender result_sender, unsigned int col, unsigned int row);

	void protocol_send_handshake(socket_ptr);
	void protocol_initialize(socket_ptr sock, uint32_t& input_file_length, uint32_t& image_width, uint32_t& image_height);
	pBinaryData protocol_download_input_file(socket_ptr sock, uint32_t input_file_length);
	pTask protocol_parse_input_file(socket_ptr sock, pBinaryData input_file);
	void protocol_event_loop(socket_ptr sock, pTask task, uint32_t image_height);
	char protocol_event_loop_get_message(socket_ptr sock);
	void protocol_event_loop_get_task(socket_ptr sock, pTask task, uint32_t& col_from, uint32_t& col_to, uint32_t& first_row);
	void protocol_event_loop_solve_task(socket_ptr sock, pTask task, uint32_t image_height, uint32_t col_from, uint32_t col_to, uint32_t first_row);

public:
	Slave(pLogger logger, string host, unsigned short port, unsigned int max_threads);
	virtual ~Slave();
	void run();
};

#endif /* SLAVE_H_ */
