
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

	SURFACE* setdrawtarget(SURFACE* surf) __attribute__ ((deprecated)) ;
	SURFACE* getdrawtarget() __attribute__ ((deprecated)) ;
	SURFACE* createsurface(U32 w,U32 h, U32 bpp) __attribute__ ((deprecated)) ;
	SURFACE* createsurface_from(SURFACE* src,U32 option) __attribute__ ((deprecated)) ;//if option is 0, make clone
	void releasesuface(SURFACE* surf) __attribute__ ((deprecated)) ;
	PALETTE* createpalette(int bpp, int nColors) __attribute__ ((deprecated)) ;
	void releasepalette(PALETTE* pal) __attribute__ ((deprecated)) ;//called by release_surface
	BOOL drawsurfaceRect(SURFACE* src_surf,int dx, int dy, int sx,int sy,int w,int h) __attribute__ ((deprecated)) ;
	BOOL drawsurface(SURFACE* src_surf,int dx,int dy) __attribute__ ((deprecated)) ;
	BOOL drawsurfacescalerect(SURFACE* src_surf,int dx, int dy,int dw,int dh,int sx, int sy, int sw, int sh) __attribute__ ((deprecated)) ;
	BOOL drawsurfacescale(SURFACE* src_surf,int dx, int dy,int dw,int dh) __attribute__ ((deprecated)) ;
	CLIP_RECT drawsetclipwindow(SURFACE* dst, CLIP_RECT* pRect) __attribute__ ((deprecated)) ;

#define createsurface  create_surface
#define createsurface_from  create_surface_from
#define releasesurface  release_surface

#define createpalette  create_palette
#define releasepalette  release_palette
#define drawsurfaceRect draw_surface_rect
#define drawsurface draw_surface
#define drawsurfacescalerect  draw_surface_scalerect
#define drawsurfacescale  draw_surface_scale
#define drawsetclipwindow  draw_set_clip_window

	void drawsetrgb( U8 sr, U8 sg, U8 sb ) __attribute__ ((deprecated)) ;
	void drawgetrgb( U8* dr, U8* dg, U8* db ) __attribute__ ((deprecated)) ;
	void drawputpixel( int x, int y, U8 r, U8 g, U8 b ) __attribute__ ((deprecated)) ;
	void drawline( int x1, int y1, int x2, int y2 ) __attribute__ ((deprecated)) ;
	void line2( int x, int y, int x1, int y2 ) __attribute__ ((deprecated)) ;
	void xyline3( int x, int y, int x1, int y2, int x3 ) __attribute__ ((deprecated)) ;
	void yxline( int x, int y, int endy ) __attribute__ ((deprecated)) ;
	void yxline2( int x, int y, int y1, int x2, int y3 ) __attribute__ ((deprecated)) ;

	void drawrect( int x, int y, int w, int h ) __attribute__ ((deprecated)) ;
	void drawrectfill( int x, int y, int w, int h ) __attribute__ ((deprecated)) ;
	void drawcircle ( int x, int y, int rad ) __attribute__ ((deprecated)) ;
	void drawcirclefill( int x, int y, int rad ) __attribute__ ((deprecated)) ;
	void drawellipse( int x, int y, int Xradius, int Yradius ) __attribute__ ((deprecated)) ;
	void drawellipsefill( int x, int y, int Xradius, int Yradius ) __attribute__ ((deprecated)) ;
	
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
