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
        TOGGLE_IMAGE_ON = 0,
        TOGGLE_IMAGE_OFF,
    } TOGGLE_IMAGE_EVENT;

    typedef struct _tagToggleImage
    {
        BOOL bOn;
        SURFACE* surf_on;
    } TOGGLE_IMAGE_DATA;

    EGL_HANDLE egl_create_toggle_image( SURFACE* surf_off, SURFACE* surf_on, int x, int y, int w, int h );
    BOOL egl_toggle_image_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );
	BOOL egl_toggle_image_set_on(EGL_HANDLE hObj,BOOL b);


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
