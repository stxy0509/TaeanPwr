/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include "egl_window.h"
#include "egl_theme.h"
#include "font/bitfont.h"

EGL_FONT* egl_sys_font;
void* egl_sendmsg( EGL_HANDLE h,EGL_MSG_ID msgID)
{
	EGL_MSG Msg;
    EGL_OBJECT_PTR pObj;
	Msg.msgID = msgID;
    //window_msg_handler( pMsg->hWin, pMsg );
    pObj = ( EGL_OBJECT_PTR )h;
    return pObj->msg_hanlder( &Msg);
}

void egl_show_object( EGL_HANDLE h, BOOL bShow )
{
    EGL_OBJECT_PTR pObj = ( EGL_OBJECT_PTR )h;
    if( pObj == NULL )
        return ;
    if(bShow==FALSE && pObj->bVisible==TRUE && pObj->parentWin==(EGL_WINDOW_PTR)egl_window_get_active())
	{
		egl_window_invalidate_rect(&pObj->rect);
		CRITICAL_ENTER();
	}
	else
	{
		CRITICAL_ENTER();
		pObj->bNeedRedraw = TRUE;
	}
	pObj->bVisible = bShow;
	CRITICAL_EXIT();
}
EGL_FONT* egl_create_default_font()
{
	return create_bitfont();
}
EGL_FONT* egl_get_default_font()
{
	return egl_sys_font;
}

BOOL egl_init()
{
    if( theme_init() == FALSE )
        return FALSE;
    egl_sys_font = egl_create_default_font();
    if( egl_sys_font == NULL )
        return FALSE;
    return TRUE;
}

EGL_FONT* egl_get_font(EGL_HANDLE h)
{
	EGL_OBJECT_PTR pObj = EGL_HANDLE_TO_OBJECT(h);
	return pObj->pFont;
}
EGL_FONT* egl_set_font(EGL_HANDLE h,EGL_FONT* font)
{
	EGL_FONT* old;
	EGL_OBJECT_PTR pObj = EGL_HANDLE_TO_OBJECT(h);
	CRITICAL_ENTER();
	old =pObj->pFont;
	pObj->pFont=font;
	CRITICAL_EXIT();
	return old;
}
/*
void egl_delete_object( EGL_HANDLE handle )
{
  egl_sendmsg(handle,EGL_MSG_DELETE);
}
*/
void egl_object_set_redraw(EGL_HANDLE handle)
{
	EGL_OBJECT_PTR pObj = ( EGL_OBJECT_PTR )handle;
	CRITICAL_ENTER();
	pObj->bNeedRedraw=TRUE;
	CRITICAL_EXIT();
}
