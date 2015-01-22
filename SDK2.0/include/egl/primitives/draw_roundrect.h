
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

	void r_draw_roundrect(int x0, int y0, int w, int h, int corner ,EGL_COLOR c);
	void draw_roundrect(int x0, int y0, int w, int h, int corner ,EGL_COLOR c);

	void r_draw_roundrectfill(int x0, int y0, int w, int h, int corner ,EGL_COLOR c);
	void draw_roundrectfill(int x0, int y0, int w, int h, int corner ,EGL_COLOR c);

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
