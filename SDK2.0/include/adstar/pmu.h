/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#pragma once


//======================================================================================================================
// PMU Write Enable Register (PMUWREN)

#define R_PMUWREN  ((volatile unsigned int*)0x80023C00)

#define F_PMUWREN_BOD		(1 <<14)		// BOD Control Register Write Enable
#define F_PMUWREN_USB		(1 <<13)		// USB PHY Control Register Write Enable
#define F_PMUWREN_PCLK	(1 <<12)		// PCLK Control Register Write Enable
#define F_PMUWREN_HCLK	(1 <<11)		// HCLK Control Register Write Enable
#define F_PMUWREN_SOUND	(1 <<10)		// Sound Clock Control Register Write Enable
#define F_PMUWREN_PLL		(1 << 9)		// PLL Control Register Write Enable
#define F_PMUWREN_CLOCK	(1 << 8)		// Clock Control Register Write Enable
#define F_PMUWREN_HALT	(3 << 1)		// Halt Control Register Write Enable

#define PMU_WRITE_ENABLE	(*(volatile unsigned int*)0x80023C00 = 0x7fff)//BOD is at 14bit
#define PMU_WRITE_DISABLE	(*(volatile unsigned int*)0x80023C00 = 0)
//======================================================================================================================

//======================================================================================================================
// Clock Control Register (CLKCON)

#define R_CLKCON  ((volatile unsigned int*)0x80023C20)

#define F_CLKCON_AHB_SCLK1			(0 <<16)		// AHB Clock Select - System Clock
#define F_CLKCON_AHB_SCLK2			(1 <<16)		// AHB Clock Select - System Clock / 2
#define F_CLKCON_AHB_SCLK3			(2 <<16)		// AHB Clock Select - System Clock / 3
#define F_CLKCON_AHB_SCLK4			(3 <<16)		// AHB Clock Select - System Clock / 4
#define F_CLKCON_AHB_SCLK5			(4 <<16)		// AHB Clock Select - System Clock / 5
#define F_CLKCON_AHB_SCLK6			(5 <<16)		// AHB Clock Select - System Clock / 6
#define F_CLKCON_AHB_SCLK7			(6 <<16)		// AHB Clock Select - System Clock / 7
#define F_CLKCON_AHB_SCLK8			(7 <<16)		// AHB Clock Select - System Clock / 8
#define F_CLKCON_AHB_SCLK9			(8 <<16)		// AHB Clock Select - System Clock / 9
#define F_CLKCON_AHB_SCLK10		(9 <<16)		// AHB Clock Select - System Clock / 10
#define F_CLKCON_AHB_SCLK11		(10<<16)	// AHB Clock Select - System Clock / 11
#define F_CLKCON_AHB_SCLK12		(11<<16)	// AHB Clock Select - System Clock / 12
#define F_CLKCON_AHB_SCLK13		(12<<16)	// AHB Clock Select - System Clock / 13
#define F_CLKCON_AHB_SCLK14		(13<<16)	// AHB Clock Select - System Clock / 14
#define F_CLKCON_AHB_SCLK15		(14<<16)	// AHB Clock Select - System Clock / 15
#define F_CLKCON_AHB_SCLK16		(15<<16)	// AHB Clock Select - System Clock / 16

#define F_CLKCON_CRTC_DSC1			(0 <<12)		// CRTC Clock Select - DotSrcClk
#define F_CLKCON_CRTC_DSC2			(1 <<12)		// CRTC Clock Select - DotSrcClk / 2
#define F_CLKCON_CRTC_DSC3			(2 <<12)		// CRTC Clock Select - DotSrcClk / 3
#define F_CLKCON_CRTC_DSC4			(3 <<12)		// CRTC Clock Select - DotSrcClk / 4
#define F_CLKCON_CRTC_DSC5			(4 <<12)		// CRTC Clock Select - DotSrcClk / 5
#define F_CLKCON_CRTC_DSC6			(5 <<12)		// CRTC Clock Select - DotSrcClk / 6
#define F_CLKCON_CRTC_DSC7			(6 <<12)		// CRTC Clock Select - DotSrcClk / 7
#define F_CLKCON_CRTC_DSC8			(7 <<12)		// CRTC Clock Select - DotSrcClk / 8
#define F_CLKCON_CRTC_DSC9			(8 <<12)		// CRTC Clock Select - DotSrcClk / 9
#define F_CLKCON_CRTC_DSC10		(9 <<12)		// CRTC Clock Select - DotSrcClk / 10
#define F_CLKCON_CRTC_DSC11		(10<<12)	// CRTC Clock Select - DotSrcClk / 11
#define F_CLKCON_CRTC_DSC12		(11<<12)	// CRTC Clock Select - DotSrcClk / 12
#define F_CLKCON_CRTC_DSC13		(12<<12)	// CRTC Clock Select - DotSrcClk / 13
#define F_CLKCON_CRTC_DSC14		(13<<12)	// CRTC Clock Select - DotSrcClk / 14
#define F_CLKCON_CRTC_DSC15		(14<<12)	// CRTC Clock Select - DotSrcClk / 15
#define F_CLKCON_CRTC_DSC16		(15<<12)	// CRTC Clock Select - DotSrcClk / 16

#define F_CLKCON_CRTC_PLL1			(1 << 9)		// CRTC Source Clock Select - 0 External CRTC Clock / 1 PLL 1 Clock
#define F_CLKCON_CRTC_ENABLE		(1 << 8)		// CRTC Clock Enable bit - 0 Disable / 1 Enable
#define F_CLKCON_WIRQ1_HIGH		(1 << 7)		// WIRQ1 Mode - 0 IRQ1 High / 1 IRQ1 Low
#define F_CLKCON_WIRQ0_HIGH		(1 << 6)		// WIRQ0 Mode - 0 IRQ0 High / 1 IRQ0 Low
#define F_CLKCON_WIRQ1_ENABLE		(1 << 5)		// WIRQ1 Enable bit - 0 WIRQ1 Disable / 1 External IRQ1 is WIRQ1
#define F_CLKCON_WIRQ0_ENABLE		(1 << 4)		// WIRQ0 Enable bit - 0 WIRQ0 Disable / 1 External IRQ0 is WIRQ0
#define F_CLKCON_USBCLK_ENABLE		(1 << 3)		// USB Clock Enable bit - 0 Disable / 1 Enable
#define F_CLKCON_USBCLK_SRC_DIV2		(1 << 2)		// USB Clock Select - 0 USB Source Clock/2  / 1 USB Source Clock
#define F_CLKCON_USBSRC_CRTC		(1 << 1)		// USB Source Clock Select - 0 AHB Clock / 1 CRTC Clock
#define F_CLKCON_SCLK_PLL0			(1 << 0)		// System Clock Select bit - 0 External Clock / 1 PLL0 Clock
//======================================================================================================================

//======================================================================================================================
// PLL Control Register (PLLCON)

#define R_PLLCON		((volatile unsigned int*)0x80023C24)

#define F_PLLCON_PLL1OS(n)					(n <<12)		// PLL 1 OS
#define F_PLLCON_PLL1IS(n)					(n << 8)		// PLL 1 IS
#define F_PLLCON_PLL0OS(n)					(n << 4)		// PLL 0 OS
#define F_PLLCON_PLL0IS(n)					(n << 0)		// PLL 0 IS
//======================================================================================================================

//======================================================================================================================
// Sound Control Register (SNDCLKCON)

#define R_SNDCLKCON	((volatile unsigned int*)0x80023C28)

#define F_SNDCLKCON_DMCLKSRC_DISABLE		(0 <<12)		// Clock Disable
#define F_SNDCLKCON_DMCLKSRC_EXTCLK		(1 <<12)		// External Clock
#define F_SNDCLKCON_DMCLKSRC_PLLCLK		(2 <<12)		// PLL Clock
#define F_SNDCLKCON_DMCLKSRC_CP0			(3 <<12)		// Capture Input[0]

#define F_SNDCLKCON_DMCLKSRC				(0 << 8)		// DMCLKSRC
#define F_SNDCLKCON_DMCLKSRC2			(1 << 8)		// DMCLKSRC / 2
#define F_SNDCLKCON_DMCLKSRC4			(2 << 8)		// DMCLKSRC / 4
#define F_SNDCLKCON_DMCLKSRC8			(3 << 8)		// DMCLKSRC / 8

#define F_SNDCLKCON_I2S_DISABLE			(0 << 4)		// I2S Source Clock Select - Disable
#define F_SNDCLKCON_I2S_EXTCLK				(1 << 4)		// I2S Source Clock Select - External Clock
#define F_SNDCLKCON_I2S_APBCLK				(2 << 4)		// I2S Source Clock Select - APB Clock
#define F_SNDCLKCON_I2S_CP0				(3 << 4)		// I2S Source Clock Select - Capture Input[0]

#define F_SNDCLKCON_I2S_SRCCLK				(0 << 0)		// I2S Clock Divide Value - Source Clock
#define F_SNDCLKCON_I2S_SRCCLK2			(1 << 0)		// I2S Clock Divide Value - Source Clock / 2
#define F_SNDCLKCON_I2S_SRCCLK3			(2 << 0)		// I2S Clock Divide Value - Source Clock / 3
#define F_SNDCLKCON_I2S_SRCCLK4			(3 << 0)		// I2S Clock Divide Value - Source Clock / 4
#define F_SNDCLKCON_I2S_SRCCLK5			(4 << 0)		// I2S Clock Divide Value - Source Clock / 5
#define F_SNDCLKCON_I2S_SRCCLK6			(5 << 0)		// I2S Clock Divide Value - Source Clock / 6
#define F_SNDCLKCON_I2S_SRCCLK7			(6 << 0)		// I2S Clock Divide Value - Source Clock / 7
#define F_SNDCLKCON_I2S_SRCCLK8			(7 << 0)		// I2S Clock Divide Value - Source Clock / 8
#define F_SNDCLKCON_I2S_SRCCLK9			(8 << 0)		// I2S Clock Divide Value - Source Clock / 9
#define F_SNDCLKCON_I2S_SRCCLK10			(9 << 0)		// I2S Clock Divide Value - Source Clock / 10
#define F_SNDCLKCON_I2S_SRCCLK11			(10<< 0)		// I2S Clock Divide Value - Source Clock / 11
#define F_SNDCLKCON_I2S_SRCCLK12			(11<< 0)		// I2S Clock Divide Value - Source Clock / 12
#define F_SNDCLKCON_I2S_SRCCLK13			(12<< 0)		// I2S Clock Divide Value - Source Clock / 13
#define F_SNDCLKCON_I2S_SRCCLK14			(13<< 0)		// I2S Clock Divide Value - Source Clock / 14
#define F_SNDCLKCON_I2S_SRCCLK15			(15<< 0)		// I2S Clock Divide Value - Source Clock / 15
#define F_SNDCLKCON_I2S_SRCCLK16			(16<< 0)		// I2S Clock Divide Value - Source Clock / 16
//======================================================================================================================

//======================================================================================================================
// AHB Clock Control Register (HCLKCON)

#define R_HCLKCON	((volatile unsigned int*)0x80023C2C)

#define F_HCLKCON_USBHOST_ENABLE			(1 <<12)		// USB Host AHB Clock Enable
#define F_HCLKCON_USBDEVICE_ENABLE		(1 <<11)		// USB Device AHB Clock Enable
#define F_HCLKCON_CRTCM_ENABLE				(1 <<10)		// CRTC AHB Master Clock Enable
#define F_HCLKCON_SDHC_ENABLE				(1 << 9)		// SDHC Clock Enable
#define F_HCLKCON_NAND_ENABLE				(1 << 8)		// NAND Flash Controller Clock Enable
#define F_HCLKCON_EXSRAM_ENABLE			(1 << 7)		// External SRAM Controller Clock Enable
#define F_HCLKCON_SERIAL_ENABLE			(1 << 6)		// Serial Flash Controller Clock Enable
#define F_HCLKCON_DMA_ENABLE				(1 << 5)		// DMA Clock Enable
#define F_HCLKCON_GPIO_ENABLE				(1 << 4)		// GPIO Clock Enable
#define F_HCLKCON_INTCON_ENABLE			(1 << 3)		// Interrupt Controller Clock Enable
#define F_HCLKCON_SDRAM_ENABLE			(1 << 2)		// SDRAM Clock Enable
#define F_HCLKCON_SDRAMCON_ENABLE		(1 << 1)		// SDRAM Controller Clock Enable
#define F_HCLKCON_AHB_ENABLE				(1 << 0)		// AHB Bus Clock Enable
//======================================================================================================================

//======================================================================================================================
// APB Clock Control Register (PCLKCON)

#define R_PCLKCON	((volatile unsigned int*)0x80023C30)

#define F_PCLKCON_PINMUX_ENABLE			(1 <<12)		// Pin MUX Clock Enable
#define F_PCLKCON_ADC_ENABLE				(1 <<11)		// ADC APB Clock Enable
#define F_PCLKCON_QEI_ENABLE				(1 <<10)		// QEI Clock Enable
#define F_PCLKCON_MPWM_ENABLE			(1 << 9)		// Motor PWM Clock Enable
#define F_PCLKCON_SMIXER_ENABLE			(1 << 8)		// Sound Mixer APB Clock Enable
#define F_PCLKCON_TWI_ENABLE				(1 << 7)		// TWI Clock Enable
#define F_PCLKCON_SPI1_ENABLE				(1 << 6)		// SPI1 Clock Enable
#define F_PCLKCON_SPI0_ENABLE				(1 << 5)		// SPI0 Clock Enable
#define F_PCLKCON_UART_ENABLE				(1 << 4)		// UART Clock Enable
#define F_PCLKCON_TIMER_ENABLE				(1 << 3)		// Timer Clock Enable
#define F_PCLKCON_WDT_ENABLE				(1 << 2)		// Watch Dog Timer Clock Enable
#define F_PCLKCON_CRTCS_ENABLE				(1 << 1)		// CRTC APB Slave Clock Enable
#define F_PCLKCON_APB_ENABLE				(1 << 0)		// APB Bus Clock Enable
//======================================================================================================================

//======================================================================================================================
// USB PHY Control Register (USBPHYCON)

#define R_USBPHYCON	((volatile unsigned int*)0x80023C34)

#define F_USBPHYCON_USBDEVICE				(0 << 8)		// USB Function Select bit - 0 USB Device / 1 USB Host
#define F_USBPHYCON_USBHOST				(1 << 8)		// USB Function Select bit - 0 USB Device / 1 USB Host

#define F_USBPHYCON_RCV_DISABLE			(0 << 4)		// Receive Enable bit - 0 USB PHY Disable / 1 USB PHY Enable
#define F_USBPHYCON_RCV_ENABLE			(1 << 4)		// Receive Enable bit - 0 USB PHY Disable / 1 USB PHY Enable

#define F_USBPHYCON_NDWPUL_DISABLE		(0 << 3)		// D- Weak Pull-up Enable bit - 0 Disable / 1 Enable
#define F_USBPHYCON_NDWPUL_ENABLE		(1 << 3)		// D- Weak Pull-up Enable bit - 0 Disable / 1 Enable

#define F_USBPHYCON_NDPUL_DISABLE			(0 << 2)		// D- Pull-up Enable bit - 0 Disable / 1 Enable
#define F_USBPHYCON_NDPUL_ENABLE			(1 << 2)		// D- Pull-up Enable bit - 0 Disable / 1 Enable

#define F_USBPHYCON_PDWPUL_DISABLE		(0 << 1)		// D+ Weak Pull-up Enable bit - 0 Disable / 1 Enable
#define F_USBPHYCON_PDWPUL_ENABLE		(1 << 1)		// D+ Weak Pull-up Enable bit - 0 Disable / 1 Enable

#define F_USBPHYCON_PDPUL_DISABLE			(0 << 0)		// D+ Pull-up Enable bit - 0 Disable / 1 Enable
#define F_USBPHYCON_PDPUL_ENABLE			(1 << 0)		// D+ Pull-up Enable bit - 0 Disable / 1 Enable
//======================================================================================================================

//======================================================================================================================
// BOD Control Register (BODCON)

#define R_BODCON	((volatile unsigned int*)0x80023C38)

#define F_BODCON_DISABLE			(0 << 0)		// BOD Enable - 0 Disable / 1 Enable
#define F_BODCON_ENABLE			(1 << 0)		// BOD Enable - 0 Disable / 1 Enable
//======================================================================================================================


U32 get_ahb_clock();
U32 get_apb_clock();

U32 get_pll(int num);

