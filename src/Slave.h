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

class Slave {
private:
	static const char mVersion;

	Logger& mLogger;
	string mMaster_Name;
	unsigned short mMaster_Port;

public:
	Slave(Logger& logger, string host, unsigned short port);
	virtual ~Slave();
	void run();
};

#endif /* SLAVE_H_ */
