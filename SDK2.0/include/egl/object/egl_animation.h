/******************************************************************************
 * Copyright (C) 2013      Advanced Digital Chips Inc. 
 *						http://www.adc.co.kr
 * Author : Software Team.
 ******************************************************************************/
#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C" {
#endif
	typedef enum
	{
		ANI_CLICKED=0,
	}ANIMATION_EVENT;

	typedef struct _tag_animation_data
	{
		SURFACE** surflist;
		int surfcnt;
		int curnum;
		int delaycnt;
		int cnt;
	} ANIMATION_DATA;

	EGL_HANDLE egl_create_animation(int x, int y, int h, int w,SURFACE** surflist,int surfcnt,int delaycnt);

#pragma once
#if	defined(__cplusplus)||defined(__GNUG__)// The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
}
#endif
