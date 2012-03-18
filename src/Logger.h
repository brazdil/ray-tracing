/*
 * Logger.h
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <boost/thread.hpp>

using namespace std;

class Logger {
public:

	enum MessageImportance {
		ERROR = 0,
		CRUCIAL = 1,
		INFORMATIVE = 2,
		DETAILED = 3
	};

	static Logger* getInstance();
	static void deleteInstance();

	void setLevel(int level);
	void setTitle(string title);
	void enableProgressBar(bool value);
	void setProgressBarValue(int value);
	void setProgressBarMax(int value);

	void println(string message, MessageImportance level);
	void printProgressBar();
	void setAndPrintProgressBar(int value);
	void incrementAndPrintProgressBar();

	void finish();

private:

	Logger();
	virtual ~Logger();
	void clearProgressBar();

	static Logger* _Instance;
	boost::mutex mObjectLock;
	int mMaxLevel;
	string mTitle;
	bool mProgressBar_Enabled;
	int mProgressBar_Value;
	int mProgressBar_LastValue;
	int mProgressBar_Max;
};

#endif /* LOGGER_H_ */
