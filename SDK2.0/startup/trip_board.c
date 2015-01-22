/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/


#include "adstar.h"


void boardinit()
{
	*R_PAF0 = (3<<14)|(1<<12)|(1<<10)|(1<<8)|(0xff);//SPI Boot(Internal), P0.0 ~ P0.3, P0.7
	*R_PAF1 = ~0xf; //uart0,P1.x
	*R_PAF2 = 0xffff; //P2
	*R_PAF3 = 0xffff; //P3
	*R_PAF4 = 0xffff; //P4
	*R_PAF5 = 0xffff; //P5
	*R_PAF6 = 0x15|(0x3ff<<6); //P6+JTAG
	*R_PAF7 = 0x5|(0xfff<<4); //P7+JTAG
	*R_PAF8 = 0xffff;  //P8
}

