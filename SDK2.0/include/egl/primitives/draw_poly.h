
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

void r_draw_polyline(EGL_POINT* p, int n,EGL_COLOR c);
void r_draw_polyline_aa(EGL_POINT* p, int n,EGL_COLOR c);
void r_draw_polygon(EGL_POINT* ptable,int cnt,EGL_COLOR c);
void r_draw_polygon_aa(EGL_POINT* ptable,int cnt,EGL_COLOR c);
void r_draw_polygonfill(EGL_POINT* ptable,int cnt,EGL_COLOR c);

void draw_polyline(EGL_POINT* p, int n,EGL_COLOR c);
void draw_polyline_aa(EGL_POINT* p, int n,EGL_COLOR c);
void draw_polygon(EGL_POINT* ptable,int cnt,EGL_COLOR c);
void draw_polygon_aa(EGL_POINT* ptable,int cnt,EGL_COLOR c);
void draw_polygonfill(EGL_POINT* ptable,int cnt,EGL_COLOR c);

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
