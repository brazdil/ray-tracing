/*
 * Master.h
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#ifndef MASTER_H_
#define MASTER_H_

#include "Logger.h"
#include "CommProtocol.h"
#include "Task.h"
#include "ResultManager.h"

#include <boost/thread.hpp>

using namespace boost;
using namespace std;

typedef shared_ptr<thread> pThread;

class Master {
private:

	struct PartialTask {
		unsigned int col_from;
		unsigned int col_to;
		unsigned int first_row;
		enum {
			PENDING,
			IN_PROGRESS,
			FINISHED
		} state;
	};

	typedef shared_ptr<PartialTask> pPartialTask;

	static const char mVersion;

	pLogger mLogger;
	unsigned short mPort;
	string mOutputFile;
	unsigned int mWorkDivision;
	unsigned int mImageWidth;
	unsigned int mImageHeight;
	unsigned int mRealtime;

	pBinaryData mInputFile;

	mutex mTasksLock;
	vector<pPartialTask> mTaskList;

	mutex mWorkersLock;
	vector<pThread> mWorkersList;

	pResultManager mResultManager;

	void accept_connections();
	void handle_connection(socket_ptr sock);

	typedef array<char, sizeof(CommProtocol::HANDSHAKE)> array_handshake;

	array_handshake protocol_get_handshake(socket_ptr sock, string client_address);
	void protocol_check_handshake(string client_address, array_handshake handshake);
	void protocol_initialize(socket_ptr sock, string client_address);
	void protocol_upload_input_file(socket_ptr sock, string client_address);
	void protocol_wait_for_acknowledgment(socket_ptr sock, string client_address);
	void protocol_assignment_loop(socket_ptr sock, string client_address);
	void protocol_assignment_loop_assign_task(socket_ptr sock, string client_address, pPartialTask task);
	void protocol_assignment_loop_get_results(socket_ptr sock, string client_address, pPartialTask task);
	void protocol_assignment_loop_send_finished(socket_ptr sock, string client_address);
	void protocol_assignment_loop_send_wait(socket_ptr sock, string client_address);

public:
	Master(pLogger logger,
	       unsigned short port,
	       unsigned int image_width,
	       unsigned int work_division,
		   string input_file,
		   string output_file,
		   unsigned int realtime);
	virtual ~Master();
	void run();
};

#endif /* MASTER_H_ */
