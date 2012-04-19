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

#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <string.h>

#include "SDL_rotozoom.h"

/* ---- Internally used structures */

/*!
\brief A 32 bit RGBA pixel.
*/
typedef struct tColorRGBA {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} tColorRGBA;

/*!
\brief A 8bit Y/palette pixel.
*/
typedef struct tColorY {
	Uint8 y;
} tColorY;

/*! 
\brief Returns maximum of two numbers a and b.
*/
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))

/*! 
\brief Number of guard rows added to destination surfaces.

This is a simple but effective workaround for observed issues.
These rows allocate extra memory and are then hidden from the surface.
Rows are added to the end of destination surfaces when they are allocated. 
This catches any potential overflows which seem to happen with 
just the right src image dimensions and scale/rotation and can lead
to a situation where the program can segfault.
*/
#define GUARD_ROWS (2)

/*!
\brief Lower limit of absolute zoom factor or rotation degrees.
*/
#define VALUE_LIMIT	0.001

/*!
\brief Returns colorkey info for a surface
*/
Uint32 _colorkey(SDL_Surface *src)
{
	Uint32 key = 0; 
#if (SDL_MINOR_VERSION == 3)
 	SDL_GetColorKey(src, &key);
#else
 	if (src) 
 	{
 		key = src->format->colorkey;
	}
#endif
 	return key;
}


/*! 
\brief Internal 32 bit integer-factor averaging Shrinker.

Shrinks 32 bit RGBA/ABGR 'src' surface to 'dst' surface.
Averages color and alpha values values of src pixels to calculate dst pixels.
Assumes src and dst surfaces are of 32 bit depth.
Assumes dst surface was allocated with the correct dimensions.

\param src The surface to shrink (input).
\param dst The shrunken surface (output).
\param factorx The horizontal shrinking ratio.
\param factory The vertical shrinking ratio.

\return 0 for success or -1 for error.
*/
int _shrinkSurfaceRGBA(SDL_Surface * src, SDL_Surface * dst, int factorx, int factory)
{
	int x, y, dx, dy, sgap, dgap, ra, ga, ba, aa;
	int n_average;
	tColorRGBA *sp, *osp, *oosp;
	tColorRGBA *dp;

	/*
	* Averaging integer shrink
	*/

	/* Precalculate division factor */
	n_average = factorx*factory;

	/*
	* Scan destination
	*/
	sp = (tColorRGBA *) src->pixels;
	sgap = src->pitch - src->w * 4;

	dp = (tColorRGBA *) dst->pixels;
	dgap = dst->pitch - dst->w * 4;

	for (y = 0; y < dst->h; y++) {

		osp=sp;
		for (x = 0; x < dst->w; x++) {

			/* Trace out source box and accumulate */
			oosp=sp;
			ra=ga=ba=aa=0;
			for (dy=0; dy < factory; dy++) {
				for (dx=0; dx < factorx; dx++) {
					ra += sp->r;
					ga += sp->g;
					ba += sp->b;
					aa += sp->a;

					sp++;
				} 
				/* src dx loop */
				sp = (tColorRGBA *)((Uint8*)sp + (src->pitch - 4*factorx)); // next y
			}
			/* src dy loop */

			/* next box-x */
			sp = (tColorRGBA *)((Uint8*)oosp + 4*factorx);

			/* Store result in destination */
			dp->r = ra/n_average;
			dp->g = ga/n_average;
			dp->b = ba/n_average;
			dp->a = aa/n_average;

			/*
			* Advance destination pointer 
			*/
			dp++;
		} 
		/* dst x loop */

		/* next box-y */
		sp = (tColorRGBA *)((Uint8*)osp + src->pitch*factory);

		/*
		* Advance destination pointers 
		*/
		dp = (tColorRGBA *) ((Uint8 *) dp + dgap);
	} 
	/* dst y loop */

	return (0);
}

/*! 
\brief Internal 8 bit integer-factor averaging shrinker.

Shrinks 8bit Y 'src' surface to 'dst' surface.
Averages color (brightness) values values of src pixels to calculate dst pixels.
Assumes src and dst surfaces are of 8 bit depth.
Assumes dst surface was allocated with the correct dimensions.

\param src The surface to shrink (input).
\param dst The shrunken surface (output).
\param factorx The horizontal shrinking ratio.
\param factory The vertical shrinking ratio.

\return 0 for success or -1 for error.
*/
int _shrinkSurfaceY(SDL_Surface * src, SDL_Surface * dst, int factorx, int factory)
{
	int x, y, dx, dy, sgap, dgap, a;
	int n_average;
	Uint8 *sp, *osp, *oosp;
	Uint8 *dp;

	/*
	* Averaging integer shrink
	*/

	/* Precalculate division factor */
	n_average = factorx*factory;

	/*
	* Scan destination
	*/
	sp = (Uint8 *) src->pixels;
	sgap = src->pitch - src->w;

	dp = (Uint8 *) dst->pixels;
	dgap = dst->pitch - dst->w;

	for (y = 0; y < dst->h; y++) {    

		osp=sp;
		for (x = 0; x < dst->w; x++) {

			/* Trace out source box and accumulate */
			oosp=sp;
			a=0;
			for (dy=0; dy < factory; dy++) {
				for (dx=0; dx < factorx; dx++) {
					a += (*sp);
					/* next x */           
					sp++;
				} 
				/* end src dx loop */         
				/* next y */
				sp = (Uint8 *)((Uint8*)sp + (src->pitch - factorx)); 
			} 
			/* end src dy loop */

			/* next box-x */
			sp = (Uint8 *)((Uint8*)oosp + factorx);

			/* Store result in destination */
			*dp = a/n_average;

			/*
			* Advance destination pointer 
			*/
			dp++;
		} 
		/* end dst x loop */

		/* next box-y */
		sp = (Uint8 *)((Uint8*)osp + src->pitch*factory);

		/*
		* Advance destination pointers 
		*/
		dp = (Uint8 *)((Uint8 *)dp + dgap);
	} 
	/* end dst y loop */

	return (0);
}

/*! 
\brief Shrink a surface by an integer ratio using averaging.

Shrinks a 32bit or 8bit 'src' surface to a newly created 'dst' surface.
'factorx' and 'factory' are the shrinking ratios (i.e. 2=1/2 the size,
3=1/3 the size, etc.) The destination surface is antialiased by averaging
the source box RGBA or Y information. If the surface is not 8bit
or 32bit RGBA/ABGR it will be converted into a 32bit RGBA format on the fly.
The input surface is not modified. The output surface is newly allocated.

\param src The surface to shrink.
\param factorx The horizontal shrinking ratio.
\param factory The vertical shrinking ratio.

\return The new, shrunken surface.
*/
/*@null@*/ 
SDL_Surface *shrinkSurface(SDL_Surface *src, int factorx, int factory)
{
	int result;
	SDL_Surface *rz_src;
	SDL_Surface *rz_dst = NULL;
	int dstwidth, dstheight;
	int is32bit;
	int i, src_converted;
	int haveError = 0;

	/*
	* Sanity check 
	*/
	if (src == NULL) {
		return (NULL);
	}

	/*
	* Determine if source surface is 32bit or 8bit 
	*/
	is32bit = (src->format->BitsPerPixel == 32);
	if ((is32bit) || (src->format->BitsPerPixel == 8)) {
		/*
		* Use source surface 'as is' 
		*/
		rz_src = src;
		src_converted = 0;
	} else {
		/*
		* New source surface is 32bit with a defined RGBA ordering 
		*/
		rz_src = SDL_CreateRGBSurface(SDL_SWSURFACE, src->w, src->h, 32, 
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#else
			0xff000000,  0x00ff0000, 0x0000ff00, 0x000000ff
#endif
			);
		if (rz_src==NULL) {
			haveError = 1;
			goto exitShrinkSurface;
		}
		
		SDL_BlitSurface(src, NULL, rz_src, NULL);
		src_converted = 1;
		is32bit = 1;
	}

	/*
	* Lock the surface 
	*/
	if (SDL_MUSTLOCK(rz_src)) {
		if (SDL_LockSurface(rz_src) < 0) {
			haveError = 1;
			goto exitShrinkSurface;
		}
	}

	/* Get size for target */
	dstwidth=rz_src->w/factorx;
	while (dstwidth*factorx>rz_src->w) { dstwidth--; }
	dstheight=rz_src->h/factory;
	while (dstheight*factory>rz_src->h) { dstheight--; }

	/*
	* Alloc space to completely contain the shrunken surface
	* (with added guard rows)
	*/
	if (is32bit==1) {
		/*
		* Target surface is 32bit with source RGBA/ABGR ordering 
		*/
		rz_dst =
			SDL_CreateRGBSurface(SDL_SWSURFACE, dstwidth, dstheight + GUARD_ROWS, 32,
			rz_src->format->Rmask, rz_src->format->Gmask,
			rz_src->format->Bmask, rz_src->format->Amask);
	} else {
		/*
		* Target surface is 8bit 
		*/
		rz_dst = SDL_CreateRGBSurface(SDL_SWSURFACE, dstwidth, dstheight + GUARD_ROWS, 8, 0, 0, 0, 0);
	}
	
	/* Check target */
	if (rz_dst == NULL) {
		haveError = 1;
		goto exitShrinkSurface;
	}

	/* Adjust for guard rows */
	rz_dst->h = dstheight;

	/*
	* Check which kind of surface we have 
	*/
	if (is32bit==1) {
		/*
		* Call the 32bit transformation routine to do the shrinking (using alpha) 
		*/
		result = _shrinkSurfaceRGBA(rz_src, rz_dst, factorx, factory);		
		if ((result!=0) || (rz_dst==NULL)) {
			haveError = 1;
			goto exitShrinkSurface;
		}
		
		/*
		* Turn on source-alpha support 
		*/
		result = SDL_SetAlpha(rz_dst, SDL_SRCALPHA, 255);
		if (result!=0) {
			haveError = 1;
			goto exitShrinkSurface;
		}
	} else {
		/*
		* Copy palette and colorkey info 
		*/
		for (i = 0; i < rz_src->format->palette->ncolors; i++) {
			rz_dst->format->palette->colors[i] = rz_src->format->palette->colors[i];
		}
		rz_dst->format->palette->ncolors = rz_src->format->palette->ncolors;
		/*
		* Call the 8bit transformation routine to do the shrinking 
		*/
		result = _shrinkSurfaceY(rz_src, rz_dst, factorx, factory);
		if (result!=0) {
			haveError = 1;
			goto exitShrinkSurface;
		}
		
		/*
		* Set colorkey on target
		*/
		result = SDL_SetColorKey(rz_dst, SDL_SRCCOLORKEY | SDL_RLEACCEL, _colorkey(rz_src));
		if (result!=0) {
			haveError = 1;
			goto exitShrinkSurface;
		}		
	}

exitShrinkSurface:
	if (rz_src!=NULL) {
		/*
		* Unlock source surface 
		*/
		if (SDL_MUSTLOCK(rz_src)) {
			SDL_UnlockSurface(rz_src);
		}

		/*
		* Cleanup temp surface 
		*/
		if (src_converted==1) {
			SDL_FreeSurface(rz_src);
		}
	}
	
	/* Check error state; maybe need to cleanup destination */
	if (haveError==1) {
		if (rz_dst!=NULL) {
			SDL_FreeSurface(rz_dst);
		}
		rz_dst=NULL;
	} 
	
	/*
	* Return destination surface 
	*/
	return (rz_dst);
}
