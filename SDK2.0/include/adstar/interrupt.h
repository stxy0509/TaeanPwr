/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#pragma once

#define CRITICAL_ENTER()	asm("set 8");asm("sync") // back up interrupt status
#define CRITICAL_EXIT()		asm("clr 8");asm("sync") // restor interrupt status
#define INTERRUPT_ENABLE	asm("set 13");
#define INTERRUPT_DISABLE	asm("clr 13");
#define NMI_ENABLE			asm("set 14");
#define NMI_DISABLE			asm("clr 14");
#define SETVECTORED			asm("set 12");

// Interrupt Pending Clear Register
#define R_INTPENDCLR		((volatile unsigned int*)0xFFFF0000)

// External Interrupt Mode and External PIN Level Register
#define R_EINTMOD		((volatile unsigned int*)0xFFFF0004)

#define F_EINTMOD_1ST			(1<<7)
#define F_EINTMOD_1MOD_LOW		(0<<4)
#define F_EINTMOD_1MOD_HIGH		(1<<4)
#define F_EINTMOD_1MOD_FEDGE	(2<<4)
#define F_EINTMOD_1MOD_REDGE	(3<<4)
#define F_EINTMOD_1MOD_AEDGE	(4<<4)

#define F_EINTMOD_0ST			(1<<3)
#define F_EINTMOD_0MOD_LOW		(0<<0)
#define F_EINTMOD_0MOD_HIGH		(1<<0)
#define F_EINTMOD_0MOD_FEDGE	(2<<0)
#define F_EINTMOD_0MOD_REDGE	(3<<0)
#define F_EINTMOD_0MOD_AEDGE	(4<<0)

// Internal Interrupt Mode Register
#define R_IINTMOD(n)	((volatile unsigned int*)(0xFFFF0008 + ((n)*0x40)))

// Interrupt Pending Register
#define R_INTPEND(n)	((volatile unsigned int*)(0xFFFF000C + ((n)*0x40)))

// Interrupt Enable Register
#define R_INTEN(n)		((volatile unsigned int*)(0xFFFF0010 + ((n)*0x40)))

// Interrupt Mask Set Register
#define R_INTMASKSET(n)	((volatile unsigned int*)(0xFFFF0014 + ((n)*0x40)))

// Interrupt Mask Status Register
#define R_INTMASK(n)	((volatile unsigned int*)(0xFFFF0014 + ((n)*0x40)))

// Interrupt Mask Clear Register
// R_INTMASKCLR
#define R_INTMASKCLR(n)	((volatile unsigned int*)(0xFFFF0018 + ((n)*0x40)))

// Programmable Interrupt Priority Enable Register
#define R_PIPENR	((volatile unsigned int*)(0xFFFF001C)

#define F_PIPENR_ENABLE			(1<<0)			// Programmable Priority Enable
#define F_PIPENR_DISABLE			(0<<0)			// Programmable Priority Disable

// Interrupt Priority Vector Register
#define R_IPVR0	((volatile unsigned int*)(0xFFFF0020)
#define R_IPVR1	((volatile unsigned int*)(0xFFFF0024)
#define R_IPVR2	((volatile unsigned int*)(0xFFFF0028)
#define R_IPVR3	((volatile unsigned int*)(0xFFFF002C)
#define R_IPVR4	((volatile unsigned int*)(0xFFFF0030)
#define R_IPVR5	((volatile unsigned int*)(0xFFFF0034)
#define R_IPVR6	((volatile unsigned int*)(0xFFFF0038)
#define R_IPVR7	((volatile unsigned int*)(0xFFFF003C)

typedef enum 
{
	INTNUM_EIRQ0=0,
	INTNUM_TIMER0,
	INTNUM_SOUNDMIXER,
	INTNUM_DMA0,

	INTNUM_FRAMESYNC,
	INTNUM_GPIOA,
	INTNUM_UART0,
	INTNUM_DMA1,
	
	INTNUM_EIRQ1, //8
	INTNUM_TIMER1,
	INTNUM_PMU,
	INTNUM_DMA2,
	
	INTNUM_SPI0,
	INTNUM_GPIOB,
	INTNUM_UART1,
	INTNUM_DMA3,
	
	INTNUM_RESV1, //0x10
	INTNUM_TIMER2,
	INTNUM_USB,
	INTNUM_DMA4,
	
	INTNUM_USBHOST,
	INTNUM_GPIOC,
	INTNUM_UART2,
	INTNUM_DMA5,
	
	INTNUM_RESV2,
	INTNUM_TIMER3,
	INTNUM_NAND,
	INTNUM_DMA6,
	
	INTNUM_SDHC,
	INTNUM_GPIOD,
	INTNUM_UART3,
	INTNUM_DMA7,
	
	INTNUM_RESV3, //0x20
	INTNUM_GPIOE,
	INTNUM_ADC,
	INTNUM_GPIOF,
	
	INTNUM_WATCHDOG,
	INTNUM_GPIOG,
	INTNUM_UART4,
	INTNUM_GPIOH,
	
	INTNUM_RESV4,
	INTNUM_GPIOI,
	INTNUM_TWI,
	INTNUM_GPIOJ,
	
	INTNUM_SPI1,
	INTNUM_CAPOVERFLOW,
	INTNUM_MOTORFAULTA,
	INTNUM_MOTORFAULTB,

	INTNUM_RESV5,//0x30
	INTNUM_MOTORQEI,
	INTNUM_PWM,
	INTNUM_RESV6,
	
	INTNUM_MAX
}INTERRUPT_TYPE;

void init_interrupt();
BOOL set_interrupt(INTERRUPT_TYPE intnum,void (*fp)());
void* get_interrupt(INTERRUPT_TYPE intnum);
void enable_interrupt(INTERRUPT_TYPE num,BOOL b);

