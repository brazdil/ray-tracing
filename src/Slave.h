/*
 * Slave.h
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#ifndef SLAVE_H_
#define SLAVE_H_

#include "Logger.h"
#include "CommProtocol.h"
#include "Task.h"

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

public:
	Slave(pLogger logger, string host, unsigned short port, unsigned int max_threads);
	virtual ~Slave();
	void run();
};

#endif /* SLAVE_H_ */
