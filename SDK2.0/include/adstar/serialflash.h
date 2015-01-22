
/******************************************************************************
 Copyright (C) 2012      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

*****************************************************************************/
#pragma once

#define R_FLMOD	((volatile unsigned int*)0x80000000)
#define R_FLBRT	((volatile unsigned int*)0x80000004)
#define R_FLCSH	((volatile unsigned int*)0x80000008)
#define R_FLPEM	((volatile unsigned int*)0x8000000c)
#define R_FLCMD	((volatile unsigned char*)0x80000010)
#define R_FLSTS	((volatile unsigned char*)0x80000014)
#define R_FLSEA	((volatile unsigned int*)0x80000018)
#define R_FLBEA	((volatile unsigned int*)0x8000001c)
#define R_FLDAT	((volatile unsigned int*)0x80000020)
#define R_FLWCP	((volatile unsigned int*)0x80000024)

void flash_erase_sector(int sector,int len);
void flash_write(U32 addr, BYTE* buf, int len);
void flash_read(U32 addr, BYTE* buf, int len);
int flash_get_sectorsize();
int flash_get_size();
