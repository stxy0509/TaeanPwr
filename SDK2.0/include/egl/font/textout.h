#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

int draw_text(EGL_FONT* font,int x, int y, const char* text);
int draw_text_pivot(EGL_FONT* font,int x, int y, const char* text,int pivot);
void draw_text_len(EGL_FONT* font,int x, int y,const char* text, int len);
void draw_text_in_box(EGL_FONT* font,EGL_RECT* pRect,const char* text,int align);
int text_width(EGL_FONT* font,const char* str);//return count in pixels
int text_count_in_width(EGL_FONT* font,const char* str,int w);//return count in bytes

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
