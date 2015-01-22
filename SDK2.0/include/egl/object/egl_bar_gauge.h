/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif

	typedef struct _tagEGL_BAR_GAUGE
	{
		int min_value;
		int max_value;
		int cur_value;
		int tickmarksize; //line length
		int tikcmarkstep;
		int tikcmarkmajorstep;
		int margin;
		int pointer_size;
		int ppv;//pixel per value, real = ppv/10000
		BOOL bVertical;
		SURFACE* bg;
		SURFACE* surf_pointer;
	} EGL_BAR_GAUGE_DATA;

	typedef struct _tagBAR_GAUGE_INFO
	{
		int min_value;
		int max_value;
		int tickmarksize; //line length
		int tickmarkstep;
		int tickmarkmajorstep;
		BOOL bVertical;
		SURFACE* bg;
	} EGL_BAR_GAUGE_INFO;

	EGL_HANDLE egl_create_bar_gauge( int x, int y, int w, int h, EGL_BAR_GAUGE_INFO* pInfo );
	BOOL egl_bar_gauge_set_value( EGL_HANDLE h, int value );
	int egl_bar_gauge_get_value( EGL_HANDLE h );


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
