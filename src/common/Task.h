/*
 * Task.h
 *
 *  Created on: Mar 19, 2012
 *      Author: db538
 */

#ifndef TASK_H_
#define TASK_H_

#include "Camera.h"
#include "Screen.h"
#include "Renderable.h"
#include "Light.h"

#include <vector>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

typedef shared_ptr< vector<char> >	pBinaryData;

inline pBinaryData create_binary_data(long len) {
	return pBinaryData(new vector<char>(len));
}

class Task {
	pCamera mCamera;
	pScreen mScreen;
	pRenderable mRenderableObject;
	vector<const Light*> mLights;

public:
	Task(pBinaryData input_file);
	virtual ~Task();

	pCamera getCamera();
	pScreen getScreen();
	pRenderable getRenderableObject();
	vector<const Light*> getLights();
};

typedef shared_ptr<Task> pTask;

#endif /* TASK_H_ */
