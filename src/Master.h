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

using namespace boost;
using namespace std;

typedef shared_ptr<thread> pThread;

class Master {
private:

	struct PartialTask {
		unsigned int col_from;
		unsigned int col_to;
		enum {
			PENDING,
			IN_PROGRESS,
			FINISHED
		} state;
	};

	typedef shared_ptr<PartialTask> pPartialTask;

	static const char mVersion;

	Logger& mLogger;
	unsigned short mPort;
	string mOutputFile;
	unsigned int mWorkDivision;
	unsigned int mImageWidth;
	unsigned int mImageHeight;

	pBinaryData mInputFile;

	mutex mTasksLock;
	vector<pPartialTask> mTaskList;

	mutex mWorkersLock;
	vector<pThread> mWorkersList;

	void accept_connections();
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
