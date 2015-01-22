/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif

    typedef enum
    {
		BTN_CLICKED = 0,
		BTN_PRESSED,
        BTN_MAX,
    }BUTTON_EVENT;

    typedef struct _tag_btn_data
    {
        EGL_HANDLE label;
        BOOL bPressed;
#if EGL_CONFIG_OBJECT_HAVE_SELF_SURFACE==1
        SURFACE* pressedsurface;
#endif  //EGL_CONFIG_OBJECT_HAVE_SELF_SURFACE

    } BTN_DATA;

    EGL_HANDLE egl_create_button( int x, int y, int w, int h, const char* text );
    BOOL egl_button_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
