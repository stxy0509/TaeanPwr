
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

void r_draw_hline(int x, int y, int x2,EGL_COLOR c);
void r_draw_vline(int x, int y, int y2,EGL_COLOR c);
void r_draw_line(int x, int y, int x2, int y2,EGL_COLOR c);
void r_draw_line_aa(int x, int y, int x2, int y2,EGL_COLOR c);

//call r_xxx and data cache invalidation
void draw_hline(int x, int y, int x2,EGL_COLOR c);
void draw_vline(int x, int y, int y2,EGL_COLOR c);
void draw_line(int x, int y, int x2, int y2,EGL_COLOR c);
void draw_line_aa(int x, int y, int x2, int y2,EGL_COLOR c);
/*
anti-aliased line
*/


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
