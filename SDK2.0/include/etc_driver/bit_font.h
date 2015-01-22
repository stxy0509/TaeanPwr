/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

*****************************************************************************/

#pragma once
void bf_setsize(int w, int h,int kor_w, int kor_h);
int bf_drawstring(int x, int y, char* str);
int bf_drawstring_vright(int x, int y, char* str);
int bf_drawstring_vleft(int x, int y, char* str);
bool bf_init();
void bf_setrgb(U8 r, U8 g, U8 b);
SURFACE* bf_makesurface(char* str);


