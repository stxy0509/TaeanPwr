
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

void r_draw_rect(int x, int y, int w, int h,EGL_COLOR c);
void r_draw_rectfill(int x, int y, int w, int h,EGL_COLOR c);
void r_draw_rectfill_v_gradient(int x, int y, int w, int h,EGL_COLOR startcolor,EGL_COLOR endcolor);
void r_draw_rectfill_h_gradient(int x, int y, int w, int h,EGL_COLOR startcolor,EGL_COLOR endcolor);
void r_draw_rectfill_gradient(int x, int y, int w, int h,EGL_COLOR startcolor,EGL_COLOR endcolor,BOOL bVertical);

void draw_rect(int x, int y, int w, int h,EGL_COLOR c);
void draw_rectfill(int x, int y, int w, int h,EGL_COLOR c);
void draw_rectfill_v_gradient(int x, int y, int w, int h,EGL_COLOR startcolor,EGL_COLOR endcolor);
void draw_rectfill_h_gradient(int x, int y, int w, int h,EGL_COLOR startcolor,EGL_COLOR endcolor);
void draw_rectfill_gradient(int x, int y, int w, int h,EGL_COLOR startcolor,EGL_COLOR endcolor,BOOL bVertical);

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
