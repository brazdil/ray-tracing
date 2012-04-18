/*
 * Logger.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#include "Logger.h"
#include <iostream>
#include <stdexcept>

Logger::Logger()
	: mObjectLock(),
	  mMaxLevel(2),
	  mTitle(),
      mProgressBar_Enabled(false),
      mProgressBar_Finished(true),
      mProgressBar_Value(0),
      mProgressBar_Max(100) {
}

Logger::~Logger() {
	finish();
}

int Logger::getLevel() {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	return mMaxLevel;
}

void Logger::setLevel(int level) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (level < 0)
		mMaxLevel = 0;
	else
		mMaxLevel = level;
}

void Logger::setTitle(std::string title) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	mTitle = title;
}

void Logger::enableProgressBar(bool value) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (mProgressBar_Enabled && !value)
		clearProgressBar();
	mProgressBar_Enabled = value;
}

void Logger::setProgressBarValue(long value) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (value < 0)
		throw std::invalid_argument("Progress bar value has to be positive");
	else if (value > mProgressBar_Max)
		mProgressBar_Value = mProgressBar_Max;
	else
		mProgressBar_Value = value;
}

void Logger::setProgressBarMax(long value) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (value < 1)
		throw std::invalid_argument("Progress bar maximum has to be greater than zero");

	mProgressBar_Max = value;
	if (mProgressBar_Value > mProgressBar_Max)
		mProgressBar_Value = mProgressBar_Max;
}

void Logger::println(boost::basic_format<char> format, MessageImportance level) {
	println(boost::str(format), level);
}

void Logger::println(std::string message, MessageImportance level) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (level <= mMaxLevel) {
		clearProgressBar();

		std::cerr << "\r";
		if (mTitle.length() > 0)
			std::cerr << mTitle << ": ";
		if (level == ERROR)
			std::cerr << "ERROR - ";
		std::cerr << message << std::endl << std::flush;
		printProgressBar(true);
	}
}

void Logger::clearProgressBar() {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (mProgressBar_Enabled) {
		std::cerr << "\r";
		for (int i = 0; i < 35; ++i)
			std::cerr << " ";
		std::cerr << std::flush;
	}
}

void Logger::printProgressBar(bool force) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (mProgressBar_Enabled) {
		int last = mProgressBar_LastValue * 1000 / mProgressBar_Max;
		int now = mProgressBar_Value * 1000 / mProgressBar_Max;
		if (force || now != last) {
			mProgressBar_LastValue = mProgressBar_Value;

			int percent = now / 10;
			int tenth = now % 10;

			clearProgressBar();

			std::cerr << "\r";
			std::cerr << "[";
			int i = 0;
			for (; i < percent / 4; ++i)
				std::cerr << "#";
			for (; i < 25; ++i)
				std::cerr << ".";
			std::cerr << "] ";
			std::cerr << percent << "." << tenth << "%";
			std::cerr << std::flush;
			mProgressBar_Finished = false;
		}
	}
}

void Logger::setAndPrintProgressBar(long value, bool force) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (value != mProgressBar_Value) {
		setProgressBarValue(value);
		printProgressBar(force);
	}
}

void Logger::incrementAndPrintProgressBar(long delta, bool force) {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	setAndPrintProgressBar(mProgressBar_Value + delta, force);
}

void Logger::finish() {
	boost::recursive_mutex::scoped_lock lock(mObjectLock);
	if (mProgressBar_Enabled && !mProgressBar_Finished) {
		mProgressBar_Finished = true;
		std::cerr << std::endl;
	}
}
