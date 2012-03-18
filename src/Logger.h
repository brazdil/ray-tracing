/*
 * Logger.h
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
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

	Logger();
	virtual ~Logger();

	int  getLevel();
	void setLevel(int level);

	void setTitle(string title);

	void println(boost::basic_format<char> format, MessageImportance level);
	void println(string message, MessageImportance level);

	void enableProgressBar(bool value);
	void setProgressBarValue(long value);
	void setProgressBarMax(long value);
	void printProgressBar(bool force = false);
	void setAndPrintProgressBar(long value, bool force = false);
	void incrementAndPrintProgressBar(long delta = 1, bool force = false);

	void finish();

private:

	void clearProgressBar();

	boost::recursive_mutex mObjectLock;
	int mMaxLevel;
	string mTitle;
	bool mProgressBar_Enabled;
	bool mProgressBar_Finished;
	long mProgressBar_Value;
	long mProgressBar_LastValue;
	long mProgressBar_Max;
};

#endif /* LOGGER_H_ */
