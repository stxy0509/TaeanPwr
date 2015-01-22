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
        CHECK_CHECKED = 0,
        CHECK_UNCHECKED,
        CHECK_MAX,
    }
    CHECK_EVENT;
    typedef enum
    {
        CHECK_STYLE_CHECKBUTTON = 0,
        CHECK_STYLE_RADIOBUTTON
    } CHECK_STYLE;

    typedef struct _tag_check_data
    {
        EGL_HANDLE label;
        int m_style;
        int m_check_x;
        int m_check_y;
        BOOL m_bChecked;
        SURFACE* surf_check;
    } CHECK_DATA;

    EGL_HANDLE egl_create_checkbutton( int x, int y, int w, int h, const char* text, CHECK_STYLE style );
    BOOL egl_checkbutton_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );

    void egl_checkbutton_set_check( EGL_HANDLE hObj, BOOL b );
    BOOL egl_checkbutton_get_check( EGL_HANDLE hObj );
    void egl_checkbutton_set_style( EGL_HANDLE hObj, int style );

#define CHECK_RADIO_SIZE 32
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
