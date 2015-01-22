#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include "font/textout.h"

#include <stdlib.h>
#include <string.h>

void draw_text_in_box(EGL_FONT* font,EGL_RECT* pRect,const char* text,int align)
{
	int strleninpixel;
	int charheight;
	int x,y,w,h,startx,starty;
	int font_len[10];
	int Draw_xPixel=0;
	int Draw_LineCnt=0;
	int Draw_FontCnt;
	int Draw_CurrentFontCnt=0;
	int eraselen;
	unsigned char temp_u8;
	int temp_s32;

	x = pRect->x;
	y = pRect->y;
	w = pRect->w;
	h = pRect->h;
	strleninpixel = text_width(font,text);
	charheight = font->h;

	if(strleninpixel == 0 || charheight == 0 || charheight > h)
	{
		return ;
	}
	if(strleninpixel > w)
	{
		Draw_FontCnt = 0;
		do
		{
			int len = text_count_in_width(font,text+Draw_FontCnt,w);
			if(len==0)
				break;
			font_len[Draw_LineCnt] = len;
			Draw_FontCnt += len;
			Draw_LineCnt++;

		}while(1);
	}
	else
	{
		Draw_xPixel = strleninpixel;
		Draw_LineCnt = 1;
		font_len[0] = text_count_in_width(font,text,w);
	}

	Draw_CurrentFontCnt = 0;	
	Draw_FontCnt = 0;

	if((align&EGL_ALIGN_MULTI_LINE) != EGL_ALIGN_MULTI_LINE)
	{
		Draw_LineCnt = 1;
	}

	if(align&EGL_ALIGN_TOP)
	{
		// TOP
		starty = y;
	}
	else if(align&EGL_ALIGN_BOTTOM)
	{
		// BOTTOM
		starty = (y+h)-(Draw_LineCnt*charheight);
	}
	else
	{
		// CENTER
		starty = y+(h/2)-((Draw_LineCnt*charheight)/2);
	}

	do{		
		if(Draw_LineCnt == 1)
		{
			Draw_xPixel = text_width(font,text+Draw_FontCnt);
			if(Draw_xPixel < w)
			{
				if(align&EGL_ALIGN_LEFT)
				{
					// LEFT
					// x point fixed
					startx = x;
				}
				else if(align&EGL_ALIGN_CENTER)
				{
					// CENTER
					startx = (x+(w/2)) - (Draw_xPixel/2);
				}		
				else
				{
					// RIGHT
					startx = (x+w) - Draw_xPixel;
				}
			}
			else
			{
				startx = x;
			}
		}
		else
		{
			startx = x;
		}
		Draw_LineCnt--;

		if(align&EGL_ALIGN_MULTI_LINE)
		{
			draw_text_len(font,startx,starty,text+Draw_FontCnt,font_len[Draw_CurrentFontCnt]);

			Draw_FontCnt += font_len[Draw_CurrentFontCnt++];
			starty += charheight;
		}
		else
		{
			if(Draw_xPixel > w)
			{
				temp_u8 = *(text+font_len[Draw_CurrentFontCnt]-1);
				if(temp_u8 < 0x80)
				{
					eraselen = 1;
					temp_u8 = *(text+font_len[Draw_CurrentFontCnt]-2);
					if(temp_u8 < 0x80)
					{
						eraselen += 1;
						temp_u8 = *(text+font_len[Draw_CurrentFontCnt]-2);
						if(temp_u8 >= 0x80)
						{
							eraselen += 2;
						}
					}
					else
					{
						eraselen += 2;
						temp_u8 = *(text+font_len[Draw_CurrentFontCnt]-2);
						if(temp_u8 < 0x80)
						{
							eraselen += 1;
						}
					}
				}
				else
				{
					eraselen = 2;
					temp_u8 = *(text+font_len[Draw_CurrentFontCnt]-2);
					if(temp_u8 < 0x80)
					{
						eraselen += 1;
					}
					else
					{
						eraselen += 2;
					}
				}
				draw_text_len(font,startx,starty,text,font_len[Draw_CurrentFontCnt]-eraselen);
				temp_s32 = text_width(font,text+font_len[Draw_CurrentFontCnt]-eraselen);
				draw_text_len(font,startx+(strleninpixel-temp_s32),starty,"..",2);	
			}
			else
			{
				draw_text_len(font,startx,starty,text,font_len[Draw_CurrentFontCnt]);
			}
		}

	}while(Draw_LineCnt);

}
