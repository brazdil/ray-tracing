/*
 * Task.h
 *
 *  Created on: Mar 19, 2012
 *      Author: db538
 */

#ifndef TASK_H_
#define TASK_H_

#include <vector>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

typedef shared_ptr< vector<char> >	binary_data;

inline binary_data create_binary_data(long len) {
	return binary_data(new vector<char>(len));
}

class Task {
public:
	Task(binary_data input_file);
	virtual ~Task();
};

#endif /* TASK_H_ */
