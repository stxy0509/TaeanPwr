
/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"
#include "surface.h"


#include <setjmp.h>

#include <libjpg/jpeglib.h>

/* Define this for fast loading and not as good image quality */
#define FAST_JPEG

/* Define this for quicker (but less perfect) JPEG identification */
#define FAST_IS_JPEG

/* See if an image is contained in a data source */
int IMG_isJPG(U8 *src)
{
	int is_JPG;
	int in_scan;
	U8 magic[4];
	U8 fp=0;

	/* This detection code is by Steaphan Greene <stea@cs.binghamton.edu> */

	is_JPG = 0;
	in_scan = 0;
	memcpy(magic,src+fp,2);
	fp+=2;
	{
		if ( (magic[0] == 0xFF) && (magic[1] == 0xD8) ) {
			is_JPG = 1;
			while (is_JPG == 1) {
				memcpy(magic,src+fp,2);
				fp+=2;
				if( (magic[0] != 0xFF) && (in_scan == 0) ) {
					is_JPG = 0;
				} else if( (magic[0] != 0xFF) || (magic[1] == 0xFF) ) {
					/* Extra padding in JPEG (legal) */
					/* or this is data and we are scanning */
					fp-=1;
				} else if(magic[1] == 0xD9) {
					/* Got to end of good JPEG */
					break;
				} else if( (in_scan == 1) && (magic[1] == 0x00) ) {
					/* This is an encoded 0xFF within the data */
				} else if( (magic[1] >= 0xD0) && (magic[1] < 0xD9) ) {
					/* These have nothing else */
				} else {
					memcpy(magic+2,src+fp,2);
					fp+=2;
					/* Yes, it's big-endian */
					U32 size;
					U32 end;
					U32 start;
					start = fp;
					size = (magic[2] << 8) + magic[3];
					fp = size-2;
					end = fp;
					
					if ( end != start + size - 2 ) is_JPG = 0;
					if ( magic[1] == 0xDA ) {
						/* Now comes the actual JPEG meat */
#ifdef	FAST_IS_JPEG
						/* Ok, I'm convinced.  It is a JPEG. */
						break;
#else
						/* I'm not convinced.  Prove it! */
						in_scan = 1;
#endif
					}
				}
			}
		}
	}
	
	return(is_JPG);
}

typedef struct {
	struct jpeg_source_mgr pub;

	U32 buflen;
	U8* buffer;
	U32 curfp;
} my_source_mgr;

/*
 * Initialize source --- called by jpeg_read_header
 * before any data is actually read.
 */
static void init_source (j_decompress_ptr cinfo)
{
	/* We don't actually need to do anything */
	return;
}

/*
 * Fill the input buffer --- called whenever buffer is emptied.
 */
static boolean fill_input_buffer (j_decompress_ptr cinfo)
{
	my_source_mgr * src = (my_source_mgr *) cinfo->src;
	if(src->curfp == src->buflen)
	{
		src->buffer[0] = (U8) 0xFF;
		src->buffer[1] = (U8) JPEG_EOI;
		src->pub.bytes_in_buffer = 2;
	}
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = src->buflen;
	src->curfp= src->buflen;

	return TRUE;
}


/*
 * Skip data --- used to skip over a potentially large amount of
 * uninteresting data (such as an APPn marker).
 *
 * Writers of suspendable-input applications must note that skip_input_data
 * is not granted the right to give a suspension return.  If the skip extends
 * beyond the data currently in the buffer, the buffer can be marked empty so
 * that the next read will cause a fill_input_buffer call that can suspend.
 * Arranging for additional bytes to be discarded before reloading the input
 * buffer is the application writer's problem.
 */
static void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
	my_source_mgr * src = (my_source_mgr *) cinfo->src;

	/* Just a dumb implementation for now.	Could use fseek() except
	 * it doesn't work on pipes.  Not clear that being smart is worth
	 * any trouble anyway --- large skips are infrequent.
	 */
	if (num_bytes > 0) {
		while (num_bytes > (long) src->pub.bytes_in_buffer) {
			num_bytes -= (long) src->pub.bytes_in_buffer;
			(void) src->pub.fill_input_buffer(cinfo);
			/* note we assume that fill_input_buffer will never
			 * return FALSE, so suspension need not be handled.
			 */
		}
		src->pub.next_input_byte += (size_t) num_bytes;
		src->pub.bytes_in_buffer -= (size_t) num_bytes;
	}
}

/*
 * Terminate source --- called by jpeg_finish_decompress
 * after all data has been read.
 */
static void term_source (j_decompress_ptr cinfo)
{
	if(cinfo->src)
		free(cinfo->src);
	return;
}

/*
 * Prepare for input from a stdio stream.
 * The caller must have already opened the stream, and is responsible
 * for closing it after finishing decompression.
 */
static void jpeg_src (j_decompress_ptr cinfo, U8 *ctx,U32 len)
{
  my_source_mgr *src;

  /* The source object and input buffer are made permanent so that a series
   * of JPEG images can be read from the same file by calling jpeg_stdio_src
   * only before the first one.  (If we discarded the buffer at the end of
   * one image, we'd likely lose the start of the next one.)
   * This makes it unsafe to use this manager and a different source
   * manager serially with the same JPEG object.  Caveat programmer.
   */
  if (cinfo->src == NULL) {	/* first time for this JPEG object? */
/*
	cinfo->src = (struct jpeg_source_mgr *)
	  (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
				  sizeof(my_source_mgr));*/
	cinfo->src = (struct jpeg_source_mgr *)malloc(sizeof(my_source_mgr));
  }

  src = (my_source_mgr *) cinfo->src;
  src->pub.init_source = init_source;
  src->pub.fill_input_buffer = fill_input_buffer;
  src->pub.skip_input_data = skip_input_data;
  src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
  src->pub.term_source = term_source;
  src->buffer = ctx;
  src->buflen = len;
  src->curfp = 0;
  src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
  src->pub.next_input_byte = NULL; /* until buffer loaded */
}

struct my_error_mgr {
	struct jpeg_error_mgr errmgr;
	jmp_buf escape;
};

static void my_error_exit(j_common_ptr cinfo)
{
	struct my_error_mgr *err = (struct my_error_mgr *)cinfo->err;
	longjmp(err->escape, 1);
}

static void output_message(j_common_ptr cinfo)
{
	/* do nothing */
}

/* Load a JPEG type image from an SDL datasource */
SURFACE *LoadSurfaceInfoFromJPG_r(U8 *src,U32 len)
{
	struct jpeg_decompress_struct cinfo;
	JSAMPROW rowptr[1];
	SURFACE *volatile surface = NULL;
	U8* outputbuf;
	struct my_error_mgr jerr;
	U32 image_w,image_h;
	if ( !src ) {
		/* The error message has been set in SDL_RWFromFile */
		return NULL;
	}
	
	/* Create a decompression structure and load the JPEG header */
	cinfo.err = jpeg_std_error(&jerr.errmgr);
	jerr.errmgr.error_exit = my_error_exit;
	jerr.errmgr.output_message = output_message;
#if 1
	if(setjmp(jerr.escape)) 
	{
		/* If we get here, libjpeg found an error */
		jpeg_destroy_decompress(&cinfo);
		if ( surface != NULL ) {
			release_surface(surface);
		}
		return NULL;
	}
#endif

	jpeg_create_decompress(&cinfo);

	jpeg_src(&cinfo, src,len);

	jpeg_read_header(&cinfo, TRUE);
	if(cinfo.num_components == 4) 
	{
		debugstring("(JPEG)32bit bmp mode is unsupported");
		jpeg_destroy_decompress(&cinfo);
		return NULL;
	} else {
		/* Set 24-bit RGB output */
		cinfo.out_color_space = JCS_RGB;
		cinfo.quantize_colors = FALSE;
#ifdef FAST_JPEG
		cinfo.scale_num   = 1;
		cinfo.scale_denom = 1;
		cinfo.dct_method = JDCT_FASTEST;
		cinfo.do_fancy_upsampling = FALSE;
#endif
		jpeg_calc_output_dimensions(&cinfo);

		/* Allocate an output surface to hold the image */
		/*
		surface = SDL_AllocSurface(SDL_SWSURFACE,
				cinfo.output_width, cinfo.output_height, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
						   0x0000FF, 0x00FF00, 0xFF0000,
#else
						   0xFF0000, 0x00FF00, 0x0000FF,
#endif
						   0);
						   */
	}
	
	image_w = cinfo.output_width;
	image_h = cinfo.output_height;
	
	outputbuf = malloc(cinfo.output_width* cinfo.output_height*3);
	if ( outputbuf == NULL ) {
		jpeg_destroy_decompress(&cinfo);
		debugstring("Out of memory");
		return NULL;
	}

	/* Decompress the image */
	jpeg_start_decompress(&cinfo);
	while ( cinfo.output_scanline < cinfo.output_height ) {
		rowptr[0] = (JSAMPROW)(U8 *)outputbuf +
							cinfo.output_scanline * cinfo.output_width*3;
		jpeg_read_scanlines(&cinfo, rowptr, (JDIMENSION) 1);
	}
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	//24bit to 16bit
	int x,y;
	U32 sptr;
	U32 screenbpp = getscreenbpp();
	surface = create_surface(image_w,image_h,screenbpp);
	if ( surface == NULL ) {
		debugstring("Out of memory");
		free(outputbuf);
		return NULL;
	}
	sptr = (U32)surface->pixels;
	if(screenbpp==32)
	{
		surface->pixtype = PIX_FMT_RGB888;
		for(y=0;y<image_h;y++) {
			for(x=0;x<image_w;x++) {
				*(U32*)(sptr + x*4 + y * surface->pitch) = MAKE_RGB888(outputbuf[x*3 + y*image_w*3],outputbuf[x*3+1 + y*image_w*3],outputbuf[x*3+2 + y*image_w*3]);
			}
		}
	}
	else
	{
		surface->pixtype = PIX_FMT_RGB565;
		for(y=0;y<image_h;y++) {
			for(x=0;x<image_w;x++) {
				*(U16*)(sptr + x*2 + y * surface->pitch) = MAKE_RGB565(outputbuf[x*3 + y*image_w*3],outputbuf[x*3+1 + y*image_w*3],outputbuf[x*3+2 + y*image_w*3]);
			}
		}
	}
	free(outputbuf);
	return(surface);
}

static char* readfile(char* fname,int* len)
{
	FIL fp;
	int size;
	char* pngbuf;
	U32 nRead;
	FRESULT res = f_open(&fp,fname,FA_READ|FA_OPEN_EXISTING);
	if( res != FR_OK )
	{
		DEBUGPRINTF("Cannot open : %s\r\n", fname);
		return 0;
	}
	size = f_size(&fp);
	pngbuf=malloc(size);
	if(pngbuf==0)
		return 0;
	f_read(&fp,pngbuf,size,&nRead);
	f_close(&fp);
	*len=size;
	return pngbuf;
}

SURFACE* loadjpg( char* filename)
{
	int len;
	U8* buf = readfile(filename,&len);
	if(buf)
	{		
		SURFACE* p = loadjpgp(buf,len);		
		free(buf);
		return p;
	}
	return NULL;
} 

SURFACE* loadjpgp( U8* databuf,U32 len )
{
	return LoadSurfaceInfoFromJPG_r(databuf,len);
}

