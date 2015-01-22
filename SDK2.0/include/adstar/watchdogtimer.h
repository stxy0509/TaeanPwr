/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once

//======================================================================================================================
// Watchdog Timer Contorl Register (WDTCTRL)

#define R_WDTCTRL  ((volatile unsigned int*)0x80020000)

#define F_WDTCTRL_WDTST			(1 << 4)		// Watchdog timer status bit - 0 No Watchdog / 1 Watchdog reset or interrupt
#define F_WDTCTRL_WDTMOD_RST	(0 << 1)		// Watchdog timer mode select bit - 0 Reset Mode / 1 Interrupt Mode
#define F_WDTCTRL_WDTMOD_INT	(1 << 1)		// Watchdog timer mode select bit - 0 Reset Mode / 1 Interrupt Mode
#define F_WDTCTRL_WDTEN_DISABLE	(0 << 0)		// Watchdog timer enable bit - 0 Disable / 1 Enable
#define F_WDTCTRL_WDTEN_ENABLE	(1 << 0)		// Watchdog timer enable bit - 0 Disable / 1 Enable
//======================================================================================================================

//======================================================================================================================
// Watchdog Timer Counter Value Register (WDTCNT)

#define R_WDTCNT  ((volatile unsigned int*)0x80020004)
//======================================================================================================================


