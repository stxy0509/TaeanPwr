
#include "egl_typedef.h"
#include "egl_base.h"

void egl_font_set_bkmode(EGL_FONT* font,int mode)
{
	font->set_bkmode(font,mode);
}
EGL_COLOR egl_font_get_bk_color(EGL_FONT* font)
{
	return font->get_bkcolor(font);
}
EGL_COLOR egl_font_set_bk_color(EGL_FONT* font,EGL_COLOR clr)
{
	return font->set_bkcolor(font,clr);
}
EGL_COLOR egl_font_get_color(EGL_FONT* font)
{
	return font->get_color(font);
}
EGL_COLOR egl_font_set_color(EGL_FONT* font,EGL_COLOR clr)
{
	return font->set_color(font,clr);
}
