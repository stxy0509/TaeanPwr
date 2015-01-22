/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif

    typedef struct _tagEGL_CIRCLE_GAUGE
    {
        int needle_x;//center of needle, offset of back-ground image
        int needle_y;//center of needle, offset of back-ground image
        int needle_length;
        int needle_thick;
        int min_value;
        int max_value;
        //clockwise direction
        int min_angle;//0~358
        int max_angle;//1~359
        int cur_value;
        int cur_angle;
		SURFACE* bg;
    } EGL_CIRCLE_GAUGE_DATA;

    typedef struct _tagCIRCLE_GAUGE_INFO
    {
        int needle_x;//center of needle, offset of back-ground image
        int needle_y;//center of needle, offset of back-ground image
        int needle_length;
        int needle_thick;
        int min_value;
        int max_value;
        //clockwise direction
        int min_angle;//0~358
        int max_angle;//1~359
    } EGL_CIRCLE_GAUGE_INFO;

    EGL_HANDLE egl_create_circle_gauge( SURFACE* bg, int x, int y, EGL_CIRCLE_GAUGE_INFO* pInfo );
    BOOL egl_circle_gauge_set_value( EGL_HANDLE h, int value );
    int egl_circle_gauge_get_value( EGL_HANDLE h );


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
