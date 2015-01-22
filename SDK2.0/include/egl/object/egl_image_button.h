/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif

	typedef struct _tag_image_btn_data
	{
		BOOL bPressed;
		SURFACE* pressedsurface;
		BOOL bRecursive;
	} IMAGE_BTN_DATA;

	EGL_HANDLE egl_create_image_button( SURFACE* img,SURFACE* pressed_img,int x, int y, int w, int h );
	BOOL egl_image_button_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
