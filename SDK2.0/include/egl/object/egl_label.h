/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif

    typedef struct _tag_label_data
    {
        char* str;
        int align;
		EGL_COLOR font_clr;
		BOOL bErase_bg;//redraw back ground, default is FALSE
		BOOL selfrecusive;
    } LABEL_DATA;

    EGL_HANDLE egl_create_label( int x, int y, int w, int h, const char* text );
	BOOL egl_label_set_text(EGL_HANDLE h,char* text);
	void egl_label_set_redraw_bg(EGL_HANDLE h,BOOL b);
	void egl_label_set_color(EGL_HANDLE h, EGL_COLOR clr);

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
