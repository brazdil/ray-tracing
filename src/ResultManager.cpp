/*
 * SDLWindow.cpp
 *
 *  Created on: 17 Apr 2012
 *      Author: db538
 */

#include "ResultManager.h"
#include <iostream>

ResultManager::ResultManager(unsigned int image_width, unsigned int image_height, unsigned int realtime)
	: mUseSDL(realtime > 0), mWndWidth(800), mWndHeight(600), mWindowUpdateInterval(realtime), mImageWidth(image_width), mImageHeight(image_height) {
	if (mUseSDL) {
		mWindowThread = shared_ptr<thread>();

		// init SDL
		if(SDL_Init(SDL_INIT_VIDEO) == -1)
			throw std::runtime_error(str(format("Unable to initialize SDL: %s") % SDL_GetError()));

		windowResize();

		// initialize surface for result
	    mResultSDL = SDL_CreateRGBSurface(SDL_SWSURFACE, mImageWidth, mImageHeight, mWndBpp, 0, 0, 0, 0);
	    if (mResultSDL == NULL)
			throw std::runtime_error(str(format("Unable to initialize SDL surface with size %dx%d") % mImageWidth % mImageHeight));
	} else {
		mResultPlain = new unsigned char[mImageWidth * mImageHeight * 3];
	}
}

ResultManager::~ResultManager() {
	if (mUseSDL) {
		if (mResultSDL)
			SDL_FreeSurface(mResultSDL);
		if (mWindowThread) {
			// wait for the thread to finish
			mWindowFinish = true;
			mWindowThread->join();
		}
		SDL_Quit();
	} else {
		if (mResultPlain)
			delete[] mResultPlain;
	}
}

inline bool ResultManager::fitsInWindow() {
	return mImageWidth <= mWndWidth && mImageHeight <= mWndHeight;
}

inline bool ResultManager::needsToFitHeight() {
	return mImageWidth * mWndHeight < mImageHeight * mWndWidth;
}

void ResultManager::windowResize() {
	// check that the video mode
	int bpp = SDL_VideoModeOK(mWndWidth, mWndHeight, mWndBpp, mWndFlags);
	if (bpp == 0)
		throw std::runtime_error(str(format("Video mode %dx%d unavailable") % mWndWidth % mWndHeight));

	// initialize window
	mWindow = SDL_SetVideoMode(mWndWidth, mWndHeight, mWndBpp, mWndFlags);
	if (mWindow == NULL)
		throw std::runtime_error(str(format("Unable to initialize SDL with window size %dx%d") % mWndWidth % mWndHeight));
	SDL_WM_SetCaption("Ray-tracing", NULL);

	// recompute results position and size
	if (fitsInWindow()) {
		mDrawingPosX = (mWndWidth - mImageWidth) / 2;
		mDrawingPosY = (mWndHeight - mImageHeight) / 2;
		mDrawingWidth = mImageWidth;
		mDrawingHeight = mImageHeight;
	} else {
		if (needsToFitHeight()) { // fit the height of the window
			// compute the scaling factor and round up
			mDrawingScale = mImageHeight / mWndHeight;
			if (mDrawingScale * mWndHeight < mImageHeight) mDrawingScale++;
		} else { // fit the width of the window
			// compute the scaling factor and round up
			mDrawingScale = mImageWidth / mWndWidth;
			if (mDrawingScale * mWndWidth < mImageWidth) mDrawingScale++;
		}

		mDrawingWidth = mImageWidth / mDrawingScale;
		mDrawingHeight = mImageHeight / mDrawingScale;
		mDrawingPosX = (mWndWidth - mDrawingWidth) / 2;
		mDrawingPosY = (mWndHeight - mDrawingHeight) / 2;
	}
}

void ResultManager::windowHandler() {
	SDL_Event event;

	// repeat until we are told to stop
	while (!mWindowFinish) {
		// check window resize
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_VIDEORESIZE:
				mWndWidth = event.resize.w;
				mWndHeight = event.resize.h;
				windowResize();
				break;
			default:
				break;
			}
		}

		// redraw
		{
			mutex::scoped_lock lock(mResultMutex);

			SDL_Surface *toDraw = mResultSDL;
			if (!fitsInWindow())
				toDraw = shrinkSurface(mResultSDL, mDrawingScale, mDrawingScale);

			SDL_Rect rect;
			rect.x = (Sint16)mDrawingPosX;
			rect.y = (Sint16)mDrawingPosY;
			SDL_BlitSurface(toDraw, NULL, mWindow, &rect);

			if (!fitsInWindow())
				SDL_FreeSurface(toDraw);

			SDL_Flip(mWindow);
		}

		// sleep for given time
		SDL_Delay(mWindowUpdateInterval * 1000);
	}
}

void ResultManager::startWindow() {
	mWindowFinish = false;
	mWindowThread = shared_ptr<thread>(new thread(bind(&ResultManager::windowHandler, this)));
}

void ResultManager::setPixel(unsigned int x, unsigned int y, unsigned char r, unsigned char g, unsigned char b) {
	mutex::scoped_lock lock(mResultMutex);

	assert(x < mImageWidth);
	assert(y < mImageHeight);

	if (mUseSDL) {
		SDL_LockSurface(mResultSDL);
		unsigned char *pixels = (unsigned char*) mResultSDL->pixels;
		unsigned int pixel_offset = (mImageWidth * y + x) * 4;
		pixels[pixel_offset + 0] = b;
		pixels[pixel_offset + 1] = g;
		pixels[pixel_offset + 2] = r;
		pixels[pixel_offset + 3] = 0xFF;
		SDL_UnlockSurface(mResultSDL);
	} else {
		unsigned int pixel_offset = (mImageWidth * y + x) * 3;
		mResultPlain[pixel_offset + 0] = r;
		mResultPlain[pixel_offset + 1] = g;
		mResultPlain[pixel_offset + 2] = b;
	}
}
