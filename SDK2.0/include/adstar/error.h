/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once
typedef enum
{
	SDKERROR_NOMEM=0,
	SDKERROR_FILEOPEN,
	SDKERROR_FILEREAD,
	SDKERROR_FILEWRITE,
	SDKERROR_FILESEEK,
	SDKERROR_UNSUPPORTED,
	SDKERROR_LAST,
} SDKERROR;


void debugerror(SDKERROR err,void* p1);
