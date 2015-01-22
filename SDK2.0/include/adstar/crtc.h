/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once
#define CRTC_BASE   0x80022400

#define R_CRTBA       ((volatile unsigned long *)(CRTC_BASE + 0x00))
#define R_CRTHT       ((volatile unsigned long *)(CRTC_BASE + 0x04))
#define R_CRTHS       ((volatile unsigned long *)(CRTC_BASE + 0x08))
#define R_CRTHA       ((volatile unsigned long *)(CRTC_BASE + 0x0C))
#define R_CRTVT       ((volatile unsigned long *)(CRTC_BASE + 0x10))
#define R_CRTVS       ((volatile unsigned long *)(CRTC_BASE + 0x14))
#define R_CRTVA       ((volatile unsigned long *)(CRTC_BASE + 0x18))
#define R_CRTXY       ((volatile unsigned long *)(CRTC_BASE + 0x1C))
#define R_CRTSR       ((volatile unsigned long *)(CRTC_BASE + 0x20))
#define R_CRTCON      ((volatile unsigned long *)(CRTC_BASE + 0x24))
#define R_CRTPM       ((volatile unsigned long *)(CRTC_BASE + 0x2C))
#define R_CRTBA0       ((volatile unsigned long *)(CRTC_BASE + 0x30))
#define R_CRTBA1       ((volatile unsigned long *)(CRTC_BASE + 0x34))
#define R_CRTFCTL	((volatile unsigned long *)(CRTC_BASE + 0x40))

#define R_CRTWIDTH		((volatile unsigned long *)(CRTC_BASE + 0x3c))

typedef enum {
	SCREEN_480x272=0,
	SCREEN_640x480,
	SCREEN_800x480,
	SCREEN_800x600,
}SCREENRES;

#define SCREENMODE_RGB888		(1<<2)
#define SCREENMODE_RGB565		(1<<3)



SURFACE* createframe(U32 w, U32 h, U32 bpp); // don't use "releasesurface()" for release. you can't release frame.
void setframebuffer(SURFACE* surf);
void setframebufferxy(SURFACE* surf,U32 x, U32 y);//if 16bit mode, x must be align 2**n
U32 getscreenwidth();
U32 getscreenheight();
U32 getscreenpitch();
U32 getscreenbpp();

void setscreenex(U32 width,U32 height,U32 scmode,U32 ht,U32 hs, U32 ha, U32 vt, U32 vs, U32 va);
void setscreen(SCREENRES size,U32 doublebuffer);
//for double buffering
void flip();
void setdoubleframebuffer(SURFACE* surf,SURFACE* surf2);
SURFACE* getbackframe();
SURFACE* getfrontframe();
