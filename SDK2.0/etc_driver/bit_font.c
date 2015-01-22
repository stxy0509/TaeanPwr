/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.
 
 English and Hangul Font driver(bit array data)

******************************************************************************/

#include "adstar.h"
#include "etc_driver/bit_font.h"

static SURFACE* engchar_surf=0;
static SURFACE* engchar_surf_vert=0;
static SURFACE* hanchar_surf=0;
static SURFACE* hanchar_surf_vert=0;
static int bit_pivot_mode = 0;//1 is left pivot, 2 is right pivot

#define FONT_SIZE_24
#ifdef FONT_SIZE_24
	#include "font/ascii_font16x24.c"
	#include "font/kor_font24x24.c"
	const U8 *engfont = EngFont16x24;
	const U8 *hanfont = HanFont24x24;

	static U8 eng_font_w=16;
	static U8 eng_font_h=24;
	static U8 han_font_w=24;
	static U8 han_font_h=24;

	static int user_eng_font_w=16;
	static int user_eng_font_h=24;
	static int user_han_font_w=24;
	static int user_han_font_h=24;

#else 
	#include "font/ascii_font8x16.c"
	#include "font/kor_font16x16.c"
	const U8 *engfont = EngFont8x16;
	const U8 *hanfont = HanFont16x16;
	static U8 eng_font_w=8;
	static U8 eng_font_h=16;
	static U8 han_font_w=16;
	static U8 han_font_h=16;
	static int user_eng_font_w=6;
	static int user_eng_font_h=16;
	static int user_han_font_w=16;
	static int user_han_font_h=16;
#endif
#include "font/kor_font_index.c"

void bf_setrgb(U8 r, U8 g, U8 b)
{
	sARGB argb;
	argb.a=255;
	argb.r=r;
	argb.g=g;
	argb.b=b;
	engchar_surf->pal->colors[1]=argb;
	engchar_surf_vert->pal->colors[1]=argb;
	hanchar_surf->pal->colors[1]=argb;
	hanchar_surf_vert->pal->colors[1]=argb;
}

void bf_setsize(int eng_w, int eng_h,int kor_w, int kor_h)
{
	user_eng_font_w=eng_w;
	user_eng_font_h=eng_h;
	user_han_font_w=kor_w;
	user_han_font_h=kor_h;
}
static void make_char_eng(int index)
{
	int i;
	int k;
	index-=0x20;
	
	if(bit_pivot_mode==1)
	{
		memset(engchar_surf_vert->pixels,0,engchar_surf_vert->pitch*engchar_surf_vert->h);
		for(i=0;i<engchar_surf_vert->w;i++)
		{
			U8 *dest = (U8 *)engchar_surf_vert->pixels + (engchar_surf_vert->pitch*(engchar_surf_vert->h-1)) + i;
			U8 *src=(U8*)engfont+(index*(eng_font_w/8 * eng_font_h)) + (i*(eng_font_w/8));
			for(k=0;k<engchar_surf_vert->h;k++)
			{
				if( src[k/8] & (1<<(7-(k%8))))
					*dest=1;
				dest-=engchar_surf_vert->pitch;
			}
		}
	}
	else if(bit_pivot_mode==2)
	{
		memset(engchar_surf_vert->pixels,0,engchar_surf_vert->pitch*engchar_surf_vert->h);
		for(i=0;i<engchar_surf_vert->w;i++)
		{
			U8 *dest = (U8 *)engchar_surf_vert->pixels + engchar_surf_vert->pitch -1 - i;
			U8 *src=(U8*)engfont+(index*(eng_font_w/8 * eng_font_h)) + (i*(eng_font_w/8));
			for(k=0;k<engchar_surf_vert->h;k++)
			{
				if( src[k/8] & (1<<(7-(k%8))))
					*dest=1;
				dest+=engchar_surf_vert->pitch;
			}
		}
	}
	else
	{
		memset(engchar_surf->pixels,0,engchar_surf->pitch*engchar_surf->h);

		for(i=0;i<engchar_surf->h;i++)
		{
			U8 *dest = (U8 *)engchar_surf->pixels + engchar_surf->pitch*i;
			U8 *src=(U8*)engfont+(index*(eng_font_w/8 * eng_font_h)) + (i*(eng_font_w/8));
			for(k=0;k<engchar_surf->w;k++)
			{
				if( src[k/8] & (1<<(7-(k%8))))
					*dest=1;
				dest++;
			}
		}
	}
}

static int _draw_char_eng(int x, int y, int index)
{
	make_char_eng(index);
	if((user_eng_font_w != eng_font_w ) || (user_eng_font_h != eng_font_h ) )
	{
		if(bit_pivot_mode==0)
			drawsurfacescalerect(engchar_surf,x,y,user_eng_font_w,user_eng_font_h,0,0,engchar_surf->w,engchar_surf->h);
		else
			drawsurfacescalerect(engchar_surf_vert,x,y,user_eng_font_h,user_eng_font_w,0,0,engchar_surf_vert->w,engchar_surf_vert->h);
			
	}
	else
	{
		if(bit_pivot_mode==0)
			drawsurface(engchar_surf,x,y);
		else
			drawsurface(engchar_surf_vert,x,y);
			
	}
	return user_eng_font_w;
}

static void make_char_han(int index)
{
	int cho,jung,jong;
	int i,k;
	cho = HanIndexTbl[index*3];
	jung = HanIndexTbl[index*3+1]+161;
	jong = HanIndexTbl[index*3+2];
	if(jong)
		jong += 248;

	if(bit_pivot_mode==1)
	{
		//clear
		memset(hanchar_surf_vert->pixels,0,hanchar_surf_vert->pitch*hanchar_surf_vert->h);
		for(i=0;i<hanchar_surf_vert->w;i++)
		{
			U8 *src ;
			if(cho)
			{
				U8 *dest = (U8 *)hanchar_surf_vert->pixels + (hanchar_surf_vert->pitch*(hanchar_surf_vert->h-1)) + i;
				src = (U8*)hanfont+(cho * han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf_vert->h;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						*dest=1;
					dest -= engchar_surf_vert->pitch;
				}
			}

			if(jung)
			{
				U8 *dest = (U8 *)hanchar_surf_vert->pixels + (hanchar_surf_vert->pitch*(hanchar_surf_vert->h-1)) + i;
				src = (U8*)hanfont+(jung* han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf_vert->h;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						*dest=1;
					dest -= engchar_surf_vert->pitch;
				}
			}
			if(jong)
			{
				U8 *dest = (U8 *)hanchar_surf_vert->pixels + (hanchar_surf_vert->pitch*(hanchar_surf_vert->h-1)) + i;
				src = (U8*)hanfont+(jong* han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf_vert->h;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						*dest=1;
					dest -= engchar_surf_vert->pitch;
				}
			}
		}
	}
	else if(bit_pivot_mode==2)
	{
		//clear
		memset(hanchar_surf_vert->pixels,0,hanchar_surf_vert->pitch*hanchar_surf_vert->h);
		for(i=0;i<hanchar_surf_vert->w;i++)
		{
			U8 *src ;
			if(cho)
			{
				U8 *dest = (U8 *)hanchar_surf_vert->pixels + hanchar_surf_vert->pitch -1 - i;
				src = (U8*)hanfont+(cho * han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf_vert->h;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						*dest=1;
					dest += engchar_surf_vert->pitch;
				}
			}

			if(jung)
			{
				U8 *dest = (U8 *)hanchar_surf_vert->pixels + hanchar_surf_vert->pitch -1 - i;
				src = (U8*)hanfont+(jung* han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf_vert->h;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						*dest=1;
					dest += engchar_surf_vert->pitch;
				}
			}
			if(jong)
			{
				U8 *dest = (U8 *)hanchar_surf_vert->pixels + hanchar_surf_vert->pitch -1 - i;
				src = (U8*)hanfont+(jong* han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf_vert->h;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						*dest=1;
					dest += engchar_surf_vert->pitch;
				}
			}
		}
	}
	else
	{
		//clear
		memset(hanchar_surf->pixels,0,hanchar_surf->pitch*hanchar_surf->h);
		for(i=0;i<hanchar_surf->h;i++)
		{
			U8 *dest = (U8 *)hanchar_surf->pixels + hanchar_surf->pitch*i;
			U8 *src ;
			if(cho)
			{
				src = (U8*)hanfont+(cho * han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf->w;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						dest[k]=1;
				}
			}

			if(jung)
			{
				src = (U8*)hanfont+(jung* han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf->w;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						dest[k]=1;
				}
			}
			if(jong)
			{
				src = (U8*)hanfont+(jong* han_font_w/8*han_font_h)+ (i*(han_font_w/8));
				for(k=0;k<hanchar_surf->w;k++)
				{
					if( src[k/8] & (1<<(7-(k%8))))
						dest[k]=1;
				}
			}
		}
	}

}

static int _draw_char_han(int x, int y, int index)
{
	make_char_han(index);
	if((user_han_font_w != han_font_w ) || (user_han_font_h != han_font_h ) )
	{
		if(bit_pivot_mode==0)
			drawsurfacescalerect(hanchar_surf,x,y,user_han_font_w,user_han_font_h,0,0,hanchar_surf->w,hanchar_surf->h);
		else
			drawsurfacescalerect(hanchar_surf_vert,x,y,user_han_font_w,user_han_font_h,0,0,hanchar_surf_vert->w,hanchar_surf_vert->h);
	}
	else
	{
		if(bit_pivot_mode==0)
			drawsurface(hanchar_surf,x,y);
		else
			drawsurface(hanchar_surf_vert,x,y);

	}
	return user_han_font_w ;
}


#define GET_HAN_INDEX(l, h)    (((h)-0xb0)*(0xff-0xa1)+((l)-0xa1))
int bf_drawstring(int x, int y, char* str)
{
	int i=0;
	U8 low,high;
	int len=0;
	int w;
	while(str[i])
	{
		low = str[i];
		i++;
		if(low<0x80) //eng
		{
			w = _draw_char_eng(x,y,low);
			x += w;
			len+=w;
		}
		else
		{
			high = str[i];
			i++;
			int index = GET_HAN_INDEX(high,low);
			if(index < 2350)//not support hangul
			{
				w = _draw_char_han(x,y,index);
				x += w;
				len+=w;
			}
			else
			{
				w=user_han_font_w;
				x += w;
				len+=w;
			}
		}
	}
	return len;
}

int bf_drawstring_vleft(int x, int y, char* str)
{
	int i=0;
	U8 low,high;
	int len=0;
	int w;
	bit_pivot_mode = 1;
	while(str[i])
	{
		low = str[i];
		i++;
		if(low<0x80) //eng
		{
			w = _draw_char_eng(x,y-user_eng_font_w,low);
			y -= w;
			len+=w;
		}
		else
		{
			high = str[i];
			i++;
			int index = GET_HAN_INDEX(high,low);
			if(index < 2350)//not support hangul
			{
				w = _draw_char_han(x,y-user_han_font_w,index);
				y -= w;
				len+=w;
			}
			else
			{
				w=user_han_font_w;
				y -= w;
				len+=w;
			}
		}
	}
	bit_pivot_mode = 0;
	return len;
}
int bf_drawstring_vright(int x, int y, char* str)
{
	int i=0;
	U8 low,high;
	int len=0;
	int w;
	bit_pivot_mode = 2;
	while(str[i])
	{
		low = str[i];
		i++;
		if(low<0x80) //eng
		{
			w = _draw_char_eng(x,y,low);
			y += w;
			len+=w;
		}
		else
		{
			high = str[i];
			i++;
			int index = GET_HAN_INDEX(high,low);
			if(index < 2350)//not support hangul
			{
				w = _draw_char_han(x,y,index);
				y += w;
				len+=w;
			}
			else
			{
				w=user_han_font_w;
				y += w;
				len+=w;
			}
		}
	}
	bit_pivot_mode = 0;
	return len;
}
int bf_gettextwidth(char* str)
{
	U8 low;
	int x=0;
	int i=0;
	while(str[i])
	{
		low = str[i];
		i++;
		if(low<0x80) //eng
		{
			x += user_eng_font_w;
		}
		else
		{
			i++;
			x += user_han_font_w;
		}
	}
	return x;
}
SURFACE* bf_makesurface(char* str)
{
	if(engchar_surf==0)
		return 0 ;
	if(hanchar_surf==0)
		return 0 ;
	int i=0;
	int x=0;
	U8 low,high;
	int w = bf_gettextwidth(str);
	sARGB argb;
	argb.a=255;
	argb.r=0;
	argb.g=0;
	argb.b=0;
	SURFACE* surf = createsurface(w,eng_font_h>han_font_h?eng_font_h:han_font_h,8);

	if(surf==0)
	{
		debugerror(SDKERROR_NOMEM,0);
		return 0;
	}
	memset(surf->pixels,0,surf->pitch*surf->h);
	surf->pal->colorkey=0;
	surf->pal->colors[1]=engchar_surf->pal->colors[1];
	while(str[i])
	{
		low = str[i];
		i++;
		if(low<0x80) //eng
		{
			make_char_eng(low);
			U8* dest;
			U8* src;
			int y;
			dest = ((U8*)surf->pixels) + x;
			src = (U8*)engchar_surf->pixels;
			for(y=0;y<eng_font_h;y++)
			{
				memcpy(dest,src,eng_font_w);
				dest += surf->pitch;
				src += engchar_surf->pitch;
			}
			x += eng_font_w;
		}
		else
		{
			high = str[i];
			i++;
			int index = GET_HAN_INDEX(high,low);
			if(index < 2350)//not support hangul
			{
				make_char_han(index);
				U8* dest;
				U8* src;
				int y;
				dest = ((U8*)surf->pixels) + x;
				src = (U8*)hanchar_surf->pixels;
				for(y=0;y<han_font_h;y++)
				{
					memcpy(dest,src,han_font_w);
					src += hanchar_surf->pitch;
					dest += surf->pitch;
				}
			}
			x += han_font_w;
		}
	}
	return surf;
}

bool  bf_init()
{
	sARGB argb;
	argb.a=255;
	argb.r=0;
	argb.g=0;
	argb.b=0;
	engchar_surf=createsurface(eng_font_w,eng_font_h,8);
	if(engchar_surf==0)
	{
		debugerror(SDKERROR_NOMEM,0);
		return false;
	}
	engchar_surf_vert=createsurface(eng_font_h,eng_font_w,8);
	if(engchar_surf_vert==0)
	{
		debugerror(SDKERROR_NOMEM,0);
		return false;
	}
	engchar_surf->pal->colorkey=0;
	engchar_surf->pal->colors[1]=argb;
	engchar_surf_vert->pal->colorkey=0;
	engchar_surf_vert->pal->colors[1]=argb;

	hanchar_surf=createsurface(han_font_w,han_font_h,8);
	if(hanchar_surf==0)
	{
		debugerror(SDKERROR_NOMEM,0);
		return false;
	}
	hanchar_surf_vert=createsurface(han_font_h,han_font_w,8);
	if(hanchar_surf_vert==0)
	{
		debugerror(SDKERROR_NOMEM,0);
		return false;
	}

	hanchar_surf->pal->colorkey=0;
	hanchar_surf->pal->colors[1]=argb;
	hanchar_surf_vert->pal->colorkey=0;
	hanchar_surf_vert->pal->colors[1]=argb;
	return true;
}
