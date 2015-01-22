/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include <stdlib.h>

static void* mymsg_handler( EGL_MSG* pMsg )
{
    void* ( *msg_handler )( EGL_MSG * pMsg );
    EGL_OBJECT_PTR pObj = ( EGL_OBJECT_PTR )pMsg->hObj;
    msg_handler = ( void * ( * )( EGL_MSG * pMsg ) )pObj->selfdata;
    if( pMsg->msgID != EGL_MSG_DELETE )
    {
        return msg_handler( pMsg ); //call user function
    }
    msg_handler( pMsg ); //call user function
    if( pMsg->msgID == EGL_MSG_DELETE )
    {
        prv_egl_delete_object( pObj );
    }
    return NULL;
}

EGL_HANDLE egl_create_custom_object( int x, int y, int w, int h, void * ( *msg_handler )( EGL_MSG* pMsg ) )
{
    EGL_OBJECT_PTR pOjb ;
    if( msg_handler == NULL )
        return NULL;
    pOjb = prv_egl_create_object( x, y, w, h );
    pOjb->msg_hanlder = mymsg_handler;
    pOjb->selfdata = msg_handler;
    return ( EGL_HANDLE )pOjb;
}
