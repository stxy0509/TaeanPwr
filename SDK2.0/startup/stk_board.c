/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#include "adstar.h"
#include "stk_board.h"


void lcdon()
{
	*(volatile U32*)0x80022c00 |= 1<<6;    //mc_pwm all counter reset
	*(volatile U32*)0x80022c00 &= ~(1<<6); 

	*(volatile U32*)0x80022c08 = 0x700;    //pwm period 
	*(volatile U32*)0x80022c34 = 0x350;    //pwm duty 2
	*(volatile U32*)0x80022c10 |= 1<<10;    //pwm independent output 2 ch
	*(volatile U32*)0x80022c00 = 1<<7|0x7;   //pwm enable
	
	//backlight on
	*R_GP3ODIR = 1<<1;
	*R_GP3OLOW= 1<<1;
	//PCI .. see the LCD Spec
	*R_GP3ODIR = 1<<4;
	*R_GP3OLOW= 1<<4;
	delayms(10);
	*R_GP3OHIGH = 1<<4;
}

void boardinit()
{
#ifdef STK_20   // Use STK Board Version 2.0
	*R_PAF0 = (3<<14)|(1<<12)|(1<<10)|(1<<8)|(0xf);//SPI Boot(Internal), P0.7, P0.0, P0.1(mode switch), SND_PWM2R(p,n) //june
#else
	*R_PAF0 = (3<<14)|(1<<12)|(1<<10)|(1<<8)|(0xf<<4);//SPI Boot(Internal), P0.7, P0.2, P0.3(mode switch), SND_PWM2L(p,n) //june
#endif
	*R_PAF1 = 0; //uart0, nand
	*R_PAF2 = 0; //nand
	*R_PAF3 = (0xf<<12)|(1<<10)|(3<<8)|(1<<7)|(1<<5)|(0xd); //P3.0, P3.1, UART3, P3.4, MPWML2, P3.6, P3.7
	*R_PAF4 = (0xf<<12)|(1<<10)|(1<<8)|(2<<6)|(2<<4)|(2<<2)|(2<<0);//EXIRQ(1,2), UART1, TWI, PIO4.6, PIO4.7
	*R_PAF5 = 0x3f; //P5.(0-2), VSYNC,HSYNC,DISP_EN,CRTC_CLK_OUT
	*R_PAF6 = 0x15; //R+JTAG
	*R_PAF7 = 0x5; //G+JTAG
	*R_PAF8 = 0;  //B
	*R_PAF9 = 0x15;  //serial flash
}

