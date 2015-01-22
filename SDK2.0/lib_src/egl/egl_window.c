/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include "egl_window.h"
#include <stdlib.h>

#ifdef _WIN32
#include <memory.h>
#endif

#include <string.h>

typedef struct _tagWinList
{
	EGL_WINDOW_PTR pWin;
	struct _tagWinList* prev;
	struct _tagWinList* next;
} WINLIST;
static WINLIST * pheadwinlist = 0;

static EGL_WINDOW_PTR pCurWin = NULL;

/*
private function
*/

static EGL_OBJECT_PTR find_object( EGL_WINDOW_PTR pWin, EGL_POINT* pPt )
{
	//ã���� �Է��� �������� ã�´�.
	int i;
	int x;
	int y;
	x = pPt->x;
	y = pPt->y;
	for( i = pWin->object_cnt - 1; i != -1; i-- )
		//for(i=0;i<m_wglist.size();i++)
	{
		EGL_OBJECT_PTR pObj = pWin->objets[i];
		if( pObj->bVisible && pObj->bEnabled )
		{
			if( ( x > pObj->rect.x )  && ( x < ( pObj->rect.x + pObj->rect.w ) ) && ( y > ( pObj->rect.y ) ) && ( y < ( pObj->rect.y + pObj->rect.h ) ) )
			{
				return pObj;
			}
		}
	}
	return NULL;
}
static void window_draw_bg( EGL_WINDOW_PTR pWin, EGL_RECT* pRect )
{
	//draw whole
	if( pRect == NULL || pRect->w == -1 )
	{
		if( pWin->bg )
			DRAW_SURFACE( pWin->bg, 0, 0 );
		else
			DRAW_RECTFILL( 0, 0, pWin->rect.w, pWin->rect.h, EGL_CONFIG_WINDOW_DEFAULT_BG_COLOR );
	}
	else
	{
		if( pWin->bg )
			DRAW_SURFACE_RECT( pWin->bg, pRect->x, pRect->y, pRect->x, pRect->y, pRect->w, pRect->h);
		else
			DRAW_RECTFILL( pRect->x, pRect->y, pRect->w, pRect->h, EGL_CONFIG_WINDOW_DEFAULT_BG_COLOR );
		//draw partial area
	}
}

static void window_draw( EGL_WINDOW_PTR pWin )
{
	int i;
	EGL_MSG msg;
	msg.hWin = ( EGL_HANDLE )pWin;
	msg.msgID = EGL_MSG_DRAW;
	if( pWin->bNeedRedraw )
	{
		window_draw_bg( pWin, &pWin->redraw_rect );
		pWin->bNeedRedraw = FALSE;
	}
	for( i = 0; i < pWin->object_cnt; i++ )
	{
		EGL_OBJECT_PTR pObj = pWin->objets[i];
		msg.hObj = ( EGL_HANDLE )pObj;
		if( pObj->bVisible && pObj->bNeedRedraw )
		{
			pObj->bNeedRedraw = FALSE;
			pObj->msg_hanlder( &msg );
			//user can set bNeedRedraw as TRUE for next frame.
		}
	}
}

INLINE EGL_WINDOW_PTR get_active_window()
{
	return pCurWin;
}


static void* window_msg_handler( EGL_HANDLE hWin, EGL_MSG* pMsg )
{
	static EGL_OBJECT_PTR pObj_old=NULL;
	static EGL_MSG_ID pmsgid_old=(EGL_MSG_ID)-1;
	EGL_OBJECT_PTR pObj;
	EGL_WINDOW_PTR pWin = ( EGL_WINDOW_PTR )hWin;
	//find object
	pObj = find_object( pWin, &pMsg->param.point ) ;

	if( (pMsg->msgID == EGL_MSG_UNTOUCHED) || (pMsg->msgID == EGL_MSG_TOUCHED))
	{
		if((pObj_old != NULL) && (pObj_old != pObj) && (pmsgid_old==EGL_MSG_TOUCHED))
		{
			EGL_MSG tmsg;
			tmsg.hObj = ( EGL_HANDLE )pObj_old;
			tmsg.msgID = EGL_MSG_UNTOUCHED;
			pObj_old->msg_hanlder( &tmsg );
		}
	}
	pObj_old = pObj;
	pmsgid_old = pMsg->msgID;
	if( pObj !=NULL)
	{
		pMsg->hObj = ( EGL_HANDLE )pObj;
		return pObj->msg_hanlder( pMsg );
	}
#ifdef _DEBUG
	else
	{
		printf( "object not found\n" );
	}
#endif
	return NULL;
}

/*
window manager
*/
static BOOL wm_show_window( EGL_WINDOW_PTR pWin, BOOL bShow )
{
	WINLIST * winlist = pheadwinlist;
	if(pheadwinlist==NULL)
		return FALSE;
	if( bShow )
	{
		//�ֻ��� window �̸鼭 ���� �������� ���� ���
		if(pCurWin == NULL && winlist->pWin==pWin)
		{
			pCurWin = pWin;
			window_draw( pCurWin );
			return TRUE;
		}
		//find pWin;
		if( pCurWin == pWin )
		{
			window_draw(pCurWin);
			return TRUE;
		}
		if(pCurWin!=NULL)
			winlist = winlist->next;
		while( winlist)
		{
			if( winlist->pWin == pWin )
			{
				WINLIST* oldhead;
				WINLIST* prev = winlist->prev;
				WINLIST* next = winlist->next;
				oldhead = pheadwinlist;
				prev->next = next;
				if(next)
					next->prev = prev;
				winlist->next = oldhead;
				winlist->prev = NULL;
				oldhead->prev = winlist;

				pheadwinlist = winlist;
				pCurWin = pWin;
				egl_window_invalidate();
				window_draw( pCurWin );
				return TRUE;
			}
			winlist = winlist->next;
		}
		return FALSE;
	}
	else
	{
		WINLIST* curhead;
		WINLIST* second;
		//already hidden
		if( pCurWin != pWin )
			return TRUE;

		second = pheadwinlist->next;
		if(second==NULL)
			return FALSE;
		curhead = pheadwinlist;
		//swap first with second
		curhead->next =second->next;
		curhead->prev = second;
		second->prev = NULL;
		second->next=curhead;

		pheadwinlist = second;
		pCurWin = pheadwinlist->pWin;
		egl_window_invalidate();
		window_draw( pCurWin );
		return TRUE;
	}
}

static BOOL wm_insert_new_window( EGL_WINDOW_PTR pWin )
{
	//for the first
	if( pheadwinlist == 0 )
	{
		pheadwinlist = ( WINLIST* )malloc( sizeof( WINLIST ) );
		if( pheadwinlist == NULL )
			return FALSE;
		pheadwinlist->pWin = pWin;
		pheadwinlist->prev = 0;
		pheadwinlist->next = 0;
	}
	else
	{
		WINLIST* old;
		WINLIST* newlist = ( WINLIST* )malloc( sizeof( WINLIST ) );
		if( newlist == NULL )
			return FALSE;
		old = pheadwinlist;
		newlist->pWin = pWin;
		newlist->prev = NULL;
		newlist->next = old;
		old->prev = newlist;
		pheadwinlist = newlist;
	}
	//pCurWin = pWin;
	return TRUE;

}

/*
WINDOW API
*/


EGL_HANDLE egl_create_window( const char* title )
{
	EGL_WINDOW_PTR pWin = ( EGL_WINDOW_PTR )malloc( sizeof( EGL_WINDOW ) );
	if( pWin == NULL )
		return NULL;
	if( strlen( title ) >= EGL_CONFIG_MAX_TITILE )
		strncpy( pWin->title, title, EGL_CONFIG_MAX_TITILE - 1 );
	else
		strcpy(pWin->title,title);

	pWin->bg = NULL;
	pWin->cb = NULL;
	//get screen size
	pWin->rect.x = 0;
	pWin->rect.y = 0;
	pWin->rect.w = EGL_GET_TARGET_SURFACE()->w;
	pWin->rect.h = EGL_GET_TARGET_SURFACE()->h;
	pWin->redraw_rect = pWin->rect;
	pWin->bNeedRedraw = TRUE;
	memset( pWin->objets, 0, EGL_CONFIG_MAX_OBJECT_IN_WINDOW*sizeof(EGL_OBJECT_PTR) );
	pWin->object_cnt = 0;
	wm_insert_new_window( pWin );
	return ( EGL_HANDLE )pWin;
}

BOOL egl_window_show( EGL_HANDLE hWin, BOOL bShow )
{
	EGL_WINDOW_PTR pWin = ( EGL_WINDOW_PTR )hWin;
	if( pWin == NULL )
		return FALSE;
	return wm_show_window( pWin, bShow );
}
BOOL egl_window_isshow( EGL_HANDLE hWin)
{
	EGL_WINDOW_PTR pWin = ( EGL_WINDOW_PTR )hWin;
	if(pCurWin==pWin)
		return TRUE;
	else 
		return FALSE;
}

void egl_window_invalidate()
{
	int i;
	pCurWin->bNeedRedraw = TRUE;
	pCurWin->redraw_rect.w = -1;
	pCurWin->redraw_rect.h = -1;
	for( i = 0; i < pCurWin->object_cnt; i++ )
	{
		EGL_OBJECT_PTR pObj = pCurWin->objets[i];
		pObj->bNeedRedraw = TRUE;
	}
}

void egl_window_invalidate_rect( EGL_RECT* pRect )
{
	int i;
	if(pCurWin==NULL)
		return;
	pCurWin->bNeedRedraw = TRUE;
	pCurWin->redraw_rect = *pRect;
	for( i = 0; i < pCurWin->object_cnt; i++ )
	{
		EGL_OBJECT_PTR pObj = pCurWin->objets[i];
		if( prvCollision_detect_rect( &pObj->rect, pRect ) )
			pObj->bNeedRedraw = TRUE;
	}
}
void egl_window_redraw_rect( EGL_RECT* pRect )
{
	int i;
	EGL_MSG msg;
	msg.hWin = ( EGL_HANDLE )pCurWin;
	msg.msgID = EGL_MSG_DRAW;
	window_draw_bg(pCurWin,pRect);
	for( i = 0; i < pCurWin->object_cnt; i++ )
	{
		EGL_OBJECT_PTR pObj = pCurWin->objets[i];
		if(pObj->bVisible &&  prvCollision_detect_rect( &pObj->rect, pRect ))
		{
			msg.hObj = (EGL_HANDLE)pObj;
			pObj->msg_hanlder(&msg);
		}
	}
}

BOOL egl_window_set_bg( EGL_HANDLE hWin, SURFACE* Img )
{
	EGL_WINDOW_PTR pWin = ( EGL_WINDOW_PTR )hWin;
	pWin->bg = Img;
	return TRUE;
}

SURFACE* egl_window_get_bg( EGL_HANDLE hWin )
{
	EGL_WINDOW_PTR pWin = ( EGL_WINDOW_PTR )hWin;
	return pWin->bg;
}

BOOL egl_window_add_object( EGL_HANDLE hWin, EGL_HANDLE hObj )
{
	EGL_OBJECT_PTR pObj ;
	EGL_WINDOW_PTR pWin ;
	pObj = ( EGL_OBJECT_PTR )hObj;
	pWin = ( EGL_WINDOW_PTR )hWin;
	CRITICAL_ENTER();
	if(pWin->object_cnt<EGL_CONFIG_MAX_OBJECT_IN_WINDOW)
	{
		pWin->objets[pWin->object_cnt] = pObj;
		pWin->object_cnt++;
		pObj->parentWin = pWin;
		CRITICAL_EXIT();
		return TRUE;
	}
	CRITICAL_EXIT();
	return FALSE;
}

void egl_window_set_callback( EGL_HANDLE hWin, EVENT_CALLBACK cb )
{
	EGL_WINDOW* pWin = ( EGL_WINDOW* )hWin;
	CRITICAL_ENTER();
	pWin->cb = cb;
	CRITICAL_EXIT();
}

void egl_user_key_input( BOOL bKeyDown, U32 val )
{
	EGL_WINDOW_PTR pWin = get_active_window();
	if( pWin )
	{
		EGL_MSG msg;
		msg.hWin = ( EGL_HANDLE ) pWin;
		if( bKeyDown )
			msg.msgID = EGL_MSG_KEY_DOWN;
		else
			msg.msgID = EGL_MSG_KEY_UP;
		msg.param.key = val;
		window_msg_handler( msg.hWin , &msg );
	}
}
/*
form User Device to EGL
*/
void egl_user_touch_input( BOOL bPressed, EGL_POINT* pt )
{
	if( get_active_window() )
	{
		EGL_MSG msg;
		msg.hWin = ( EGL_HANDLE ) get_active_window();
		if( bPressed )
			msg.msgID = EGL_MSG_TOUCHED;
		else
			msg.msgID = EGL_MSG_UNTOUCHED;
		msg.param.point = *pt;
		window_msg_handler( msg.hWin, &msg );
	}
}

void egl_draw()
{
	EGL_WINDOW_PTR pWin = get_active_window();
	window_draw( pWin );
}

EGL_HANDLE egl_window_get_active()
{
	return ( EGL_HANDLE )get_active_window();
}
