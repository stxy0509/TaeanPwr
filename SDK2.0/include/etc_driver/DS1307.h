#pragma once

#include "adstar.h"

// DS1307.h

#define DS1307_DEVICE_ADDR		(0x68<<1)
#define DS1307_ADDR_SEC			0x0
#define DS1307_ADDR_MIN			0x1
#define DS1307_ADDR_HOUR		0x2
#define DS1307_ADDR_DAY			0x3
#define DS1307_ADDR_DATE		0x4
#define DS1307_ADDR_MONTH		0x5
#define DS1307_ADDR_YEAR		0x6
#define DS1307_ADDR_CONT		0x7

//#define DS1307_SET_HEX

typedef struct
{
	U8 sec;
	U8 min;
	U8 hour;
	U8 day;
	U8 date;
	U8 month;
	U8 year;
}DS1307_Time;

S32 DS1307_Write(U8 addr,U8 *buffer, int length);
S32 DS1307_Read(U8 addr,U8 *buffer, int length);

S32 DS1307_init();
S32 DS1307_SetTime(DS1307_Time* pDT);
S32 DS1307_GetTime(DS1307_Time* pDT);
