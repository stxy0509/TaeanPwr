/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc.
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/


#include "adstar.h"


void boardinit()
{
	// ---0---
	// Alternate Functions
	//*R_PAF0 = 0x55ff;	//P0 0101 0101 1111 1111 - Test, Flash-boot, P03~P01
	*R_PAF0 = 0x95ff;	//P0 1001 0101 1111 1111 - RX4, Flash-boot, P03~P01





	// ---1---
	// Alternate Functions
	*R_PAF1 = 0xfd70;	//P1 1111 1101 0111 0000 - P17~P15, SDHC-CLK/CMD, P12, RX0/TX0

	// Pin / Port mode
	*R_GPIDIR(1) |= (1<<8);		// pin_mode

	// direction
	// nRESET (P1.6) : GPIO, output, PinMode
	*R_GPODIR(1) |= (1<<5);
	*R_GP1OHIGH |= (1<<5);		// init Level = H (reset ??태)

	*R_GPODIR(1) |= (1<<6);
	*R_GP1OHIGH |= (1<<6);		// init Level = H (reset ??태)

	*R_GPIDIR(1) |= (1<<7);




	// ---2---
	// Alternate Functions
	//*R_PAF2 = 0x55f5; 	//P2 0101 0101 1111 0101 - SDHC-D3~D0, P23~P20
	*R_PAF2 = 0x55d5; 	//P2 0101 0101 1101 0101 - SDHC-D3~D0, P23, TX4, RX3, TX3


	// ---3---
	// Alternate Functions
	*R_PAF3 = 0xffff; 	//P3 1111 1111 1111 1111 - P37~P30

	// Pin / Port mode
	*R_GPODIR(3) |= (1<<8);	// port mode

	// direction
	// Address(P3) : GPIO, Output, PortMode
	*R_GPODIR(3) |= 0xFF;
	// init Level
	*R_GPDOUT(3) = 0xFF;



	// ---4---
	// Alternate Functions
	*R_PAF4 = 0x55Af; 	//P4 0101 0101 1010 1111 - SPI-CS1/SCK1, TWI-SDA/SCL, RX1/TX1, P41, P40
	//*R_PAF4 = 0x55ff; 	//P4 0101 0101 1010 1111 - SPI-CS1/SCK1, TWI-SDA/SCL, RX1/TX1, P41, P40

	// Pin / Port mode
	*R_GPIDIR(4) |= (1<<8);	// pin mode

	// direction
	//P4.1 : heartbeat (output / pin-mode)
	*R_GPODIR(4) |= (1<<1);

#if 1 	// for test
	//P4.0 : for debug only (output / pin-mode)
	*R_GPODIR(4) |= (1<<0);
#endif


	// ---5---
	// Alternate Functions
	*R_PAF5 = 0xf5f5; 	//P5 1111 0101 1111 0101 - P57~P56, EIRQ1, EIRQ0, P53~P52, SPI-MOSI1/MISO1

	// Pin / Port mode
	*R_GPIDIR(5) = (1<<8);	// pin mode

	// direction
	// nIOR (P5.7), nIOW (P5.6), nCS (P5.3) : GPIO, output, PinMode
	*R_GPODIR(5)  |= (1<<7) | (1<<6) | (1<<3) | (1<<2);
	*R_GPOHIGH(5) |= (1<<7) | (1<<6) | (1<<3) | (1<<2);

	// INT1(P5.5), INT2(P5.4) : GPIO, input, PinMode, EINT
	*R_GPIDIR(5) |= (1<<5) | (1<<4);
	*R_EINTMOD = 0x33;		// Rising Edge Detect Register Set


	// ---6---
	// Alternate Functions
	*R_PAF6 = 0xafd5; 	//P6 1010 1111 1101 0101 - P67~P63, JTAG
	// Pin / Port mode
	*R_GPIDIR(6) = (1<<8);	// pin mode
// direction
	// Data (P6.4), CLK (P6.3) : GPIO, output, PinMode
	*R_GPODIR(6)  |= (1<<4) | (1<<3);
	*R_GPOHIGH(6) |= (1<<4) | (1<<3);


	// ---7---
	// Alternate Functions
	*R_PAF7 = 0x5575; 	//P7 0101 0101 0111 0101 - CFG4~0, P72, JTAG


	// ---8---
	// Alternate Functions
	*R_PAF8 = 0xffff;  	//P8 1111 1111 1111 1111 - P87~P80

	// Pin / Port mode
	*R_GPODIR(8) = 0x100;	// port mode

	// direction
	*R_GPODIR(8) |= 0xFF;	// SB16C1058 address
	// init Level
	*R_GPDOUT(8) = 0xFF;



	//--- UART ---
	uart_config(0,115200,DATABITS_8,STOPBITS_1,UART_PARNONE);	// debug
	uart_config(1,  9600,DATABITS_8,STOPBITS_1,UART_PARNONE);	// sysMCU
	uart_config(2,  9600,DATABITS_8,STOPBITS_1,UART_PARNONE);	// iridium1
	uart_config(3,  9600,DATABITS_8,STOPBITS_1,UART_PARNONE);	// iridium2

	uart_config(4,460800,DATABITS_8,STOPBITS_1,UART_PARNONE);	// iridium2
	// uart_config(4,230400,DATABITS_8,STOPBITS_1,UART_PARNONE);	// iridium2
	// uart_config(4,115200,DATABITS_8,STOPBITS_1,UART_PARNONE);	// iridium2

}




