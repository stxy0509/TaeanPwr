/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/

#include "egl_typedef.h"
#include "egl_base.h"
#include "egl_private.h"
#include "object/egl_animation.h"
#include <stdlib.h>


static void mydraw(EGL_OBJECT_PTR pObj)
{
	ANIMATION_DATA* pAni = (ANIMATION_DATA*)(pObj->selfdata);
	if(pAni->cnt==0)
	{
		DRAW_SURFACE(pAni->surflist[pAni->curnum],pObj->rect.x,pObj->rect.y);
		pAni->curnum++;
		if(pAni->curnum>9)
			pAni->curnum=0;
		pObj->bNeedRedraw=TRUE;//for next frame drawing
	}
	if(pAni->delaycnt)
	{
		pAni->cnt++;
		if(pAni->cnt==pAni->delaycnt)
			pAni->cnt=0;
	}
}
static void* msg_handler(EGL_MSG* pMsg)
{
	EGL_OBJECT_PTR pObj = EGL_HANDLE_TO_OBJECT(pMsg->hObj);
	switch(pMsg->msgID)
	{
	case EGL_MSG_DRAW:
		mydraw(pObj);
		break;
	case EGL_MSG_UNTOUCHED:
		if(pObj->cb)
			pObj->cb(pMsg->hObj,(int)ANI_CLICKED);
		break;
	default:
		break;
	}
	return NULL;
}

EGL_HANDLE egl_create_animation(int x, int y, int h, int w,SURFACE** surflist,int surfcnt,int delaycnt)
{
	ANIMATION_DATA* ani_data ;
	EGL_OBJECT_PTR pObj= prv_egl_create_object(x,y,w,h);
	int i;
	if(pObj==NULL)
		return NULL;
	if(surfcnt==0)
		return NULL;
	if(surflist==NULL)
		return NULL;

	ani_data = (ANIMATION_DATA*)malloc(sizeof(ANIMATION_DATA));
	if(ani_data==NULL)
	{
		prv_egl_delete_object(pObj);
		return NULL;
	}
	ani_data->surflist = (SURFACE**)malloc(sizeof(SURFACE*)*surfcnt);
	if(ani_data->surflist ==NULL)
	{
		prv_egl_delete_object(pObj);
		free(ani_data);
		return NULL;
	}
	ani_data->cnt=0;
	for(i=0;i<surfcnt;i++)
		ani_data->surflist[i]=surflist[i];

	ani_data->surfcnt=surfcnt;
	ani_data->curnum=0;
	ani_data->delaycnt=delaycnt;
	pObj->selfdata = ani_data;
	pObj->msg_hanlder = msg_handler;
	return (EGL_HANDLE)pObj;
}
