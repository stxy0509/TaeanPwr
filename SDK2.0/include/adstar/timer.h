/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#pragma once

#define TIMER_BASE_ADDR	0x80020400


//======================================================================================================================
// Timer Pre-scale Control Registers (TPxCTRL)

#define R_TP0CTRL	((volatile unsigned int*)(0x80020400))
#define R_TP1CTRL	((volatile unsigned int*)(0x80020420))
#define R_TP2CTRL	((volatile unsigned int*)(0x80020440))
#define R_TP3CTRL	((volatile unsigned int*)(0x80020460))

#define F_TPCTRL_CNTCLR			(1 << 1)		// Pre-scale Counter and Timer Counter Reset
#define F_TPCTRL_CLKSEL_CAP0	(1 << 0)		// Pre-scale Clock Selection - CAP0
#define F_TPCTRL_CLKSEL_SCLK	(0 << 0)		// Pre-scale Clock Selection - System Clock
//======================================================================================================================

//======================================================================================================================
// Timer Control Registers (TMxCTRL)

#define R_TM0CTRL	((volatile unsigned int*)(0x80020404))
#define R_TM1CTRL	((volatile unsigned int*)(0x80020424))
#define R_TM2CTRL	((volatile unsigned int*)(0x80020444))
#define R_TM3CTRL	((volatile unsigned int*)(0x80020464))

#define F_TMCTRL_TMOD_TIMER		(0 <<14)		// Timer/Counter Mode - TIMER
#define F_TMCTRL_TMOD_PWM		(1 <<14)		// Timer/Counter Mode - PWM
#define F_TMCTRL_TMOD_CAP		(2 <<14)		// Timer/Counter Mode - Capture
#define F_TMCTRL_OVST			(1 <<12)		// Capture Overflow Status bit
#define F_TMCTRL_CAPMOD_LH		(0 << 8)		// Capture Mode Selection - Low/High Pulse
#define F_TMCTRL_CAPMOD_LP		(2 << 8)		// Capture Mode Selection - Low Pulse
#define F_TMCTRL_CAPMOD_HP		(3 << 8)		// Capture Mode Selection - High Pulse
#define F_TMCTRL_CAPMOD_FF		(4 << 8)		// Capture Mode Selection - Falling to Falling Period
#define F_TMCTRL_CAPMOD_RR		(6 << 8)		// Capture Mode Selection - Rising to Rising Period
#define F_TMCTRL_PWMO			(1 << 6)		// PWM Output One Period Generation - Enable
#define F_TMCTRL_PWML_HIGH		(1 << 5)		// PWM Output Start Level - High
#define F_TMCTRL_TMOUT			(1 << 4)		// Timer Wave Output Generation - Enable
#define F_TMCTRL_PFSEL_2		(0 << 1)		// Pre-scale Factor Selection - 1/2
#define F_TMCTRL_PFSEL_8		(1 << 1)		// Pre-scale Factor Selection - 1/8
#define F_TMCTRL_PFSEL_32		(2 << 1)		// Pre-scale Factor Selection - 1/32
#define F_TMCTRL_PFSEL_128		(3 << 1)		// Pre-scale Factor Selection - 1/128
#define F_TMCTRL_PFSEL_512		(4 << 1)		// Pre-scale Factor Selection - 1/512
#define F_TMCTRL_PFSEL_2048		(5 << 1)		// Pre-scale Factor Selection - 1/2048
#define F_TMCTRL_PFSEL_8192		(6 << 1)		// Pre-scale Factor Selection - 1/8192
#define F_TMCTRL_PFSEL_32768	(7 << 1)		// Pre-scale Factor Selection - 1/32768
#define F_TMCTRL_TMEN			(1 << 0)		// Timer/Counter or PWM Enable
//======================================================================================================================

//======================================================================================================================
// Timer Counter / PWM Period Registers (TMxCNT)

#define R_TM0CNT	((volatile unsigned int*)(0x80020408))
#define R_TM1CNT	((volatile unsigned int*)(0x80020428))
#define R_TM2CNT	((volatile unsigned int*)(0x80020448))
#define R_TM3CNT	((volatile unsigned int*)(0x80020468))
//======================================================================================================================

//======================================================================================================================
// Capture Counter / PWM Duty  Registers (TMxDUT)

#define R_TM0DUT	((volatile unsigned int*)(0x8002040C))
#define R_TM1DUT	((volatile unsigned int*)(0x8002042C))
#define R_TM2DUT	((volatile unsigned int*)(0x8002044C))
#define R_TM3DUT	((volatile unsigned int*)(0x8002046C))
//======================================================================================================================

//======================================================================================================================
// PWM Pulse Count Registers (TMxPUL)

#define R_TM0PUL	((volatile unsigned int*)(0x80020410))
#define R_TM1PUL	((volatile unsigned int*)(0x80020430))
#define R_TM2PUL	((volatile unsigned int*)(0x80020450))
#define R_TM3PUL	((volatile unsigned int*)(0x80020470))
//======================================================================================================================


#define TPCTRL_ADDR(ch) (TIMER_BASE_ADDR+0x20*ch)
#define TMCTRL_ADDR(ch) (TIMER_BASE_ADDR+4+0x20*ch)
#define TMCNT_ADDR(ch) (TIMER_BASE_ADDR+8+0x20*ch)
#define TMDUT_ADDR(ch) (TIMER_BASE_ADDR+0xc+0x20*ch)
#define TMPUL_ADDR(ch) (TIMER_BASE_ADDR+0x10+0x20*ch)



#define MAX_TIMER_CHANNEL 3

BOOL set_timer(int nCh,U32 ms);
BOOL stop_timer(int nCh);
BOOL delayms(U32 ms); //return false, if ms is too big, check the timer.c
BOOL delayus(int us);
