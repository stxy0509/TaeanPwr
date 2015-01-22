/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

 ******************************************************************************/
#pragma once

#define GPIO_BASE	0xffff3000

#define R_GPDIR(ch) ((volatile unsigned int*)(GPIO_BASE+(0x40*(ch))))
#define R_GP0DIR	((volatile unsigned int*)GPIO_BASE)
#define R_GP1DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*1)))
#define R_GP2DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*2)))
#define R_GP3DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*3)))
#define R_GP4DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*4)))
#define R_GP5DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*5)))
#define R_GP6DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*6)))
#define R_GP7DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*7)))
#define R_GP8DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*8)))
#define R_GP9DIR	((volatile unsigned int*)(GPIO_BASE+(0x40*9)))

#define R_GPODIR(ch) ((volatile unsigned int*)(GPIO_BASE+(0x40*(ch))))
#define	R_GP0ODIR R_GP0DIR
#define	R_GP1ODIR R_GP1DIR
#define	R_GP2ODIR R_GP2DIR
#define	R_GP3ODIR R_GP3DIR
#define	R_GP4ODIR R_GP4DIR
#define	R_GP5ODIR R_GP5DIR
#define	R_GP6ODIR R_GP6DIR
#define	R_GP7ODIR R_GP7DIR
#define	R_GP8ODIR R_GP8DIR
#define	R_GP9ODIR R_GP9DIR

// Port Direction Input Mode Setting Registers
#define R_GPIDIR(ch) ((volatile unsigned int*)(GPIO_BASE+4+(0x40*(ch))))
#define R_GP0IDIR	(R_GP0DIR+1)
#define R_GP1IDIR	(R_GP1DIR+1)
#define R_GP2IDIR	(R_GP2DIR+1)
#define R_GP3IDIR	(R_GP3DIR+1)
#define R_GP4IDIR	(R_GP4DIR+1)
#define R_GP5IDIR	(R_GP5DIR+1)
#define R_GP6IDIR	(R_GP6DIR+1)
#define R_GP7IDIR	(R_GP7DIR+1)
#define R_GP8IDIR	(R_GP8DIR+1)
#define R_GP9IDIR	(R_GP9DIR+1)

//Port Output Data Level Registers
#define R_GPOLEV(ch) ((volatile unsigned int*)(GPIO_BASE+8+(0x40*(ch))))
#define R_GP0OLEV	(R_GP0DIR+2)
#define R_GP1OLEV	(R_GP1DIR+2)
#define R_GP2OLEV	(R_GP2DIR+2)
#define R_GP3OLEV	(R_GP3DIR+2)
#define R_GP4OLEV	(R_GP4DIR+2)
#define R_GP5OLEV	(R_GP5DIR+2)
#define R_GP6OLEV	(R_GP6DIR+2)
#define R_GP7OLEV	(R_GP7DIR+2)
#define R_GP8OLEV	(R_GP8DIR+2)
#define R_GP9OLEV	(R_GP9DIR+2)

// Port Output Data Registers
#define R_GPDOUT(ch) ((volatile unsigned char*)(GPIO_BASE+8+(0x40*(ch))))
#define R_GP0DOUT	(R_GP0DIR+2)
#define R_GP1DOUT	(R_GP1DIR+2)
#define R_GP2DOUT	(R_GP2DIR+2)
#define R_GP3DOUT	(R_GP3DIR+2)
#define R_GP4DOUT	(R_GP4DIR+2)
#define R_GP5DOUT	(R_GP5DIR+2)
#define R_GP6DOUT	(R_GP6DIR+2)
#define R_GP7DOUT	(R_GP7DIR+2)
#define R_GP8DOUT	(R_GP8DIR+2)
#define R_GP9DOUT	(R_GP9DIR+2)
 
//Port Output Data High Level Setting Registers
#define R_GPOHIGH(ch) ((volatile unsigned int*)(GPIO_BASE+8+(0x40*(ch))))
#define R_GP0OHIGH R_GP0OLEV	
#define R_GP1OHIGH R_GP1OLEV	
#define R_GP2OHIGH R_GP2OLEV	
#define R_GP3OHIGH R_GP3OLEV 
#define R_GP4OHIGH R_GP4OLEV 
#define R_GP5OHIGH R_GP5OLEV 
#define R_GP6OHIGH R_GP6OLEV 
#define R_GP7OHIGH R_GP7OLEV 
#define R_GP8OHIGH R_GP8OLEV 
#define R_GP9OHIGH R_GP9OLEV 

//Port Output Data Low Level Setting Registers
#define R_GPOLOW(ch) ((volatile unsigned int*)(GPIO_BASE+0xc+(0x40*(ch))))
#define R_GP0OLOW	(R_GP0DIR+3)
#define R_GP1OLOW	(R_GP1DIR+3)
#define R_GP2OLOW	(R_GP2DIR+3)
#define R_GP3OLOW	(R_GP3DIR+3)
#define R_GP4OLOW	(R_GP4DIR+3)
#define R_GP5OLOW	(R_GP5DIR+3)
#define R_GP6OLOW	(R_GP6DIR+3)
#define R_GP7OLOW	(R_GP7DIR+3)
#define R_GP8OLOW	(R_GP8DIR+3)
#define R_GP9OLOW	(R_GP9DIR+3)

//Port Input Data Level Registers
#define R_GPILEV(ch) ((volatile unsigned int*)(GPIO_BASE+0x10+(0x40*(ch))))
#define R_GP0ILEV	(R_GP0DIR+4)
#define R_GP1ILEV	(R_GP1DIR+4)
#define R_GP2ILEV	(R_GP2DIR+4)
#define R_GP3ILEV	(R_GP3DIR+4)
#define R_GP4ILEV	(R_GP4DIR+4)
#define R_GP5ILEV	(R_GP5DIR+4)
#define R_GP6ILEV	(R_GP6DIR+4)
#define R_GP7ILEV	(R_GP7DIR+4)
#define R_GP8ILEV	(R_GP8DIR+4)
#define R_GP9ILEV	(R_GP9DIR+4)

//Port Pull-up Status Registers
#define R_GPPUS(ch) ((volatile unsigned int*)(GPIO_BASE+0x18+(0x40*(ch))))
#define R_GP0PUS		(R_GP0DIR+6)
#define R_GP1PUS		(R_GP1DIR+6)
#define R_GP2PUS		(R_GP2DIR+6)
#define R_GP3PUS		(R_GP3DIR+6)
#define R_GP4PUS		(R_GP4DIR+6)
#define R_GP5PUS		(R_GP5DIR+6)
#define R_GP6PUS		(R_GP6DIR+6)
#define R_GP7PUS		(R_GP7DIR+6)
#define R_GP8PUS		(R_GP8DIR+6)
#define R_GP9PUS		(R_GP9DIR+6)

//Port Pull-up Enable Registers
#define R_GPPUEN(ch) ((volatile unsigned int*)(GPIO_BASE+0x18+(0x40*(ch))))
#define R_GP0PUEN		R_GP0PUS
#define R_GP1PUEN		R_GP1PUS
#define R_GP2PUEN		R_GP2PUS
#define R_GP3PUEN		R_GP3PUS
#define R_GP4PUEN		R_GP4PUS
#define R_GP5PUEN		R_GP5PUS
#define R_GP6PUEN		R_GP6PUS
#define R_GP7PUEN		R_GP7PUS
#define R_GP8PUEN		R_GP8PUS
#define R_GP9PUEN		R_GP9PUS

//Port Pull-up Disable Registers
#define R_GPPUDIS(ch) ((volatile unsigned int*)(GPIO_BASE+0x1c+(0x40*(ch))))
#define R_GP0PUDIS		(R_GP0DIR+7)
#define R_GP1PUDIS		(R_GP1DIR+7)
#define R_GP2PUDIS		(R_GP2DIR+7)
#define R_GP3PUDIS		(R_GP3DIR+7)
#define R_GP4PUDIS		(R_GP4DIR+7)
#define R_GP5PUDIS		(R_GP5DIR+7)
#define R_GP6PUDIS		(R_GP6DIR+7)
#define R_GP7PUDIS		(R_GP7DIR+7)
#define R_GP8PUDIS		(R_GP8DIR+7)
#define R_GP9PUDIS		(R_GP9DIR+7)

//Port Rising-Edge Detect Registers
#define R_GPRED(ch) ((volatile unsigned int*)(GPIO_BASE+0x20+(0x40*(ch))))
#define R_GP0RED		(R_GP0DIR+8)
#define R_GP1RED		(R_GP1DIR+8)
#define R_GP2RED		(R_GP2DIR+8)
#define R_GP3RED		(R_GP3DIR+8)
#define R_GP4RED		(R_GP4DIR+8)
#define R_GP5RED		(R_GP5DIR+8)
#define R_GP6RED		(R_GP6DIR+8)
#define R_GP7RED		(R_GP7DIR+8)
#define R_GP8RED		(R_GP8DIR+8)
#define R_GP9RED		(R_GP9DIR+8)

//Port Falling-Edge Detect Registers
#define R_GPFED(ch) ((volatile unsigned int*)(GPIO_BASE+0x24+(0x40*(ch))))
#define R_GP0FED		(R_GP0DIR+9)
#define R_GP1FED		(R_GP1DIR+9)
#define R_GP2FED		(R_GP2DIR+9)
#define R_GP3FED		(R_GP3DIR+9)
#define R_GP4FED		(R_GP4DIR+9)
#define R_GP5FED		(R_GP5DIR+9)
#define R_GP6FED		(R_GP6DIR+9)
#define R_GP7FED		(R_GP7DIR+9)
#define R_GP8FED		(R_GP8DIR+9)
#define R_GP9FED		(R_GP9DIR+9)

//Port Edge Status Registers
#define R_GPEDS(ch) ((volatile unsigned int*)(GPIO_BASE+0x28+(0x40*(ch))))
#define R_GP0EDS		(R_GP0DIR+10)
#define R_GP1EDS		(R_GP1DIR+10)
#define R_GP2EDS		(R_GP2DIR+10)
#define R_GP3EDS		(R_GP3DIR+10)
#define R_GP4EDS		(R_GP4DIR+10)
#define R_GP5EDS		(R_GP5DIR+10)
#define R_GP6EDS		(R_GP6DIR+10)
#define R_GP7EDS		(R_GP7DIR+10)
#define R_GP8EDS		(R_GP8DIR+10)
#define R_GP9EDS		(R_GP9DIR+10)


#define R_PAF(N) ((volatile unsigned int*)(0x80023400+(N*4)))
//======================================================================================================================
// Port Alternate Function Register 0 (PAF0)

#define R_PAF0 ((volatile unsigned int*)0x80023400)

#define F_PAF0_0_SND2_PWML_P	(0 << 0)
#define F_PAF0_0_SPI_CS0			(1 << 0)
#define F_PAF0_0_TWI_SCL		(2 << 0)
#define F_PAF0_0_GP0			(3 << 0)

#define F_PAF0_1_SND2_PWML_N	(0 << 2)
#define F_PAF0_1_SPI_MISO0		(1 << 2)
#define F_PAF0_1_TWI_SDA		(2 << 2)
#define F_PAF0_1_GP1			(3 << 2)

#define F_PAF0_2_SND2_PWMR_P	(0 << 4)
#define F_PAF0_2_SPI_MOSI0		(1 << 4)
#define F_PAF0_2_SRAM_CS1		(2 << 4)
#define F_PAF0_2_GP2			(3 << 4)

#define F_PAF0_3_SND2_PWMR_N	(0 << 6)
#define F_PAF0_3_SPI_SCK0		(1 << 6)
#define F_PAF0_3_SRAM_CS2		(2 << 6)
#define F_PAF0_3_vGP3			(3 << 6)

#define F_PAF0_4_SND3_PWML_P	(0 << 8)
#define F_PAF0_4_SFLASH_CS		(1 << 8)
#define F_PAF0_4_CAP_IN1		(2 << 8)
#define F_PAF0_4_GP4			(3 << 8)

#define F_PAF0_5_SND3_PWML_N	(0 <<10)
#define F_PAF0_5_SFLASH_DQ1	(1 <<10)
#define F_PAF0_5_TM_OUT1		(2 <<10)
#define F_PAF0_5_GP5			(3 <<10)

#define F_PAF0_6_SND3_PWMR_P	(0 <<12)
#define F_PAF0_6_SFLASH_DQ2	(1 <<12)
#define F_PAF0_6_UART_TX4		(2 <<12)
#define F_PAF0_6_GP6			(3 <<12)

#define F_PAF0_7_SND3_PWMR_N	(0 <<14)
#define F_PAF0_7_TAP_SEL			(1 <<14)
#define F_PAF0_7_UART_RX4		(2 <<14)
#define F_PAF0_7_GP7			(3 <<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 1 (PAF1)

#define R_PAF1 ((volatile unsigned int*)0x80023404)

#define F_PAF1_0_UART_TX0		(0 << 0)
#define F_PAF1_0_SND_MCLK		(1 << 0)
#define F_PAF1_0_TWI_SCL		(2 << 0)
#define F_PAF1_0_GP0			(3 << 0)

#define F_PAF1_1_UART_RX0		(0 << 2)
#define F_PAF1_1_SND0_SDI		(1 << 2)
#define F_PAF1_1_TWI_SDA		(2 << 2)
#define F_PAF1_1_GP1			(3 << 2)

#define F_PAF1_2_NF_CS			(0 << 4)
#define F_PAF1_2_MC_FAULT1		(1 << 4)
#define F_PAF1_2_SRAM_CS1		(2 << 4)
#define F_PAF1_2_GP2			(3 << 4)

#define F_PAF1_3_NF_ALE			(0 << 6)
#define F_PAF1_3_SDHC_CMD		(1 << 6)
#define F_PAF1_3_SRAM_CS3		(2 << 6)
#define F_PAF1_3_GP3			(3 << 6)

#define F_PAF1_4_NF_CLE			(0 << 8)
#define F_PAF1_4_SDHC_CLK		(1 << 8)
#define F_PAF1_4_SRAM_BE1		(2 << 8)
#define F_PAF1_4_GP4			(3 << 8)

#define F_PAF1_5_NF_WE			(0 <<10)
#define F_PAF1_5_SND0_SCLK		(1 <<10)
#define F_PAF1_5_SRAM_A17		(2 <<10)
#define F_PAF1_5_GP5			(3 <<10)

#define F_PAF1_6_NF_RE			(0 <<12)
#define F_PAF1_6_SND0_LRCLK		(1 <<12)
#define F_PAF1_6_SRAM_A18		(2 <<12)
#define F_PAF1_6_GP6			(3 <<12)

#define F_PAF1_7_NF_BUSYX		(0 <<14)
#define F_PAF1_7_SND0_SDO		(1 <<14)
#define F_PAF1_7_SRAM_WAIT		(2 <<14)
#define F_PAF1_7_GP7			(3 <<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 2 (PAF2)

#define R_PAF2 ((volatile unsigned int*)0x80023408)

#define F_PAF2_0_NF_D0			(0 << 0)
#define F_PAF2_0_UART_TX3		(1 << 0)
#define F_PAF2_0_SRAM_AD8		(2 << 0)
#define F_PAF2_0_GP0			(3 << 0)

#define F_PAF2_1_NF_D1			(0 << 2)
#define F_PAF2_1_UART_RX3		(1 << 2)
#define F_PAF2_1_SRAM_AD9		(2 << 2)
#define F_PAF2_1_GP1			(3 << 2)

#define F_PAF2_2_NF_D2			(0 << 4)
#define F_PAF2_2_UART_TX4		(1 << 4)
#define F_PAF2_2_SRAM_AD10		(2 << 4)
#define F_PAF2_2_GP2			(3 << 4)

#define F_PAF2_3_NF_D3			(0 << 6)
#define F_PAF2_3_UART_RX4		(1 << 6)
#define F_PAF2_3_SRAM_AD11		(2 << 6)
#define F_PAF2_3_GP3			(3 << 6)

#define F_PAF2_4_NF_D4			(0 << 8)
#define F_PAF2_4_SDHC_D0		(1 << 8)
#define F_PAF2_4_SRAM_AD12		(2 << 8)
#define F_PAF2_4_GP4			(3 << 8)

#define F_PAF2_5_NF_D5			(0 <<10)
#define F_PAF2_5_SDHC_D1		(1 <<10)
#define F_PAF2_5_SRAM_AD13		(2 <<10)
#define F_PAF2_5_GP5			(3 <<10)

#define F_PAF2_6_NF_D6			(0 <<12)
#define F_PAF2_6_SDHC_D2		(1 <<12)
#define F_PAF2_6_SRAM_AD14		(2 <<12)
#define F_PAF2_6_GP6			(3 <<12)

#define F_PAF2_7_NF_D7			(0 <<14)
#define F_PAF2_7_SDHC_D3		(1 <<14)
#define F_PAF2_7_SRAM_AD15		(2 <<14)
#define F_PAF2_7_GP7			(3 <<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 3 (PAF3)

#define R_PAF3 ((volatile unsigned int*)0x8002340C)

#define F_PAF3_0_SRAM_AD0A8	(0 << 0)
#define F_PAF3_0_MC_PWMH0		(1 << 0)
#define F_PAF3_0_CAP_IN0		(2 << 0)
#define F_PAF3_0_GP0			(3 << 0) 

#define F_PAF3_1_SRAM_AD1A9	(0 << 2)
#define F_PAF3_1_MC_PWML0		(1 << 2)
#define F_PAF3_1_TM_OUT0		(2 << 2)
#define F_PAF3_1_GP1			(3 << 2)

#define F_PAF3_2_SRAM_AD2A10	(0 << 4)
#define F_PAF3_2_MC_PWMH1		(1 << 4)
#define F_PAF3_2_UART_TX3		(2 << 4)
#define F_PAF3_2_GP2			(3 << 4)

#define F_PAF3_3_SRAM_AD3A11	(0 << 6)
#define F_PAF3_3_MC_PWML1		(1 << 6)
#define F_PAF3_3_UART_RX3		(2 << 6)
#define F_PAF3_3_GP3			(3 << 6)

#define F_PAF3_4_SRAM_AD4A12	(0 << 8)
#define F_PAF3_4_MC_PWMH2		(1 << 8)
#define F_PAF3_4_CAP_IN2		(2 << 8)
#define F_PAF3_4_GP4			(3 << 8)

#define F_PAF3_5_SRAM_AD5A13	(0 <<10)
#define F_PAF3_5_MC_PWML2		(1 <<10)
#define F_PAF3_5_TM_OUT2		(2 <<10)
#define F_PAF3_5_GP5			(3 <<10)

#define F_PAF3_6_SRAM_AD6A14	(0 <<12)
#define F_PAF3_6_MC_PWMH3		(1 <<12)
#define F_PAF3_6_OHCI_OC		(2 <<12)		// OHCI_OverCurrent
#define F_PAF3_6_GP6			(3 <<12)

#define F_PAF3_7_SRAM_AD7A15	(0 <<14)
#define F_PAF3_7_MC_PWML3		(1 <<14)
#define F_PAF3_7_OHCI_PP		(2 <<14)		// OHCI_PortPower
#define F_PAF3_7_GP7			(3 <<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 4 (PAF4)

#define R_PAF4 ((volatile unsigned int*)0x80023410)

#define F_PAF4_0_SRAM_A16 		(0 << 0)		// External SRAM Address 16
#define F_PAF4_0_MC_FAULT0		(1 << 0)		
#define F_PAF4_0_EIRQ0			(2 << 0)
#define F_PAF4_0_GP0			(3 << 0)

#define F_PAF4_1_SRAM_ALE0		(0<<2)
#define F_PAF4_1_IND				(1<<2)
#define F_PAF4_1_EIRQ1			(2<<2)
#define F_PAF4_1_GP1			(3<<2)

#define F_PAF4_2_SRAM_ALE1		(0<<4)
#define F_PAF4_2_QEB			(1<<4)
#define F_PAF4_2_UART_TX1		(2<<4)
#define F_PAF4_2_GP2			(3<<4)

#define F_PAF4_3_SRAM_RE	 	(0<<6)
#define F_PAF4_3_QEA			(1<<6)
#define F_PAF4_3_UART_RX1		(2<<6)
#define F_PAF4_3_GP3			(3<<6)

#define F_PAF4_4_SRAM_WE	 	(0<<8)
#define F_PAF4_4_TWI_SCL		(1<<8)
#define F_PAF4_4_UART_TX2		(2<<8)
#define F_PAF4_4_GP4			(3<<8)

#define F_PAF4_5_SRAM_CS0 		(0<<10)
#define F_PAF4_5_TWI_SDA		(1<<10)
#define F_PAF4_5_UART_RX2		(2<<10)
#define F_PAF4_5_GP5			(3<<10)

#define F_PAF4_6_SND_MCLK 		(0<<12)
#define F_PAF4_6_SPI_SCK1		(1<<12)
#define F_PAF4_6_CAP_IN3		(2<<12)
#define F_PAF4_6_GP6			(3<<12)

#define F_PAF4_7_SND0_SDI 		(0<<14)
#define F_PAF4_7_SPI_CS1			(1<<14)
#define F_PAF4_7_TM_OUT3		(2<<14)
#define F_PAF4_7_GP7	 		(3<<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 5 (PAF5)

#define R_PAF5 ((volatile unsigned int*)0x80023414)

#define F_PAF5_0_SND0_SCLK		(0<<0)
#define F_PAF5_0_SPI_MISO1		(1<<0)
#define F_PAF5_0_SRAM_A0		(2<<0)
#define F_PAF5_0_GP0			(3<<0)

#define F_PAF5_1_SND0_LRCLK		(0<<2)
#define F_PAF5_1_SPI_MOSI1		(1<<2)
#define F_PAF5_1_SRAM_A1		(2<<2)
#define F_PAF5_1_GP1			(3<<2)

#define F_PAF5_2_SND0_SDO		(0<<4)
#define F_PAF5_2_UART_TX0		(1<<4)
#define F_PAF5_2_SRAM_A2		(2<<4)
#define F_PAF5_2_GP2			(3<<4)

#define F_PAF5_3_CRTC_CLK_IN	(0<<6)
#define F_PAF5_3_UART_RX0		(1<<6)
#define F_PAF5_3_SRAM_A3		(2<<6)
#define F_PAF5_3_GP3			(3<<6)

#define F_PAF5_4_VSYNC			(0<<8)
#define F_PAF5_4_EIRQ0			(1<<8)
#define F_PAF5_4_SRAM_A4		(2<<8)
#define F_PAF5_4_GP4			(3<<8)

#define F_PAF5_5_HSYNC			(0<<10)
#define F_PAF5_5_EIRQ1			(1<<10)
#define F_PAF5_5_SRAM_A5		(2<<10)
#define F_PAF5_5_GP5			(3<<10)

#define F_PAF5_6_DISP_EN			(0<<12)
#define F_PAF5_6_UART_TX1		(1<<12)
#define F_PAF5_6_SRAM_A6		(2<<12)
#define F_PAF5_6_GP6			(3<<12)

#define F_PAF5_7_CRTC_CLK_OUT	(0<<14)
#define F_PAF5_7_UART_RX1		(1<<14)
#define F_PAF5_7_SRAM_A7		(2<<14)
#define F_PAF5_7_GP7			(3<<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 6 (PAF6)

#define R_PAF6 ((volatile unsigned int*)0x80023418)

#define F_PAF6_0_R0				(0<<0)
#define F_PAF6_0_NTRST			(1<<0)
#define F_PAF6_0_GP0			(3<<0)

#define F_PAF6_1_R1				(0<<2)
#define F_PAF6_1_TCK				(1<<2)
#define F_PAF6_1_DBG_SCK		(2<<2)
#define F_PAF6_1_GP1			(3<<2)

#define F_PAF6_2_R2				(0<<4)
#define F_PAF6_2_TDI				(1<<4)
#define F_PAF6_2_DBG_SDA		(2<<4)
#define F_PAF6_2_GP2			(3<<4)

#define F_PAF6_3_R3				(0<<6)
#define F_PAF6_3_SDHC_CMD		(1<<6)
#define F_PAF6_3_SND1_SCLK		(2<<6)
#define F_PAF6_3_GP3			(3<<6)

#define F_PAF6_4_R4				(0<<8)
#define F_PAF6_4_SDHC_D0		(1<<8)
#define F_PAF6_4_SNDL_LRCLK		(2<<8)
#define F_PAF6_4_GP4			(3<<8)

#define F_PAF6_5_R5				(0<<10)
#define F_PAF6_5_SDHC_D1		(1<<10)
#define F_PAF6_5_SNDL_SDO		(2<<10)
#define F_PAF6_5_GP5			(3<<10)

#define F_PAF6_6_R6				(0<<12)
#define F_PAF6_6_SDHC_D2		(1<<12)
#define F_PAF6_6_UART_TX2		(2<<12)
#define F_PAF6_6_GP6			(3<<12)

#define F_PAF6_7_R7				(0<<14)
#define F_PAF6_7_SDHC_D3		(1<<14)
#define F_PAF6_7_UART_RX2		(2<<14)
#define F_PAF6_7_GP7			(3<<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 7 (PAF7)

#define R_PAF7 ((volatile unsigned int*)0x8002341C)

#define F_PAF7_0_G0				(0<<0)
#define F_PAF7_0_TMS			(1<<0)
#define F_PAF7_0_SRAM_CS1		(2<<0)
#define F_PAF7_0_GP0			(3<<0)

#define F_PAF7_1_G1				(0<<2)
#define F_PAF7_1_TDO			(1<<2)
#define F_PAF7_1_SRAM_CS2		(2<<2)
#define F_PAF7_1_GP1			(3<<2)

#define F_PAF7_2_G2				(0<<4)
#define F_PAF7_2_SDHC_CLK		(1<<4)
#define F_PAF7_2_SRAM_A10		(2<<4)
#define F_PAF7_2_GP2			(3<<4)

#define F_PAF7_3_G3				(0<<6)
#define F_PAF7_3_CFG0			(1<<6)
#define F_PAF7_3_SRAM_A11		(2<<6)
#define F_PAF7_3_GP3			(3<<6)

#define F_PAF7_4_G4				(0<<8)
#define F_PAF7_4_CFG1			(1<<8)
#define F_PAF7_4_SRAM_A12		(2<<8)
#define F_PAF7_4_GP4			(3<<8)

#define F_PAF7_5_G5				(0<<10)
#define F_PAF7_5_CFG2			(1<<10)
#define F_PAF7_5_SRAM_A13		(2<<10)
#define F_PAF7_5_GP5			(3<<10)

#define F_PAF7_6_G6				(0<<12)
#define F_PAF7_6_CFG3			(1<<12)
#define F_PAF7_6_SRAM_A14		(2<<12)
#define F_PAF7_6_GP6			(3<<12)

#define F_PAF7_7_G7				(0<<14)
#define F_PAF7_7_CFG4			(1<<14)
#define F_PAF7_7_SRAM_A15		(2<<14)
#define F_PAF7_7_GP7			(3<<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 8 (PAF8)

#define R_PAF8 ((volatile unsigned int*)0x80023420)

#define F_PAF8_0_B0				(0<<0)
#define F_PAF8_0_SND2_PWML_P	(1<<0)
#define F_PAF8_0_SRAM_A8		(2<<0)
#define F_PAF8_0_GP0			(3<<0)

#define F_PAF8_1_B1				(0<<2)
#define F_PAF8_1_SND2_PWML_N	(1<<2)
#define F_PAF8_1_SRAM_A9		(2<<2)
#define F_PAF8_1_GP1			(3<<2)

#define F_PAF8_2_B2				(0<<4)
#define F_PAF8_2_SND2_PWMR_P	(1<<4)
#define F_PAF8_2_CAP_IN1		(2<<4)
#define F_PAF8_2_GP2			(3<<4)

#define F_PAF8_3_B3				(0<<6)
#define F_PAF8_3_SND2_PWMR_N	(1<<6)
#define F_PAF8_3_TM_OUT1		(2<<6)
#define F_PAF8_3_GP3			(3<<6)

#define F_PAF8_4_B4				(0<<8)
#define F_PAF8_4_SND3_PWML_P	(1<<8)
#define F_PAF8_4_CAP_IN2		(2<<8)
#define F_PAF8_4_GP4			(3<<8)

#define F_PAF8_5_B5				(0<<10)
#define F_PAF8_5_SND3_PWML_N	(1<<10)
#define F_PAF8_5_TM_OUT2		(2<<10)
#define F_PAF8_5_GP5			(3<<10)

#define F_PAF8_6_B6				(0<<12)
#define F_PAF8_6_SND3_PWMR_P	(1<<12)
#define F_PAF8_6_CAP_IN3		(2<<12)
#define F_PAF8_6_GP6			(3<<12)

#define F_PAF8_7_B7				(0<<14)
#define F_PAF8_7_SND3_PWMR_N	(1<<14)
#define F_PAF8_7_TM_OUT3		(2<<14)
#define F_PAF8_7_GP7			(3<<14)
//======================================================================================================================

//======================================================================================================================
// Port Alternate Function Register 9 (PAF9)

#define R_PAF9 ((volatile unsigned int*)0x80023424)

#define F_PAF9_0_SNDL_SCLK		(0<<0)
#define F_PAF9_0_SFLASH_DQ0	(1<<0)
#define F_PAF9_0_CAP_IN0		(2<<0)
#define F_PAF9_0_GP0			(3<<0)

#define F_PAF9_1_SNDL_LRCLK		(0<<2)
#define F_PAF9_1_SFLASH_CLK		(1<<2)
#define F_PAF9_1_TM_OUT0		(2<<2)
#define F_PAF9_1_GP1			(3<<2)

#define F_PAF9_2_SNDL_SDO		(0<<4)
#define F_PAF9_2_SFLASH_DQ3	(1<<4)
#define F_PAF9_2_SRAM_CS3		(2<<4)
#define F_PAF9_2_GP2			(3<<4)
//======================================================================================================================

