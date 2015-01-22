#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

	void r_draw_arc2( int x, int y, int rx, int ry,int ax, int ay, int bx, int by, EGL_COLOR c );
	void r_draw_arc(int x, int y, int rx, int ry,int a1, int a2,EGL_COLOR c);
	void r_draw_pie(int x, int y, int rx, int ry,int a1, int a2,EGL_COLOR c);
	void r_draw_piefill(int x, int y, int rx, int ry,int a1, int a2,EGL_COLOR c);
	void r_draw_ellipse(int x, int y, int rx, int ry,EGL_COLOR c);
	void r_draw_ellipsefill(int x, int y, int rx, int ry,EGL_COLOR c);
	void r_draw_circle(int x, int y, int r,EGL_COLOR c);
	void r_draw_circlefill(int x, int y, int r,EGL_COLOR c);
	void r_draw_ellipse_aa(int x, int y, int rx, int ry, EGL_COLOR color);
	void r_draw_bezier(EGL_POINT* pts,int n, int s,EGL_COLOR c);

	void draw_arc2( int x, int y, int rx, int ry,int ax, int ay, int bx, int by, EGL_COLOR c );
	void draw_arc(int x, int y, int rx, int ry,int a1, int a2,EGL_COLOR c);
	void draw_pie(int x, int y, int rx, int ry,int a1, int a2,EGL_COLOR c);
	void draw_piefill(int x, int y, int rx, int ry,int a1, int a2,EGL_COLOR c);
	void draw_ellipse(int x, int y, int rx, int ry,EGL_COLOR c);
	void draw_ellipsefill(int x, int y, int rx, int ry,EGL_COLOR c);
	void draw_circle(int x, int y, int r,EGL_COLOR c);
	void draw_circlefill(int x, int y, int r,EGL_COLOR c);
	void draw_ellipse_aa(int x, int y, int rx, int ry, EGL_COLOR color);
	void draw_bezier(EGL_POINT* pts,int n, int s,EGL_COLOR c);

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
