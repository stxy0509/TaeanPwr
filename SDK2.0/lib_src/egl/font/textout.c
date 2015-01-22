#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include "font/textout.h"

#include <stdlib.h>
#include <string.h>


int draw_text(EGL_FONT* font,int x, int y, const char* text)
{
	return font->draw(font,x,y,text);
}

int draw_text_pivot(EGL_FONT* font,int x, int y, const char* text,int pivot)
{
	return font->draw_pivot(font,x,y,text,pivot);
}

void draw_text_len(EGL_FONT* font,int x, int y,const char* text, int len)
{
	font->draw_len(font,x,y,text,len);
}
//return count in pixels
int text_width(EGL_FONT* font,const char* str)
{
	return font->text_width(font,str);
}
int text_count_in_width(EGL_FONT* font,const char* str,int w)
{
	return font->text_count_in_width(font,str,w);
}
