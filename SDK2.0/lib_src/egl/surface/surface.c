/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc.
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#include "adstar.h"
#include "egl_typedef.h"
static SURFACE* g_targetsurface = 0;

CLIP_RECT draw_set_clip_window(SURFACE* dst, CLIP_RECT* pRect)
{
	CLIP_RECT old = dst->clip;
	dst->clip = *pRect;
	return old;
}
void draw_get_clip_window(SURFACE* dst, CLIP_RECT* pRect)
{
	*pRect = dst->clip;
}
/*
return true, if coordinate is in clip-windows, otherwise return false
*/
static bool check_clipwindow(SURFACE* dsrf,int* dx, int* dy,int* sx,int* sy,int* w,int* h)
{
	int diff;
	if(*dx < dsrf->clip.x)
	{
		diff = dsrf->clip.x - *dx;
		*dx = dsrf->clip.x;
		*sx += diff;
		*w -= diff;
		if(*w<=0)
			return false;
	}
	if(*dy < dsrf->clip.y)
	{
		diff = dsrf->clip.y - *dy;
		*dy=dsrf->clip.y;
		*sy+=diff;
		*h -= diff;
		if(*h<=0)
			return false;
	}
	if((*dx + *w)>dsrf->clip.endx)
	{
		diff = (*dx + *w) - dsrf->clip.endx;
		*w -=diff;
		if(*w<=0)
			return false;
	}
	if((*dy + *h)>dsrf->clip.endy)
	{
		diff = (*dy + *h) - dsrf->clip.endy ;
		*h -=diff;
		if(*h<=0)
			return false;
	}
	return true;
}

SURFACE* set_draw_target( SURFACE* surf )
{
    SURFACE* old = g_targetsurface;
    g_targetsurface = surf;
    return old;
}
SURFACE* get_draw_target()
{
    return g_targetsurface;
}
//for SDK1.x
SURFACE* setdrawtarget( SURFACE* surf )
{
    SURFACE* old = g_targetsurface;
    g_targetsurface = surf;
    return old;
}
//for SDK1.x
SURFACE* getdrawtarget()
{
    return g_targetsurface;
}

PALETTE* createpalette( int bpp, int nColors )
{
    PALETTE* pal = ( PALETTE* )malloc( sizeof( PALETTE ) );
    if( pal == 0 )
        return 0;
    pal->colors = ( EGL_COLOR* )malloc( ( bpp / 8 ) * nColors );
    if( pal->colors == 0 )
    {
        free( pal );
        return 0;
    }
    pal->bpp = bpp;
    pal->nColors = nColors;
    pal->colorkey = -1;
    return pal;
}

SURFACE* create_surface( U32 w, U32 h, U32 bpp )
{
    if( !( ( bpp == 32 ) || ( bpp == 16 ) || ( bpp == 4 ) || ( bpp == 8 ) ) )
        return 0;
    SURFACE* surface = NULL;
    if( ( w == 0 ) || ( h == 0 ) )
        return 0;
	surface = ( SURFACE* )malloc( sizeof( SURFACE ) );
	if( surface == 0 )
    {
        debugstring( "Not enough memory\r\n" );
        return 0;
    }
    memset( surface, 0, sizeof( SURFACE ) );
    surface->bpp = bpp;
    surface->w = w;
    surface->h = h;
    surface->clip.endx = w;
    surface->clip.endy = h;
    if( bpp == 32 )
    {
        surface->pitch = w * 4;
        surface->pixtype = PIX_FMT_RGB888;
    }
    else
    {
        surface->pitch = ( ( w * bpp / 8 ) + 0x3 ) & 0xfffffffc; //align 4byte
        if( bpp == 16 )
            surface->pixtype = PIX_FMT_RGB565;
        else
        {
            surface->pixtype = PIX_FMT_PALETTE;
            if( bpp == 4 )
                surface->pal = createpalette( 32, 16 );
            else
                surface->pal = createpalette( 32, 256 );
        }
    }
	surface->pixels = ( U32* )memalign( 16, surface->pitch * h );
    return surface;
}
void release_palette( PALETTE* pal )
{
    if( pal == 0 )
        return ;
    free( pal->colors );
    free( pal );
}
void release_surface( SURFACE* surf )
{
    ASSERT( surf != 0 );
    if( surf == 0 )
        return;
    if( surf->pixels )
    {
        free( surf->pixels );
    }
    if( surf->pal )
    {
        release_palette( surf->pal );
    }
    free( surf );
}

/*
RGB888 ==> RGB888 or ARGB ==> RGB888
*/
static BOOL blit32to32( U8* dest, SURFACE* dsrf, U8* src, SURFACE* ssrf, int w, int h )
{
	register int x, y;
	register U8 r, g, b, a;
	register U32 bg_r, bg_g, bg_b;
    int spitch, dpitch;
    int ashiftbit;
    int rshiftbit;
    int gshiftbit;
    int bshiftbit;
	U32* argbbuf;

    ashiftbit = 24;
    rshiftbit = 16;
    gshiftbit = 8;
    bshiftbit = 0;

    spitch = ssrf->pitch;
    dpitch = dsrf->pitch;
	if( ssrf->bpp == 32 )
	{
		if( ssrf->pixtype == PIX_FMT_ARGB )
		{
			for( y = 0; y < h; y++ )
			{
				argbbuf = ( U32* )( ( U32 )src + ( spitch * y ) );
				rRGB* rrgbbuf = ( rRGB* )( ( U32 )dest + ( dpitch * y ) );
				for( x = 0; x < w; x++ )
				{
					a = argbbuf[x] >> ashiftbit;
					if( a )
					{
						b = argbbuf[x] >> bshiftbit;
						g = argbbuf[x] >> gshiftbit;
						r = argbbuf[x] >> rshiftbit;
						if( a != 255 )
						{
							a = 255 - a;
							bg_r = rrgbbuf[x].rrgb.r;
							bg_g = rrgbbuf[x].rrgb.g;
							bg_b = rrgbbuf[x].rrgb.b;
							r += ( ( ( bg_r - ( int )r ) * a ) >> 8 );
							g += ( ( ( bg_g - ( int )g ) * a ) >> 8 );
							b += ( ( ( bg_b - ( int )b ) * a ) >> 8 );
						}
						rrgbbuf[x] = ( rRGB )MAKE_RGB888( r, g, b );
					}
				}
			}
			dcache_invalidate_way();
			return TRUE;
		}
		else
		{
			DEBUGPRINTF( "Not Supported format yet(Source %dbpp, Destination %d)\n", ssrf->bpp, dsrf->bpp );
			return FALSE;
		}
	}
	else if( ssrf->bpp == 16 )
	{
		for( y = 0; y < h; y++ )
		{
			RGB565* buf565 = ( RGB565* )( ( U32 )src + ( ssrf->pitch * y ) );
			rRGB* rrgbbuf = ( rRGB* )( ( U32 )dest + ( dsrf->pitch * y ) );
			for( x = 0; x < w; x++ )
			{
				r = buf565[x].r;
				g = buf565[x].g;
				b = buf565[x].b;
				rrgbbuf[x] = ( rRGB )MAKE_RGB888( r, g, b );
			}
		}
		dcache_invalidate_way();
		return TRUE;
	}
	else if( ssrf->bpp == 8 )
	{
		if( ssrf->pal == 0 )
			return FALSE;
		sARGB* pal = ( sARGB* )ssrf->pal->colors;
		for( y = 0; y < h; y++ )
		{
			U8* indexbuf = ( U8* )( ( U32 )src + ( ssrf->pitch * y ) );
			rRGB* rrgbbuf = ( rRGB* )( ( U32 )dest + ( dsrf->pitch * y ) );
			if( ssrf->pixtype == PIX_FMT_A000 )
			{
				for( x = 0; x < w; x++ )
				{
					a = indexbuf[x];
					if( a )
					{
						r = pal->r;
						g = pal->g;
						b = pal->b;
						if( a != 255 ) //
						{
							//a = 255-a;
							bg_r = rrgbbuf[x].rrgb.r;
							bg_g = rrgbbuf[x].rrgb.g;
							bg_b = rrgbbuf[x].rrgb.b;
							r += ( ( ( bg_r - ( int )r ) * a ) >> 8 );
							g += ( ( ( bg_g - ( int )g ) * a ) >> 8 );
							b += ( ( ( bg_b - ( int )b ) * a ) >> 8 );
						}
						rrgbbuf[x].data32 = MAKE_RGB888( r, g, b );
					}
				}
			}
			else
			{
				for( x = 0; x < w; x++ )
				{
					int index = indexbuf[x];
					if( index != ssrf->pal->colorkey )
						rrgbbuf[x].data32 = MAKE_RGB888( pal[index].r, pal[index].g, pal[index].b );
				}
			}
		}
		dcache_invalidate_way();
		return TRUE;
	} 
	return FALSE;
}

/*
	ARGB ==> RGB565
*/
static BOOL blit32to16( U8* dest, SURFACE* dsrf, U8* src, SURFACE* ssrf, int w, int h )
{
    register int x, y;
	register U8 r, g, b, a;
	register U32 bg_r, bg_g, bg_b;
    int spitch, dpitch;
    int ashiftbit;
    int rshiftbit;
    int gshiftbit;
    int bshiftbit;
	U32* argbbuf;
	U16* bg16buf;
	
    ashiftbit = 24;
	if(ssrf->pixtype ==  PIX_FMT_ABGR)
	{
		bshiftbit = 16;
		gshiftbit = 8;
		rshiftbit = 0;
	}
	else
	{
		rshiftbit = 16;
		gshiftbit = 8;
		bshiftbit = 0;
	}

    spitch = ssrf->pitch;
    dpitch = dsrf->pitch;
    if( ssrf->pixtype == PIX_FMT_ARGB )
    {
        for( y = 0; y < h; y++ )
        {
            argbbuf = ( U32* )( ( U32 )src + ( spitch * y ) );
            bg16buf = ( U16* )( ( U32 )dest + ( dpitch * y ) );
            for( x = 0; x < w; x++ )
            {
                a = argbbuf[x] >> ashiftbit;
                if( a )
                {
                    b = argbbuf[x] >> bshiftbit;
                    g = argbbuf[x] >> gshiftbit;
                    r = argbbuf[x] >> rshiftbit;

                    if( a != 255 ) //
                    {
                        a = 255 - a;
                        U16 bg_rgb = bg16buf[x];
                        bg_r = ( bg_rgb & 0xf100 ) >> 8;
                        bg_g = ( bg_rgb & 0x07e0 ) >> 3;
                        bg_b = ( bg_rgb & 0x01f ) << 3;
                        r += ( ( ( bg_r - ( int )r ) * a ) >> 8 );
                        g += ( ( ( bg_g - ( int )g ) * a ) >> 8 );
                        b += ( ( ( bg_b - ( int )b ) * a ) >> 8 );
                    }
                    bg16buf[x] = MAKE_RGB565( r, g, b );
                }
            }
        }
        dcache_invalidate_way();
        return TRUE;
    }//ssrf->pixtype == PIX_FMT_ARGB
    else if( ssrf->pixtype == PIX_FMT_RGB888 )
    {
        for( y = 0; y < h; y++ )
        {
            rRGB* rgbbuf = ( rRGB* )( ( U32 )src + ( spitch * y ) );
            U16* bg16buf = ( U16* )( ( U32 )dest + ( dpitch * y ) );
            for( x = 0; x < w; x++ )
            {
                r = rgbbuf[x].rrgb.r;
                g = rgbbuf[x].rrgb.g;
                b = rgbbuf[x].rrgb.b;
                bg16buf[x] = MAKE_RGB565( r, g, b );
            }
        }
        dcache_invalidate_way();
        return TRUE;
    }//ssrf->pixtype == PIX_FMT_RGB888
    else
    {
        DEBUGPRINTF( "Not Supported format (Source %dbpp, Destination %d)\n", ssrf->bpp, dsrf->bpp );
        return FALSE;
    }
}
static BOOL blit8to16(U8* dest, SURFACE* dsrf, U8* src, SURFACE* ssrf, int w, int h )
{
	register int x;
	register int y;
	register sARGB* pal ;
	register U8 r, g, b,a;
	register U32 bg_r, bg_g, bg_b;
	register int spitch, dpitch;

	if( ssrf->pal == 0 )
		return FALSE;
	spitch = ssrf->pitch;
	dpitch = dsrf->pitch;

	pal = ( sARGB* )ssrf->pal->colors;
	for( y = 0; y < h; y++ )
	{
		U8* indexbuf = ( U8* )( ( U32 )src + ( spitch * y ) );
		U16* bg16buf = ( U16* )( ( U32 )dest + ( dpitch * y ) );
		if( ssrf->pixtype == PIX_FMT_A000 ) //first palette is used for color
		{
			r = pal->r;
			g = pal->g;
			b = pal->b;
			U16 rgb565 = MAKE_RGB565( r, g, b );
			for( x = 0; x < w; x++ )
			{
				a = indexbuf[x];
				if( a )
				{
					if( a != 255 ) //
					{
						//a = 255-a;
						U16 bg_rgb = bg16buf[x];
						bg_r = ( bg_rgb & 0xf100 ) >> 8;
						bg_r |= 0x7;
						bg_g = ( bg_rgb & 0x07e0 ) >> 3;
						bg_g |= 0x3;
						bg_b = ( bg_rgb & 0x01f ) << 3;
						bg_b |= 0x3;
						r += ( ( ( bg_r - ( int )r ) * a ) >> 8 );
						g += ( ( ( bg_g - ( int )g ) * a ) >> 8 );
						b += ( ( ( bg_b - ( int )b ) * a ) >> 8 );
						bg16buf[x] = MAKE_RGB565(r,g,b);
					}
					else
						bg16buf[x] = rgb565;
				}
			}
		}
		else
		{
			for( x = 0; x < w; x++ )
			{
				int index = indexbuf[x];
				if( index != ssrf->pal->colorkey )
					bg16buf[x] = MAKE_RGB565( pal[index].r, pal[index].g, pal[index].b );
			}
		}
	}
	dcache_invalidate_way();
	return TRUE;
}
static BOOL blit8to32(U8* dest, SURFACE* dsrf, U8* src, SURFACE* ssrf, int w, int h )
{
	register int x;
	register int y;
	register sARGB* pal ;
	register U8 r, g, b,a;
	register U32 bg_r, bg_g, bg_b;
	register int spitch, dpitch;

	if( ssrf->pal == 0 )
		return FALSE;
	spitch = ssrf->pitch;
	dpitch = dsrf->pitch;

	pal = ( sARGB* )ssrf->pal->colors;
	for( y = 0; y < h; y++ )
	{
		U8* indexbuf = ( U8* )( ( U32 )src + ( spitch * y ) );
		U32* bg32buf = ( U32* )( ( U32 )dest + ( dpitch * y ) );
		if( ssrf->pixtype == PIX_FMT_A000 ) //first palette is used for color
		{
			r = pal->r;
			g = pal->g;
			b = pal->b;
			U16 rgb = MAKE_RGB888( r, g, b );
			for( x = 0; x < w; x++ )
			{
				a = indexbuf[x];
				if( a )
				{
					if( a != 255 ) //
					{
						//a = 255-a;
						U32 bg_rgb = bg32buf[x];
						EXTRACT_RGB(bg_rgb,bg_r,bg_g,bg_b);
						r += ( ( ( bg_r - ( int )r ) * a ) >> 8 );
						g += ( ( ( bg_g - ( int )g ) * a ) >> 8 );
						b += ( ( ( bg_b - ( int )b ) * a ) >> 8 );
						bg32buf[x] = MAKE_RGB888(r,g,b);
					}
					else
						bg32buf[x] = rgb;
				}
			}
		}
		else
		{
			for( x = 0; x < w; x++ )
			{
				int index = indexbuf[x];
				if( index != ssrf->pal->colorkey )
					bg32buf[x] = MAKE_RGB888( pal[index].r, pal[index].g, pal[index].b );
			}
		}
	}
	dcache_invalidate_way();
	return TRUE;
}
/*
ignore destination alpha value
*/
BOOL draw_surface_rect( SURFACE* ssrf, int dx, int dy, int sx, int sy, int w, int h )
{
    U8* src;
    U8* dest;
    SURFACE* dsrf = g_targetsurface;
    if( dy >= dsrf->h )
        return FALSE;
    if( dx >= dsrf->w )
        return FALSE;
    if( ( dsrf->w - dx ) < w )
        w = dsrf->w - dx;
    if( ( dsrf->h - dy ) < h )
        h = dsrf->h - dy;
    if( !( ( dsrf->bpp == 16 ) || ( dsrf->bpp == 32 ) ) )
        return FALSE;
    if( check_clipwindow( dsrf, &dx, &dy, &sx, &sy, &w, &h ) == false )
        return FALSE;
    src = ( U8* )( ( ( U32 )ssrf->pixels ) + ( ssrf->pitch * sy ) + ( sx * ( ssrf->bpp / 8 ) ) );
    dest = ( U8* )( ( ( U32 )dsrf->pixels ) + ( dsrf->pitch * dy ) + ( dx * ( dsrf->bpp / 8 ) ) );
    if( ( dsrf->bpp == ssrf->bpp ) && ( dsrf->pixtype == ssrf->pixtype ) )
    {
        dcache_invalidate_way();
        dma_blockcpy( dest, src, dsrf->pitch, ssrf->pitch, w * ( dsrf->bpp / 8 ), h );
        return TRUE;
    }
    else
    {
        if( dsrf->bpp == 16 )
        {
            if( ssrf->bpp == 32 )
            {
                return blit32to16( dest, dsrf, src, ssrf, w, h );
            }//ssrf->bpp==32
            else if( ssrf->bpp == 8 )
            {
                return blit8to16(dest, dsrf, src, ssrf, w, h );
            } // end ssrf->bpp==8
        }
        else if( dsrf->bpp == 32 )
        {
			if(dsrf->pixtype == PIX_FMT_RGB888 )
				return blit32to32(dest, dsrf, src, ssrf, w, h );
			else if(ssrf->bpp==8)
				return blit8to32(dest, dsrf, src, ssrf, w, h );
        }
    }
    DEBUGPRINTF( "Not Supported format yet(Source %dbpp, Destination %d)\n", ssrf->bpp, dsrf->bpp );
    return FALSE;
}

BOOL draw_surface( SURFACE* src_surf, int dx, int dy )
{
    return draw_surface_rect( src_surf, dx, dy, 0, 0, src_surf->w, src_surf->h );
}


