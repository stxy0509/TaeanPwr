/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"
SURFACE* loadimage(char* fname)
{
	int len = strlen(fname);
#if CONFIG_SUPPORT_BMP != 0
	if(strcmp(".bmp",fname+len-4)==0)
	{
		return loadbmp(fname);
	}
#endif
#if CONFIG_SUPPORT_JPG != 0
	if(strcmp(".jpg",fname+len-4)==0)
	{
		return loadjpg(fname);
	}
#endif
#if CONFIG_SUPPORT_TGA != 0
	if(strcmp(".tga",fname+len-4)==0)
	{
		return loadtga(fname);
	}
#endif
#if CONFIG_SUPPORT_PNG != 0
	if(strcmp(".png",fname+len-4)==0)
	{
		return loadpng(fname);
	}
#endif
	return 0;
}
