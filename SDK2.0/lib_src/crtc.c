/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"
#include "surface.h"


static U32 screen_w;
static U32 screen_h;
static U32 screen_bpp=16;

SURFACE* createframe(U32 w, U32 h, U32 bpp)
{
	SURFACE* fsurf = create_surface(w,h,bpp);
	return fsurf;
}
static volatile bool framesync=false;
void framesync_isr()
{
	framesync=true;
}
static SURFACE* dframe[2]={0,};
static int curframenum=0;
void flip()
{
	if(curframenum)
	{
		curframenum=0;
		setframebuffer(dframe[0]);
	}
	else
	{
		curframenum=1;
		setframebuffer(dframe[1]);
	}
}

void setdoubleframebuffer(SURFACE* surf,SURFACE* surf2)
{
	dframe[0]=surf;
	dframe[1]=surf2;
	*R_CRTBA0 = (U32)surf->pixels;
	*R_CRTWIDTH = surf->w;
	*R_CRTBA1 = (U32)surf2->pixels;
}

SURFACE* getbackframe()
{
	if(curframenum)
		return dframe[0];
	else
		return dframe[1];
}
SURFACE* getfrontframe()
{
	if(curframenum)
		return dframe[1];
	else
		return dframe[0];
}

void setframebuffer(SURFACE* surf)
{
#ifdef ADSTAR_REV
	*R_CRTBA0 = (U32)surf->pixels;
	delayms(10);
#else
	framesync = false;
	CRITICAL_ENTER();
	enable_interrupt(INTNUM_FRAMESYNC,TRUE);
	CRITICAL_EXIT();
	while(1)
	{
		if(framesync == true)
			break;
	}
	*R_CRTBA0 = (U32)surf->pixels;
	*R_CRTWIDTH = surf->w;
	enable_interrupt(INTNUM_FRAMESYNC,FALSE);
#endif
}

void setframebufferxy(SURFACE* surf,U32 x, U32 y)
{
	*R_CRTBA0 = ((U32)surf->pixels)+(x * (surf->bpp/8))+( y * (U32)surf->pitch);
}

U32 getscreenwidth()
{
	return screen_w;
}

U32 getscreenheight()
{
	return screen_h;
}
U32 getscreenpitch()
{
	//32bit
	if(screen_bpp==32)
	{
		return (*R_CRTWIDTH) *4;
	}
	else
	{
		return (*R_CRTWIDTH )*2;
	}
	//16bit
}

U32 getscreenbpp()
{
	return screen_bpp;
}


void setscreenex(U32 width,U32 height,U32 scmode,U32 ht,U32 hs, U32 ha, U32 vt, U32 vs, U32 va)
{
	*R_CRTHT  = ht;
	*R_CRTHS  = hs;
	*R_CRTHA  = ha;
	*R_CRTVT  = vt;
	*R_CRTVS  = vs;
	*R_CRTVA  = va;
	screen_w= width;
	screen_h = height;
	
	U32 conval=0;
	DEBUGPRINTF(" CRTC %d x %d Setting Done\r\n",width,height);
	if(scmode&SCREENMODE_RGB888)
	{
		debugstring("RGB888 Mode\r\n");
		screen_bpp = 32;
		conval |= (1<<13);
	}
	else
	{
		debugstring("RGB565 Mode\r\n");
		screen_bpp = 16;
		conval |= (1<<12);
	}
	conval |= (1<<4);//line pitch not align 512,1024
	*R_CRTWIDTH = screen_w;
	*R_CRTCON = conval;
	set_interrupt(INTNUM_FRAMESYNC,framesync_isr);
	enable_interrupt(INTNUM_FRAMESYNC,FALSE);
}

void setscreen(SCREENRES res,U32 scmode)
{
	switch(res)
	{
	case SCREEN_480x272:
		setscreenex(480,272,scmode,0x0000020D,0x0002002B,0x002D020D ,0x0000011E ,0x0002000C ,0x000E011E );
		break;
	case SCREEN_640x480: 
		setscreenex(640,480,scmode,0x00000320,0x00130073,0x00A00320 ,0x0000020D ,0x0007000D ,0x002B020B );
		break;
	case SCREEN_800x480:
		setscreenex(800,480,scmode,824,(8<<16)|14,(24<<16)|824 ,493 ,(5<<16)|2 ,(13<<16)|493 );
		break;
	case SCREEN_800x600:
		setscreenex(800,600,scmode,0x00000420,0x002300b0,0x01000420 ,0x00000274 ,0x0004000a ,0x001a0272 );
		break;
	default:
		debugstring("invalid screen size\r\n");
		return;
	}
}
