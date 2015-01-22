/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.
*****************************************************************************/
#include "adstar.h"

extern int g_clip_x;
extern int g_clip_y;
extern int g_clip_endx;
extern int g_clip_endy;

static BOOL ScaleLine(int dx,int dy,int dw,SURFACE* srcf,int sx,int sy, int sw)
{
	int NumPixels = dw;
	int IntPart = sw / dw;
	int FractPart = sw % dw;
	int E = 0;
	U8 a,r,g,b;
	SURFACE* targetsurface = get_draw_target();
	if(targetsurface->bpp==16)
	{
		U16* dest = (U16*)(((U32)targetsurface->pixels)+(targetsurface->pitch*dy)+(dx*2));
		if(srcf->bpp==16)
		{
			U16* src = (U16*)(((U32)srcf->pixels)+(srcf->pitch*sy)+(sx*srcf->bpp/8));
			while (NumPixels-- > 0) {
				*dest=*src;
				dest++;
				src += IntPart;
				E += FractPart;
				if (E >= dw) {
					E -= dw;
					src++;
				} /* if */
			} /* while */
			return TRUE;
		}
		else if(srcf->bpp==32)
		{
			U32* src = (U32*)(((U32)srcf->pixels)+(srcf->pitch*sy)+(sx*srcf->bpp/8));
			if(srcf->pixtype == PIX_FMT_ARGB)
			{
				while (NumPixels-- > 0) {
					a = (*src) >> 24;
					if(a)
					{
						b = (*src) >> 0;
						g = (*src) >> 8;
						r = (*src) >> 16;
						if(a!=255)
						{
							a=255-a;
							U8 bg_r,bg_g,bg_b;
							U16 bg_rgb = *dest;

							bg_r = (bg_rgb&0xf100)>>8;
							bg_g = (bg_rgb&0x07e0)>>3;
							bg_b = (bg_rgb&0x01f)<<3;
							r += (((bg_r- (int)r)*a)>>8);
							g += (((bg_g- (int)g)*a)>>8);
							b += (((bg_b- (int)b)*a)>>8);
						}
						*dest=MAKE_RGB565(r,g,b);
					}
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
			else if(srcf->pixtype==PIX_FMT_RGB888)
			{
				while (NumPixels-- > 0) {
					b = (*src) >> 0;
					g = (*src) >> 8;
					r = (*src) >> 16;
					*dest=MAKE_RGB565(r,g,b);
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
		}
		else if(srcf->bpp==8)
		{
			U8* src = (U8*)(((U32)srcf->pixels)+(srcf->pitch*sy)+(sx*srcf->bpp/8));
			sARGB* pal = (sARGB*)srcf->pal->colors;
			if(srcf->pixtype == PIX_FMT_A000)//first palette is used for color
			{
				U16 rgb565;
				r = pal->r;
				g = pal->g;
				b = pal->b;
				rgb565 = MAKE_RGB565(r,g,b );
				while (NumPixels-- > 0) {
					a = *src;
					if(a)
					{
						if(a!=255)
						{
							U8 bg_r,bg_g,bg_b;
							U16 bg_rgb = *dest;

							bg_r = (bg_rgb&0xf100)>>8;
							bg_r |= 0x7;
							bg_g = (bg_rgb&0x07e0)>>3;
							bg_g |=0x3;
							bg_b = (bg_rgb&0x01f)<<3;
							bg_b |=0x3;
							r += (((bg_r- (int)r)*a)>>8);
							g += (((bg_g- (int)g)*a)>>8);
							b += (((bg_b- (int)b)*a)>>8);
						}
						*dest = rgb565;
					}
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
			else
			{
				while (NumPixels-- > 0) {
					if(*src != srcf->pal->colorkey)
					{
						*dest = MAKE_RGB565(pal[*src].r,pal[*src].g,pal[*src].b );
					}
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
		}
	}
	else if(targetsurface->bpp==32) //dest PIX_FMT_RGB888
	{
		U32* dest = (U32*)(((U32)targetsurface->pixels)+(targetsurface->pitch*dy)+(dx*4));
		if(srcf->bpp==16)
		{
			U16* src = (U16*)(((U32)srcf->pixels)+(srcf->pitch*sy)+(sx*srcf->bpp/8));
			while (NumPixels-- > 0) {
				r = ((*src)>>8) & 0xf8;
				g = ((*src)>>3) & 0xfc;
				b = ((*src)<<3);
				*dest=MAKE_RGB888(r,g,b);

				dest++;
				src += IntPart;
				E += FractPart;
				if (E >= dw) {
					E -= dw;
					src++;
				} /* if */
			} /* while */
			return TRUE;
		}
		else if(srcf->bpp==32)
		{
			U32* src = (U32*)(((U32)srcf->pixels)+(srcf->pitch*sy)+(sx*srcf->bpp/8));
			U8 a,r,g,b;
			if(srcf->pixtype == PIX_FMT_ARGB)
			{
				while (NumPixels-- > 0) {
					a = (*src) >> 24;
					if(a)
					{
						b = (*src) >> 0;
						g = (*src) >> 8;
						r = (*src) >> 16;
						if(a!=255)
						{
							a=255-a;
							U8 bg_r,bg_g,bg_b;
							bg_r = (*dest)>>16;
							bg_g = (*dest)>>8;
							bg_b = (U8)(*dest);
							r += (((bg_r- (int)r)*a)>>8);
							g += (((bg_g- (int)g)*a)>>8);
							b += (((bg_b- (int)b)*a)>>8);
						}
						*dest=MAKE_RGB888(r,g,b);
					}
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
			else if(srcf->pixtype==PIX_FMT_RGB888)
			{
				while (NumPixels-- > 0) {
					*dest=*src;
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
		}
		else if(srcf->bpp==8)
		{
			U8* src = (U8*)(((U32)srcf->pixels)+(srcf->pitch*sy)+(sx*srcf->bpp/8));
			sARGB* pal = (sARGB*)srcf->pal->colors;
			if(srcf->pixtype == PIX_FMT_A000)//first palette is used for color
			{
				r = pal->r;
				g = pal->g;
				b = pal->b;
				while (NumPixels-- > 0) {
					a = *src;
					if(a)
					{
						if(a!=255)
						{
							U8 bg_r,bg_g,bg_b;
							U32 bg_rgb = *dest;

							bg_r = bg_rgb>>16;
							bg_g = bg_rgb>>8;
							bg_b = bg_rgb&0xff;
							r += (((bg_r- (int)r)*a)>>8);
							g += (((bg_g- (int)g)*a)>>8);
							b += (((bg_b- (int)b)*a)>>8);
						}
						*dest = MAKE_RGB888(r,g,b);
					}
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
			else
			{
				while (NumPixels-- > 0) {
					if(*src != srcf->pal->colorkey)
					{
						*dest = MAKE_RGB888(pal[*src].r,pal[*src].g,pal[*src].b );
					}
					dest++;
					src += IntPart;
					E += FractPart;
					if (E >= dw) {
						E -= dw;
						src++;
					} /* if */
				} /* while */
				return TRUE;
			}
		}
	}
	debugstring("Not Supports format ("__FILE__")\r\n");
	return FALSE;
	
}
BOOL draw_surface_scalerect(SURFACE* src_surf,int x, int y,int w,int h,int sx, int sy, int sw, int sh)
{
	//calculate clip and surface 
	int percent;
	int diff;
	SURFACE* targetsurface = get_draw_target();
	if(x<0)
		return FALSE;
	if(y<0)
		return FALSE;
	if(sx<0)
		return FALSE;
	if(sy<0)
		return FALSE;
	if(x+w>targetsurface->w)
		return FALSE;
	if(y+h>targetsurface->h)
		return FALSE;
	if(sx+sw>src_surf->w)
		return FALSE;
	if(sy+sh>src_surf->h)
		return FALSE;

	if(x<targetsurface->clip.x)
	{
		diff  = targetsurface->clip.x-x;
		percent = (diff*0x10000)/w;
		x = targetsurface->clip.x;
		w -= diff;
		diff = (percent*sw)/0x10000;
		sx += diff;
		sw -=  diff;
	}

	if(y<targetsurface->clip.y)
	{
		diff  = targetsurface->clip.y-y;
		percent = (diff*0x10000)/h;
		y = targetsurface->clip.y;
		h -= diff;
		diff = (percent*sh)/0x10000;
		sy += diff;
		sh -=  diff;
	}

	if((x+w)>targetsurface->clip.endx)
	{
		diff  = (x+w) - targetsurface->clip.endx;
		percent = (diff*0x10000)/w;
		w = targetsurface->clip.endx - x;
		diff = (percent*sw)/0x10000;
		sw -=  diff;
	}
	if((y+h)>targetsurface->clip.endy)
	{
		diff  = (y+h) - targetsurface->clip.endy;
		percent = (diff*0x10000)/h;
		h = targetsurface->clip.endy - y;
		diff = (percent*sh)/0x10000;
		sh -=  diff;
	}

	int NumPixels = h;
	int IntPart = (sh/ h);
	int FractPart = sh% h;
	int E = 0;
	while(NumPixels > 0 )
	{
		if(ScaleLine(x,y,w,src_surf,sx,sy,sw)==FALSE)
		{
			dcache_invalidate_way();
			return FALSE;
		}
		y++;
		sy+=IntPart;
		E += FractPart;
		if(E>=h)
		{
			E-=h;
			sy++;
		}
		NumPixels--;
	}
	dcache_invalidate_way();
	return TRUE;
}

BOOL draw_surface_scale(SURFACE* src_surf,int dx, int dy,int dw,int dh)
{
	return draw_surface_scalerect(src_surf,dx, dy,dw,dh,0, 0, src_surf->w,src_surf->h);
}

