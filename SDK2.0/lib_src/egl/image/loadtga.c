/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#include "adstar.h"
#include "surface.h"


struct TGAheader {
	U8 infolen;		/* length of info field */
	U8 has_cmap;		/* 1 if image has colormap, 0 otherwise */
	U8 type;

	U8 cmap_start[2];	/* index of first colormap entry */
	U8 cmap_len[2];		/* number of entries in colormap */
	U8 cmap_bits;		/* bits per colormap entry */

	U16 yorigin;		/* image origin (ignored here) */
	U16 xorigin;
	U16 width;		/* image size */
	U16 height;
	U8 pixel_bits;		/* bits/pixel */
	U8 flags;
};

enum tga_type {
	TGA_TYPE_INDEXED = 1,
	TGA_TYPE_RGB = 2,
	TGA_TYPE_BW = 3,
	TGA_TYPE_RLE_INDEXED = 9,
	TGA_TYPE_RLE_RGB = 10,
	TGA_TYPE_RLE_BW = 11
};

#define TGA_INTERLEAVE_MASK	0xc0
#define TGA_INTERLEAVE_NONE	0x00
#define TGA_INTERLEAVE_2WAY	0x40
#define TGA_INTERLEAVE_4WAY	0x80

#define TGA_ORIGIN_MASK		0x30
#define TGA_ORIGIN_LEFT		0x00
#define TGA_ORIGIN_RIGHT	0x10
#define TGA_ORIGIN_LOWER	0x00
#define TGA_ORIGIN_UPPER	0x20


/* read/write unaligned little-endian 16-bit ints */
#define LE16(p) ((p)[0] + ((p)[1] << 8))
#define SETLE16(p, v) ((p)[0] = (v), (p)[1] = (v) >> 8)

SURFACE* loadtgap(U8* databuf)
{
	struct TGAheader* phdr;
	int rle = 0;
	int alpha = 0;
	int indexed = 0;
	int grey = 0;
	int ckey = -1;
	int ncols, w, h;
	U32 rmask, gmask, bmask, amask;
	U8 *dst;
	int i;
	int bpp;
	int lstep;
	U32 pixel;
	int count, rep;
	SURFACE* surf;

	U8* src ;
	if ( databuf==0)
		return 0;
	
	phdr = (struct TGAheader*)databuf;
	src = databuf + sizeof(struct TGAheader);
	ncols = LE16(phdr->cmap_len);
	switch(phdr->type) {
	case TGA_TYPE_RLE_INDEXED:
		rle = 1;
		/* fallthrough */
	case TGA_TYPE_INDEXED:
		if(!phdr->has_cmap || phdr->pixel_bits != 8 || ncols > 256)
			return 0;
		indexed = 1;
		break;

	case TGA_TYPE_RLE_RGB:
		rle = 1;
		/* fallthrough */
	case TGA_TYPE_RGB:
		indexed = 0;
		break;

	case TGA_TYPE_RLE_BW:
		rle = 1;
		/* fallthrough */
	case TGA_TYPE_BW:
		if(phdr->pixel_bits != 8)
			return 0;
		/* Treat greyscale as 8bpp indexed images */
		indexed = grey = 1;
		break;

	default:
		return 0;
	}

	bpp = (phdr->pixel_bits + 7) >> 3;
	rmask = gmask = bmask = amask = 0;
	switch(phdr->pixel_bits) {
	case 8:
		if(!indexed) {
			return 0;
		}
		break;

	case 15:
	case 16:
		/* 15 and 16bpp both seem to use 5 bits/plane. The extra alpha bit
		is ignored for now. */
		rmask = 0x7c00;
		gmask = 0x03e0;
		bmask = 0x001f;
		break;

	case 32:
		alpha = 1;
		/* fallthrough */
	case 24:
		amask = alpha ? 0xff000000 : 0;
		rmask = 0x00ff0000;
		gmask = 0x0000ff00;
		bmask = 0x000000ff;
		break;

	default:
		return 0;
	}

	if((phdr->flags & TGA_INTERLEAVE_MASK) != TGA_INTERLEAVE_NONE
		|| phdr->flags & TGA_ORIGIN_RIGHT) {
			return 0;
	}

	//SDL_RWseek(src, phdr->infolen, RW_SEEK_CUR); /* skip info field */

	w = phdr->width;
	h = phdr->height;
	surf = create_surface(w,h,bpp*8);
	if(surf== NULL) {
		return 0;
	}
	src += phdr->infolen;

	if(phdr->has_cmap) {
		int palsiz = ncols * ((phdr->cmap_bits + 7) >> 3);
		if(indexed && !grey) {
			//surf->pal = createpalette(((phdr->cmap_bits + 7) >> 3)*8,ncols);
			U8 *p ;
			sARGB* colors = (sARGB* )surf->pal->colors;
			p =src;
			for(i = 0; i < ncols; i++) {
				switch(phdr->cmap_bits) {
				case 15:
				case 16:
					{
						U16 c = p[0] + (p[1] << 8);
						p += 2;
						colors[i].r = (c >> 7) & 0xf8;
						colors[i].g = (c >> 2) & 0xf8;
						colors[i].b = c << 3;
					}
					break;
				case 24:
				case 32:
					colors[i].b = *p++;
					colors[i].g = *p++;
					colors[i].r = *p++;
					if(phdr->cmap_bits == 32 && *p++ < 128)
						ckey = i;
					break;
				}
			}
			if(ckey >= 0)
			{
				surf->pal->colorkey=ckey;
			}
		}
		src+= palsiz;
	}

	if(grey) {
		//surf->pal = createpalette(32,256);
		sARGB *colors = (sARGB* )surf->pal->colors;
		for(i = 0; i < 256; i++)
			colors[i].r = colors[i].g = colors[i].b = i;
		//if grey, set colorkey
		surf->pal->colorkey=0;
	}

	if(phdr->flags & TGA_ORIGIN_UPPER) {
		lstep = surf->pitch;
		dst = surf->pixels;
	} else {
		lstep = -(surf->pitch);
		dst = (U8 *)surf->pixels + (h - 1) * surf->pitch;
	}
	
	/* The RLE decoding code is slightly convoluted since we can't rely on
	spans not to wrap across scan lines */
	count = rep = 0;
	for(i = 0; i < h; i++) {
		if(rle) {
			int x = 0;
			for(;;) {
				U8 c;

				if(count) {
					int n = count;
					if(n > w - x)
						n = w - x;
					memcpy(dst + x * bpp,src,n * bpp);
					src += n * bpp;
					count -= n;
					x += n;
					if(x == w)
						break;
				} else if(rep) {
					int n = rep;
					if(n > w - x)
						n = w - x;
					rep -= n;
					while(n--) {
						memcpy(dst + x * bpp, &pixel, bpp);
						x++;
					}
					if(x == w)
						break;
				}
				c = *(src);
				src++;
				if(c & 0x80) {
					memcpy(&pixel,src,bpp);
					src+=bpp;
					rep = (c & 0x7f) + 1;
				} else {
					count = c + 1;
				}
			}

		} else {
			memcpy(dst,src, w * bpp);
			src +=  w * bpp;
		}
		dst += lstep;
	}
	if(!indexed)
	{
		if(alpha)
		{
			surf->pixtype = PIX_FMT_ARGB;
		}
		else
		{
			surf->pixtype = PIX_FMT_RGB888;
		}
	}
	else
		surf->pixtype = PIX_FMT_PALETTE;

	return surf;
}


SURFACE* loadtga(char* fname)
{
	U32 nRead;
	FIL fp;
	U32 datasize;
	U8* buf;
	FRESULT res = f_open(&fp,fname,FA_READ|FA_OPEN_EXISTING);
	if( res != FR_OK )
	{
		DEBUGPRINTF("Cannot open : %s\r\n", fname);
		return 0;
	}
	datasize = f_size(&fp);
	buf = (U8*)malloc(datasize);
	if(buf==0)
	{
		debugstring("memory is insufficient to load file\n");
		return 0;
	}
	f_read(&fp,buf,datasize,&nRead);
	f_close(&fp);
	SURFACE *surf = loadtgap(buf);
	free(buf);
	return surf;
}

