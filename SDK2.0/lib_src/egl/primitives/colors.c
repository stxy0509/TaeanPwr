#include "egl_typedef.h"
#include "primitives/colors.h"

/**
Returns a color that contrasts with the background color.
This will be the foreground color if it contrasts sufficiently with the
background color. Otherwise, returns \p FL_WHITE or \p FL_BLACK depending
on which color provides the best contrast.
\param[in] fg,bg foreground and background colors
\return contrasting color
*/
EGL_COLOR color_contrast ( EGL_COLOR fg, EGL_COLOR bg )
{
	unsigned c1, c2;	// RGB colors
	int l1, l2;		// Luminosities

	c1 = ( unsigned ) fg;
	c2 = ( unsigned ) bg;

	// Compute the luminosity...
	l1 = ( ( c1 >> 16 ) * 30 + ( ( c1 >> 8 ) & 255 ) * 59 + ( ( c1 >> 0 ) & 255 ) * 11 ) / 100;
	l2 = ( ( c2 >> 16 ) * 30 + ( ( c2 >> 8 ) & 255 ) * 59 + ( ( c2 >> 0 ) & 255 ) * 11 ) / 100;

	// Compare and return the contrasting color...
	if ( ( l1 - l2 ) > 99 ) return fg;
	else if ( ( l2 - l1 ) > 99 ) return fg;
	else if ( l2 > 127 ) return 0;
	else return MAKE_COLORREF(255,255,255);
}

EGL_COLOR color_average( EGL_COLOR color1, EGL_COLOR color2, U8 weight )
{
	U16 r,g,b;
	r = ( ( ( uchar ) ( color1 >> 16 ) ) * weight + ( ( uchar ) ( color2 >> 16 ) ) * ( 255 - weight ) );
	r= r>>8;
	g = ( ( ( uchar ) ( color1 >> 8 ) ) * weight + ( ( uchar ) ( color2 >> 8 ) ) * ( 255 - weight ) );
	g= g>>8;
	b = ( ( ( uchar ) ( color1  ) ) * weight + ( ( uchar ) ( color2 ) ) * ( 255 - weight ) );
	b= b>>8;
	return MAKE_COLORREF ( r, g, b );
}

