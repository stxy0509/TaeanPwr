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
        PICTURE_CLICKED = 0,
        PICTURE_MAX,
    } PICTUREEVENT;

    EGL_HANDLE egl_create_picture( SURFACE* surf, int x, int y, int w, int h );
    BOOL egl_picture_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );
    SURFACE* egl_picture_set( EGL_HANDLE hObj, SURFACE* surf ); //return old surface


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
