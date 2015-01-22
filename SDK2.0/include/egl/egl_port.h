#pragma once

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif

#include "egl_base.h"
#include "egl_typedef.h"

#include "surface.h"
#define DRAW_SURFACE		draw_surface
#define DRAW_SURFACE_RECT	draw_surface_rect
#define DRAW_SURFACE_SCALE_RECT	draw_surface_scalerect
#define DRAW_SURFACE_SCALE	draw_surface_scale

#define EGL_GET_TARGET_SURFACE		get_draw_target
#define EGL_SET_TARGET_SURFACE		set_draw_target
#define CREATE_SURFACE		create_surface
#define RELEASE_SURFACE		release_surface

#include "primitives/draw_rect.h"
#include "primitives/draw_line.h"
#include "primitives/draw_poly.h"
#include "primitives/draw_arc.h"
#define DRAW_RECT			draw_rect
#define DRAW_RECTFILL		draw_rectfill
#define DRAW_RECTFILLGRADIENT	draw_rectfill_gradient
#define DRAW_VLINE			draw_vline
#define DRAW_HLINE			draw_hline
#define DRAW_LINE			draw_line
#define DRAW_THICKLINE		draw_thickline

#define DRAW_CIRCLE			draw_circle
#define DRAW_CIRCLEFILL		draw_circlefill

#define DRAW_POLYGON		draw_polygon
#define DRAW_POLYGONFILL	draw_polygonfill

#define DRAW_ELLIPSE		draw_ellipse
#define DRAW_ELLIPSE_fill	draw_ellipsefill
#define DRAW_ELLIPSE_AA		draw_ellipse_aa

#include "adstar/crtc.h"
#define EGL_SCREEN_WIDTH	getscreenwidth()
#define EGL_SCREEN_HEIGHT	getscreenheight()
#define EGL_SCREEN_BPP		getscreenbpp()

#ifndef CRITICAL_ENTER	
#define CRITICAL_ENTER()	asm("set 8");asm("sync") // back up interrupt status
#endif

#ifndef CRITICAL_EXIT	
#define CRITICAL_EXIT()		asm("clr 8");asm("sync") // restor interrupt status
#endif

#define INLINE inline

#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
