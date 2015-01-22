
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

void r_draw_thickline( int x1, int y1, int x2, int y2, U8 width, EGL_COLOR color );
void draw_thickline( int x1, int y1, int x2, int y2, U8 width, EGL_COLOR color );

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
