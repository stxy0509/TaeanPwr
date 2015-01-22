#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

typedef struct _tagEGL_Font 
{
	int (*draw)(struct _tagEGL_Font* pFont,int x, int y,const char* str);
	int (*draw_pivot)(struct _tagEGL_Font* pFont,int x, int y,const char* str,int pivot);//PIVOT_RIGHT or PIVOT_LEFT
	void (*draw_len)(struct _tagEGL_Font* pFont,int x, int y,const char* str,int len);
	int (*text_width)(struct _tagEGL_Font* pFont,const char* str);//return count in pixels
	int (*text_count_in_width)(struct _tagEGL_Font* pFont,const char* str,int w);//return byte count, not character count
	EGL_COLOR (*set_color)(struct _tagEGL_Font* pFont,EGL_COLOR color);
	EGL_COLOR (*set_bkcolor)(struct _tagEGL_Font* pFont,EGL_COLOR color);
	EGL_COLOR (*get_color)(struct _tagEGL_Font* pFont);
	EGL_COLOR (*get_bkcolor)(struct _tagEGL_Font* pFont);
	void (*set_bkmode)(struct _tagEGL_Font* pFont,int mode);
	int h;//read only
	void* ext;//used by internal function
}EGL_FONT;

EGL_FONT* egl_create_default_font();//called by ut_init()
EGL_FONT* egl_create_font_from_font(EGL_FONT* font);//return clone from exist font
void egl_font_set_bkmode(EGL_FONT* font,int mode);
EGL_COLOR egl_font_set_bk_color(EGL_FONT* font,EGL_COLOR clr);
EGL_COLOR egl_font_get_color(EGL_FONT* font);
EGL_COLOR egl_font_set_color(EGL_FONT* font,EGL_COLOR clr);


#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
