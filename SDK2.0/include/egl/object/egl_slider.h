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
        SLR_CLICKED = 0,
        SLR_MAX,
    }
    SLIDER_EVENT;

    typedef enum
    {
        TICK_NONE,
        TICK_TOPLEFT,
        TICK_BOTTOMRIGHT,
        TICK_BOTH
    } TICKSTYLE;

    typedef struct _tag_slr_data
    {
        EGL_HANDLE label;
        BOOL m_bVerticalMode;
        EGL_COLOR m_pos_color;
        EGL_COLOR m_bar_color;
        BOOL m_colorless;
        int m_pos;
        int m_minpos;
        int m_maxpos;
        int m_range;
        int m_stepsize;
        int m_tickfrequency;

        int m_bar_box_x;
        int m_bar_box_y;
        int m_bar_box_h;
        int m_bar_box_w;

        int m_thumb_box_x;
        int m_thumb_box_y;
        int m_thumb_box_h;
        int m_thumb_box_w;

        TICKSTYLE m_tickstyle;
        EGL_COLOR m_tick_color;

        // TICK_NONE		: 컨트롤에 눈금 표시가 나타나지 않습니다.
        // TICK_TOPLEFT		: 눈금 표시가 가로 방향컨트롤의 위쪽 또는 세로 방향 컨트롤의 왼쪽에 배치됩니다.
        // TICK_BOTTOMRIGHT : 눈금 표시가 가로 방향 컨트롤의 아래쪽 또는 세로 방향 컨트롤의 오른쪽에 배치됩니다.
        // TICK_BOTH		: 눈금 표시가 컨트롤의 양쪽에 배치됩니다.
    } SLR_DATA;

    EGL_HANDLE egl_create_slider( int x, int y, int w, int h, int range, TICKSTYLE style, BOOL bVertical );
    BOOL egl_slider_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb );

    void egl_slider_set_pos( EGL_HANDLE hObj, int nPos );
    int egl_slider_get_pos( EGL_HANDLE hObj );
    void egl_slider_get_range( EGL_HANDLE hObj, int* IpMinPos, int* IpMaxPos );
    void egl_slider_set_range( EGL_HANDLE hObj, int nMinPos, int nMaxPos );
    void egl_slider_stepit( EGL_HANDLE hObj, BOOL inc );
    void egl_slider_set_tick_frequency( EGL_HANDLE hObj, int freq );
    void egl_slider_set_tick_style( EGL_HANDLE hObj, TICKSTYLE style );
    void egl_slider_set_thumb_size( EGL_HANDLE hObj, int width, int high );
    void egl_slider_get_thumb_size( EGL_HANDLE hObj, int* w, int* h );

    void egl_slider_set_barcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_slider_set_tickcolor( EGL_HANDLE hObj, unsigned char r, unsigned char g, unsigned char b );
    void egl_slider_set_transparent( EGL_HANDLE hObj, BOOL bflag );

#define SLR_HORIZONTAL 0
#define SLR_VERTICAL 1

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
