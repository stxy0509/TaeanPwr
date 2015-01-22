/******************************************************************************
 Copyright (C) 2013      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

EGL_FONT *create_bitfont ();
void release_bitfont(EGL_FONT* pFont);

int bitfont_draw (EGL_FONT* pFont, int x, int y, const char *str );
int bitfont_draw_vleft( EGL_FONT* font,int x, int y, const char *text );
int bitfont_draw_vright( EGL_FONT* font,int x, int y, const char *text );
SURFACE* bitfont_makesurface( EGL_FONT* pFont,char* text );


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
