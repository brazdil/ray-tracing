/*  

SDL_rotozoom.c: rotozoomer, zoomer and shrinker for 32bit or 8bit surfaces

Copyright (C) 2001-2011  Andreas Schiffler

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.

Andreas Schiffler -- aschiffler at ferzkopp dot net

*/

#ifndef _SDL_rotozoom_h
#define _SDL_rotozoom_h

#include <math.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI	3.141592654
#endif

#include "SDL.h"

	/* ---- Defines */

	/*!
	\brief Disable anti-aliasing (no smoothing).
	*/
#define SMOOTHING_OFF		0

	/*!
	\brief Enable anti-aliasing (smoothing).
	*/
#define SMOOTHING_ON		1

	/* ---- Function Prototypes */

#ifdef _MSC_VER
#  if defined(DLL_EXPORT) && !defined(LIBSDL_GFX_DLL_IMPORT)
#    define SDL_ROTOZOOM_SCOPE __declspec(dllexport)
#  else
#    ifdef LIBSDL_GFX_DLL_IMPORT
#      define SDL_ROTOZOOM_SCOPE __declspec(dllimport)
#    endif
#  endif
#endif
#ifndef SDL_ROTOZOOM_SCOPE
#  define SDL_ROTOZOOM_SCOPE extern
#endif

	/* 

	Shrinking functions

	*/     

	SDL_ROTOZOOM_SCOPE SDL_Surface *shrinkSurface(SDL_Surface * src, int factorx, int factory);

#ifdef __cplusplus
}
#endif

#endif				/* _SDL_rotozoom_h */
