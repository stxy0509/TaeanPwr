/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

EGL_FONT *create_bmpfont ( const char *fname );
void release_bmpfont(EGL_FONT* pFont);

int bmpfont_draw (EGL_FONT* pFont, int x, int y, const char *str );
int bmpfont_draw_vleft( EGL_FONT* font,int x, int y, const char *text );
int bmpfont_draw_vright( EGL_FONT* font,int x, int y, const char *text );

/* API, Only for bitmap-font */
typedef enum
{
	NONE,
	UTF8,
	UTF16
} EFontTextEncoding;
void bmpfont_settextencoding (EGL_FONT* pFont, EFontTextEncoding encoding );

bool bmpfont_setkerning( EGL_FONT* pFont, int k );
SURFACE* bmpfont_makesurface( EGL_FONT* pFont,char* text );

bool bmpfont_setautokerning (EGL_FONT* pFont,bool b );


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
