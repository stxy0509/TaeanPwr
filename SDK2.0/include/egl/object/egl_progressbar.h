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
        PGBAR_MAX,
    }
    PROGRESSBAR_EVENT;

    typedef enum
    {
        PGBAR_STYLE_NOMAL = 0,
        PGBAR_STYLE_DIV,
    } PROGRESSBAR_STYLE;

    typedef struct _tag_pgbar_data
    {
        EGL_HANDLE label;
        int m_pos;
        int m_value;
        int m_bar_width;
        PROGRESSBAR_STYLE m_style;
        BOOL m_bVertical;
        EGL_COLOR m_pos_color;
        EGL_COLOR m_bg_color;
        EGL_COLOR m_text_color;
    } PGBAR_DATA;

    EGL_HANDLE egl_create_progressbar( int x, int y, int w, int h, const char* text, PROGRESSBAR_STYLE style, BOOL bVertical );
    BOOL egl_progressbar_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );

    void egl_progressbar_set_barcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_progressbar_set_bgcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_progressbar_set_textcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_progressbar_set_pos( EGL_HANDLE hObj, int value ); // value = 0 ~ 100
    int egl_progressbar_get_pos( EGL_HANDLE hObj );
    void egl_progressbar_set_text( EGL_HANDLE hObj, const char* text );


#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif

