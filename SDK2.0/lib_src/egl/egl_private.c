/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include <stdlib.h>


EGL_OBJECT_PTR prv_egl_create_object(int x, int y, int w, int h)
{
	EGL_OBJECT_PTR obj = (EGL_OBJECT_PTR)malloc(sizeof(EGL_OBJECT));
	if(obj==NULL)
		return NULL;
	obj->bNeedRedraw = TRUE;
	obj->bEnabled = TRUE;
	obj->bVisible = TRUE;
	obj->rect.x=x;
	obj->rect.y=y;
	obj->rect.w=w;
	obj->rect.h=h;
	obj->selfdata = NULL;
	obj->cb = NULL;
	obj->pFont = egl_sys_font;
	obj->selfsurface=NULL;
	return obj;
}

BOOL prv_egl_delete_object(EGL_OBJECT_PTR pObj)
{
#ifdef _DEBUG
	if(pObj==NULL)
	{
		return FALSE;
	}
#endif
	free(pObj);
	return TRUE;
}

BOOL prvCollision_detect_rect(EGL_RECT* object1, EGL_RECT* object2)
{ 
	int left1, left2;
	int right1, right2;        
	int top1, top2;
	int bottom1, bottom2;        
	left1 = object1->x;
	left2 = object2->x;        
	right1 = object1->x + object1->w;
	right2 = object2->x + object2->w;
	top1 = object1->y;
	top2 = object2->y;
	bottom1 = object1->y + object1->h;
	bottom2 = object2->y + object2->h;
	if (bottom1 < top2) return FALSE;
	if (top1 > bottom2) return FALSE;
	if (right1 < left2) return FALSE;
	if (left1 > right2) return FALSE;
	return TRUE;
}
