/*
 * Logger.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#include "Logger.h"
#include <iostream>
#include <stdexcept>

Logger* Logger::_Instance = NULL;

Logger* Logger::getInstance() {
	if (_Instance == NULL)
		_Instance = new Logger();
	return _Instance;
}

void Logger::deleteInstance() {
	if (_Instance != NULL) {
		_Instance->finish();
		delete _Instance;
	}
}

Logger::Logger()
	: mObjectLock(),
	  mMaxLevel(2),
	  mTitle(),
      mProgressBar_Enabled(false),
      mProgressBar_Value(0),
      mProgressBar_Max(100) {
}

Logger::~Logger() {
}

void Logger::setLevel(int level) {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (level < 0)
		mMaxLevel = 0;
	else
		mMaxLevel = level;
}

void Logger::setTitle(string title) {
	boost::mutex::scoped_lock lock(mObjectLock);
	mTitle = title;
}

void Logger::enableProgressBar(bool value) {
	boost::mutex::scoped_lock lock(mObjectLock);
	mProgressBar_Enabled = value;
}

void Logger::setProgressBarValue(int value) {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (value < 0)
		throw std::invalid_argument("Progress bar value has to be positive");
	else if (value > mProgressBar_Max)
		mProgressBar_Value = mProgressBar_Max;
	else
		mProgressBar_Value = value;
}

void Logger::setProgressBarMax(int value) {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (value < 1)
		throw std::invalid_argument("Progress bar maximum has to be greater than zero");

	mProgressBar_Max = value;
	if (mProgressBar_Value > mProgressBar_Max)
		mProgressBar_Value = mProgressBar_Max;
}

void Logger::println(string message, MessageImportance level) {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (level <= mMaxLevel) {
		clearProgressBar();

		std::cerr << "\r";
		if (mTitle.length() > 0)
			std::cerr << mTitle << ": ";
		if (level == ERROR)
			std::cerr << "ERROR - ";
		std::cerr << message << std::endl << std::flush;
		printProgressBar();
	}
}

void Logger::printProgressBar() {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (mProgressBar_Enabled) {
		int last = mProgressBar_LastValue * 1000 / mProgressBar_Max;
		int now = mProgressBar_Value * 1000 / mProgressBar_Max;
		if (now != last) {
			mProgressBar_LastValue = mProgressBar_Value;

			int percent = now / 10;
			int tenth = now % 10;

			clearProgressBar();

			std::cerr << "\r";
			std::cerr << "[";
			int i = 0;
			for (; i < percent / 5; ++i)
				std::cerr << "#";
			for (; i < 20; ++i)
				std::cerr << ".";
			std::cerr << "] ";
			std::cerr << percent << "." << tenth << "%";
			std::cerr << std::flush;
		}
	}
}

void Logger::setAndPrintProgressBar(int value) {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (value != mProgressBar_Value) {
		setProgressBarValue(value);
		printProgressBar();
	}
}

void Logger::incrementAndPrintProgressBar() {
	boost::mutex::scoped_lock lock(mObjectLock);
	setAndPrintProgressBar(mProgressBar_Value + 1);
}

void Logger::clearProgressBar() {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (mProgressBar_Enabled) {
		std::cerr << "\r";
		for (int i = 0; i < 28; ++i)
			std::cerr << " ";
		std::cerr << std::flush;
	}
}

void Logger::finish() {
	boost::mutex::scoped_lock lock(mObjectLock);
	if (mProgressBar_Enabled)
		std::cerr << std::endl;
}
