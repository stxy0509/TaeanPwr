/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

	BOOL theme_init();
	EGL_COLOR theme_get_ctrl_color();
	EGL_COLOR theme_get_ctrl_text_color();
	EGL_COLOR theme_get_wnd_bg_color();

	void theme_draw_frame(int x, int y, int w, int h); //if only frame, call this
	void theme_draw_box(int x, int y, int w, int h,BOOL selected); 
	void theme_draw_button(int x, int y, int w, int h,BOOL selected);
	void theme_draw_scroll_thumb(int x, int y, int w, int h,BOOL vertical,BOOL selected);
	void theme_draw_up_arrow_btn(int x, int y, int w, int h,BOOL selected);
	void theme_draw_down_arrow_btn(int x, int y, int w, int h,BOOL selected);
	void theme_draw_left_arrow_btn(int x, int y, int w, int h,BOOL selected);
	void theme_draw_right_arrow_btn(int x, int y, int w, int h,BOOL selected);
	void theme_draw_radio_btn(int x, int y, int w, int h,BOOL selected);
	void theme_draw_check_btn(int x, int y, int w, int h,BOOL selected);
	int theme_frame_dw();
	int theme_frame_dh();


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
