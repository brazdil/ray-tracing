/*
 * SDLWindow.h
 *
 *  Created on: 17 Apr 2012
 *      Author: db538
 */

#ifndef SDLWINDOW_H_
#define SDLWINDOW_H_

#include "SDL.h"
#include "SDL_rotozoom.h"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

using namespace boost;

class ResultManager {
private:
	static const unsigned int mWndBpp = 32; // ! if you change this, change the initialization of result surface as well !
	static const unsigned int mWndFlags = SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF;

	const bool mUseSDL;

	unsigned int mWndWidth, mWndHeight;
	unsigned int mDrawingPosX, mDrawingPosY, mDrawingWidth, mDrawingHeight, mDrawingScale;
	SDL_Surface *mWindow;
	bool mWindowFinish;
	shared_ptr<thread> mWindowThread;
	unsigned int mWindowUpdateInterval;

	mutex mResultMutex;
	const unsigned int mImageWidth, mImageHeight;
	SDL_Surface *mResultSDL;
	unsigned char *mResultPlain;

	inline bool fitsInWindow();
	inline bool needsToFitHeight();

	void windowResize();
	void windowHandler();
public:
	ResultManager(unsigned int image_width, unsigned int image_height, unsigned int realtime);
	virtual ~ResultManager();

	void startWindow();
	void setPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b);
};

typedef shared_ptr<ResultManager> pResultManager;

#endif /* SDLWINDOW_H_ */
