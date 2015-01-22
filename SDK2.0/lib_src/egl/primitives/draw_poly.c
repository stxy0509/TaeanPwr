#include "egl_typedef.h"
#include <stdlib.h>
#include "primitives/draw_line.h"
#include "primitives/draw_poly.h"
#include "egl_private.h"

void r_draw_polyline(EGL_POINT* p, int n,EGL_COLOR c)
{
	int i;
	for(i=1;i<n;i++)
		r_draw_line(p[i-1].x,p[i-1].y,p[i].x,p[i].y,c);
}
void draw_polyline(EGL_POINT* p, int n,EGL_COLOR c)
{
	r_draw_polyline(p, n,c);
	SURFACE_FLUSH();
}
void r_draw_polyline_aa(EGL_POINT* p, int n,EGL_COLOR c)
{
	int i;
	for(i=1;i<n;i++)
		r_draw_line_aa(p[i-1].x,p[i-1].y,p[i].x,p[i].y,c);
}
void draw_polyline_aa(EGL_POINT* p, int n,EGL_COLOR c)
{
	r_draw_polyline_aa(p, n,c);
	SURFACE_FLUSH();
}
void r_draw_polygon(EGL_POINT* ptable,int n,EGL_COLOR c)
{
	r_draw_polyline(ptable,n,c);
	r_draw_line(ptable[0].x,ptable[0].y,ptable[n-1].x,ptable[n-1].y,c);
}
void draw_polygon(EGL_POINT* ptable,int n,EGL_COLOR c)
{
	r_draw_polygon(ptable,n,c);
	SURFACE_FLUSH();
}
void r_draw_polygon_aa(EGL_POINT* ptable,int n,EGL_COLOR c)
{
	r_draw_polyline_aa(ptable,n,c);
	r_draw_line_aa(ptable[0].x,ptable[0].y,ptable[n-1].x,ptable[n-1].y,c);
}
void draw_polygon_aa(EGL_POINT* ptable,int n,EGL_COLOR c)
{
	r_draw_polygon_aa(ptable,n,c);
	SURFACE_FLUSH();
}
