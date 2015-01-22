/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif
#include "egl_base.h"
#include "egl_port.h"


    extern EGL_FONT* egl_sys_font;
    /*
    Not public API
    */
    BOOL prvCollision_detect_rect( EGL_RECT* object1, EGL_RECT* object2 );
    EGL_OBJECT_PTR prv_egl_create_object( int x, int y, int w, int h );
    BOOL prv_egl_delete_object( EGL_OBJECT_PTR pObj );


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
