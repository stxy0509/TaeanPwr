/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif

	void egl_user_key_input( BOOL bKeyDown, U32 val );
	void egl_user_touch_input( BOOL bPressed, EGL_POINT* pt );
	void egl_draw();
	EGL_HANDLE egl_create_window( const char* title );
	BOOL egl_window_show( EGL_HANDLE hWin, BOOL bShow );
	BOOL egl_window_isshow( EGL_HANDLE hWin);
	void egl_window_invalidate();
	void egl_window_invalidate_rect( EGL_RECT* pRect );
	void egl_window_redraw_rect( EGL_RECT* pRect );
	BOOL egl_window_set_bg( EGL_HANDLE hWin, SURFACE* Img );
	SURFACE* egl_window_get_bg( EGL_HANDLE hWin );
	BOOL egl_window_add_object( EGL_HANDLE hWin, EGL_HANDLE hObj );
	void egl_window_set_callback( EGL_HANDLE hWin, EVENT_CALLBACK cb );
	EGL_HANDLE egl_window_get_active();


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
