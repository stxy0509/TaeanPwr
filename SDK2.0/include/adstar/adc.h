/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.
*****************************************************************************/
#pragma once
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ADC Control Register (ADCCTRL)

#define R_ADCCTRL (volatile U32*)(0xA0023800)	
	//--------------------------------------------------------------------------------------------------------	
#define F_ADCCTRL_ETE 		(1<<15)				// External Tirigger Enable
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_ETS_T2 	(0<<12)				// External Trigger Source Select - TIMER2
#define F_ADCCTRL_ETS_T3 	(1<<12)				// External Trigger Source Select - TIMER3
#define F_ADCCTRL_ETS_PA7 	(2<<12)				// External Trigger Source Select - GPIOA7
#define F_ADCCTRL_ETS_PB7 	(3<<12)				// External Trigger Source Select - GPIOB7
#define F_ADCCTRL_ETS_PC7 	(4<<12)				// External Trigger Source Select - GPIOC7
#define F_ADCCTRL_ETS_PD7 	(5<<12)				// External Trigger Source Select - GPIOD7
#define F_ADCCTRL_ETS_PE7 	(6<<12)				// External Trigger Source Select - GPIOE7
#define F_ADCCTRL_ETS_PF7 	(7<<12)				// External Trigger Source Select - GPIOF7
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_PMS 		(1<<11)				// Periodic Mode Selection - Periodic Mode
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_DLT 		(1<<10)				// DMA Last Transfer - Request Enable
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_DME 		(1<<9)				// DMA Mode - Enable	
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_FIFO 		(1<<8)				// FIFO Mode - Enable
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_CH0 		(0<<5)				// ADC Channel Selection - Channel 0
#define F_ADCCTRL_CH1 		(1<<5)				// ADC Channel Selection - Channel 1
#define F_ADCCTRL_CH2 		(2<<5)				// ADC Channel Selection - Channel 2
#define F_ADCCTRL_CH3 		(3<<5)				// ADC Channel Selection - Channel 3
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_APB2 		(0<<2)				// Sampling Clock Selection - APB Clock/2
#define F_ADCCTRL_APB4 		(1<<2)				// Sampling Clock Selection - APB Clock/4
#define F_ADCCTRL_APB8 		(2<<2)				// Sampling Clock Selection - APB Clock/8
#define F_ADCCTRL_APB16 	(3<<2)				// Sampling Clock Selection - APB Clock/16
#define F_ADCCTRL_APB32 	(4<<2)				// Sampling Clock Selection - APB Clock/32
#define F_ADCCTRL_APB64 	(5<<2)				// Sampling Clock Selection - APB Clock/64
#define F_ADCCTRL_APB128 	(6<<2)				// Sampling Clock Selection - APB Clock/128
#define F_ADCCTRL_APB256 	(7<<2)				// Sampling Clock Selection - APB Clock/256
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_STC 		(1<<1)				// ADC Start Conversion
	//--------------------------------------------------------------------------------------------------------		
#define F_ADCCTRL_EN 		(1<<0)				// ADC Enable
	//--------------------------------------------------------------------------------------------------------		
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ADC Data Register (ADCDATA)

#define R_ADCDATA (volatile U32*)(0xA0023804)
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ADC FIFO Register (ADCFIFO)
#define R_ADCFIFO (volatile U32*)(0xA0023808)
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ADC Status Register (ADCSTAT)
#define R_ADCSTAT (volatile U32*)(0xA002380C)
	//--------------------------------------------------------------------------------------------------------	
#define F_ADCSTAT_FOVE		(1<<6)				// FIFO Overflow
#define F_ADCSTAT_FFUL		(1<<5)				// FIFO Full
#define F_ADCSTAT_FEMP		(1<<4)				// FIFO Empty
#define F_ADCSTAT_FLEV		(7<<1)				// FIFO Level
#define F_ADCSTAT_ADCDR		(1<<0)				// FIFO ADC Data Ready
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
