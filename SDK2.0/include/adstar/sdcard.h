/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#pragma once 

#define SDHC_CTRL_BASE 0xA0001000

#define R_SDCCON		((volatile unsigned int*)(SDHC_CTRL_BASE+0x00))
#define R_SDCSTAT		((volatile unsigned int*)(SDHC_CTRL_BASE+0x04))
#define R_SDCCD			((volatile unsigned int*)(SDHC_CTRL_BASE+0x08))
#define R_SDCRTO		((volatile unsigned int*)(SDHC_CTRL_BASE+0x0c))
#define R_SDCDTO		((volatile unsigned int*)(SDHC_CTRL_BASE+0x10))
#define R_SDCBL			((volatile unsigned int*)(SDHC_CTRL_BASE+0x14))
#define R_SDCNOB		((volatile unsigned int*)(SDHC_CTRL_BASE+0x18))
#define R_SDCIE			((volatile unsigned int*)(SDHC_CTRL_BASE+0x1c))
#define R_SDCCMDCON		((volatile unsigned int*)(SDHC_CTRL_BASE+0x20))
#define R_SDCCMDA		((volatile unsigned int*)(SDHC_CTRL_BASE+0x24))
#define R_SDCRFA		((volatile unsigned int*)(SDHC_CTRL_BASE+0x28))
#define R_SDCDFA		((volatile unsigned int*)(SDHC_CTRL_BASE+0x2c))
#define R_SDCNWR		((volatile unsigned int*)(SDHC_CTRL_BASE+0x30)) // interval cmd,data

BOOL sdhc_init();
U32 sdhc_getsectorsize();
U32 sdhc_getsectorcount();
U32 sdc_memsize(); //return MegaByte Size
BOOL sdhc_read_sect(void* buf,U32 sector,U32 sectcnt);//read sectors(512byte)
BOOL sdhc_write_sect(void* buf,U32 sector,U32 sectcnt);//write sectors(512byte)
