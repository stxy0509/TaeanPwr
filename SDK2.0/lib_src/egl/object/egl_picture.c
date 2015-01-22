/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include "egl_theme.h"
#include <stdlib.h>
#include "object/egl_label.h"
#include "object/egl_picture.h"

static void delete_picture( EGL_HANDLE hObj );
static void draw( EGL_OBJECT_PTR picture )
{
    SURFACE* surf = ( SURFACE* )picture->selfdata;
    DRAW_SURFACE_RECT( surf, picture->rect.x, picture->rect.y, 0, 0, picture->rect.w, picture->rect.h );
}

static void* picture_msg_hanlder( EGL_MSG* pMsg )
{
    EGL_OBJECT_PTR picture = ( EGL_OBJECT_PTR )pMsg->hObj;
    switch ( pMsg->msgID )
    {
    case EGL_MSG_DRAW:
        draw( picture );
        break;
    case EGL_MSG_TOUCHED:
    case EGL_MSG_UNTOUCHED:
        if( picture->cb )
        {
            picture->cb( pMsg->hObj, PICTURE_CLICKED );
        }
        break;
    case EGL_MSG_MOVE:
        picture->rect.x = pMsg->param.point.x;
        picture->rect.y = pMsg->param.point.y;
        picture->bNeedRedraw = TRUE;
        break;
    case EGL_MSG_DELETE:
        delete_picture( pMsg->hObj );
        break;
    case EGL_MSG_FOCUS:
    case EGL_MSG_UNFOCUS:
    case EGL_MSG_KEY_UP:
    case EGL_MSG_KEY_DOWN:
    case EGL_MSG_TIMETICK:
    default:
        break;
    }
    return NULL;
}

/*
* GLOBAL API
*/

EGL_HANDLE egl_create_picture( SURFACE* surf, int x, int y, int w, int h )
{
    EGL_OBJECT_PTR picture;
    EGL_HANDLE label = egl_create_label( x, y, w, h, "PICTURE" );
    if( label == NULL )
        return NULL;
    picture = prv_egl_create_object( x, y, w, h );
    if( picture == NULL )
    {
        prv_egl_delete_object( ( EGL_OBJECT_PTR )label );
        return NULL;
    }
    picture->msg_hanlder = picture_msg_hanlder;
    picture->selfdata = ( void* )surf;
    return ( EGL_HANDLE )picture;
}
static void delete_picture( EGL_HANDLE hObj )
{
    EGL_OBJECT_PTR pObj;
    if( hObj == NULL )
        return ;
    pObj = ( EGL_OBJECT_PTR )hObj;
    prv_egl_delete_object( pObj );
}

BOOL egl_picture_callback( EGL_HANDLE hObj, EVENT_CALLBACK cb )
{
    EGL_OBJECT_PTR pObj = ( EGL_OBJECT_PTR )hObj;
	CRITICAL_ENTER();
    pObj->cb = cb;
	CRITICAL_EXIT();
    return TRUE;
}

SURFACE* egl_picture_set( EGL_HANDLE hObj, SURFACE* surf )
{
    SURFACE* old;
    EGL_OBJECT_PTR pObj = ( EGL_OBJECT_PTR )hObj;
	CRITICAL_ENTER();
    old = pObj->selfsurface;
    pObj->selfsurface = surf;
	CRITICAL_EXIT();
    return old;
}
