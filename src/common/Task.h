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
#include "Object.h"
#include "Light.h"
#include "Material.h"

#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

typedef shared_ptr< vector<char> >	pBinaryData;

inline pBinaryData create_binary_data(long len) {
	pBinaryData ptr(new vector<char>());
	ptr->reserve(len);
	return ptr;
}

class Task {
	pCamera mCamera;
	pScreen mScreen;
	pObject mSceneObject;
	list<pMaterial> mMaterials;
	list<const Light*> mLights;

	Color color_at_intersection(pObject obj, const Ray& ray, unsigned int ttl) const;
public:
	Task(pBinaryData input_file);
	virtual ~Task();

	/**
	 * Returns color of object that is hit by a ray, or background_color otherwise.
	 * Last argument is "time to live", number of recursive ray-traces, before
	 * it gives up and returns background_color.
	 */
	Color getColorAtIntersection(const Ray& ray, unsigned int ttl = 10) const;

	pCamera getCamera();
	pScreen getScreen();
	pObject getSceneObject();
	list<const Light*> getLights();
	list<pMaterial> getMaterials();
};

typedef shared_ptr<Task> pTask;

#endif /* TASK_H_ */
