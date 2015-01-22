#include "egl_typedef.h"
/*
 * A function to calculate the intersection of two rectangles:
 * return true if the rectangles intersect, false otherwise
 */
BOOL intersect_rect(EGL_RECT* dest,EGL_RECT* src,EGL_RECT* intersection)
{
	int Amin, Amax, Bmin, Bmax;

	/* Horizontal intersection */
	Amin = dest->x;
	Amax = Amin + dest->w;
	Bmin = src->x;
	Bmax = Bmin + src->w;
	if(Bmin > Amin)
	        Amin = Bmin;
	intersection->x = Amin;
	if(Bmax < Amax)
	        Amax = Bmax;
	intersection->w = Amax - Amin > 0 ? Amax - Amin : 0;

	/* Vertical intersection */
	Amin = dest->y;
	Amax = Amin + dest->h;
	Bmin = src->y;
	Bmax = Bmin + src->h;
	if(Bmin > Amin)
	        Amin = Bmin;
	intersection->y = Amin;
	if(Bmax < Amax)
	        Amax = Bmax;
	intersection->h = Amax - Amin > 0 ? Amax - Amin : 0;

	return (intersection->w && intersection->h);
}
