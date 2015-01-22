/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

 이 파일을 수정 함으로써 전체 실행파일 사이즈와 메모리 사용량을 줄일 수도 있다.
 하지만 줄어든 만큼 성능의 저하가 발생 할 경우도 있다.
 제거 하고자 하는 기능은 0 으로 설정 함으로써 제거된다.

******************************************************************************/
#pragma once

/******************************************************************************
	Nested Interrupt 
	특정 인터럽트 서비스 함수(ISR)내에서 또다른 인터럽트를 받아 들일 것인지에 대한 설정
	특정 인터럽트를 1 로 설정 할 경우 해당 인터럽트 함수내에서 모든 다른 인터럽트를 받아 들인다.
	단,인터럽트 진입 속도가 그렇지 않은 경우 보다 느리다.
	ex)
		SDK 내의 MP3 decoding 은 DMA 인터럽트내에서 frmae 단위로 이루어 진다.
		따라서 이 구간내에서도 다른 인터럽트를 받아 들이고자 한다면
		모든 DMA 인터럽트를 1 로 설정 하면 된다.
******************************************************************************/
#define SUPPORT_NESTED_INTNUM_EIRQ0	0
#define SUPPORT_NESTED_INTNUM_TIMER0 0
#define SUPPORT_NESTED_INTNUM_SOUNDMIXER 0
#define SUPPORT_NESTED_INTNUM_DMA0 1 //DMA is used by sound , 1 is recommended

#define SUPPORT_NESTED_INTNUM_FRAMESYNC 0
#define SUPPORT_NESTED_INTNUM_GPIOA 0
#define SUPPORT_NESTED_INTNUM_UART0 0
#define SUPPORT_NESTED_INTNUM_DMA1 1 

#define SUPPORT_NESTED_INTNUM_EIRQ1 0 
#define SUPPORT_NESTED_INTNUM_TIMER1 0
#define SUPPORT_NESTED_INTNUM_PMU 0
#define SUPPORT_NESTED_INTNUM_DMA2 1

#define SUPPORT_NESTED_INTNUM_SPI0 0
#define SUPPORT_NESTED_INTNUM_GPIOB 0
#define SUPPORT_NESTED_INTNUM_UART1 0
#define SUPPORT_NESTED_INTNUM_DMA3 1

#define SUPPORT_NESTED_INTNUM_RESV1 0
#define SUPPORT_NESTED_INTNUM_TIMER2 0
#define SUPPORT_NESTED_INTNUM_USB 0
#define SUPPORT_NESTED_INTNUM_DMA4 1

#define SUPPORT_NESTED_INTNUM_USBHOST 0
#define SUPPORT_NESTED_INTNUM_GPIOC 0
#define SUPPORT_NESTED_INTNUM_UART2 0
#define SUPPORT_NESTED_INTNUM_DMA5 1

#define SUPPORT_NESTED_INTNUM_RESV2 0
#define SUPPORT_NESTED_INTNUM_TIMER3 0
#define SUPPORT_NESTED_INTNUM_NAND 0
#define SUPPORT_NESTED_INTNUM_DMA6 1

#define SUPPORT_NESTED_INTNUM_SDHC 0
#define SUPPORT_NESTED_INTNUM_GPIOD 0
#define SUPPORT_NESTED_INTNUM_UART3 0
#define SUPPORT_NESTED_INTNUM_DMA7 1

#define SUPPORT_NESTED_INTNUM_RESV3 0
#define SUPPORT_NESTED_INTNUM_GPIOE 0
#define SUPPORT_NESTED_INTNUM_ADC 0
#define SUPPORT_NESTED_INTNUM_GPIOF 0

#define SUPPORT_NESTED_INTNUM_WATCHDOG 0
#define SUPPORT_NESTED_INTNUM_GPIOG	0
#define SUPPORT_NESTED_INTNUM_UART4	0
#define SUPPORT_NESTED_INTNUM_GPIOH	0

#define SUPPORT_NESTED_INTNUM_RESV4 0
#define SUPPORT_NESTED_INTNUM_GPIOI 0
#define SUPPORT_NESTED_INTNUM_TWI 0
#define SUPPORT_NESTED_INTNUM_GPIOJ 0

#define SUPPORT_NESTED_INTNUM_SPI1 0
#define SUPPORT_NESTED_INTNUM_CAPOVERFLOW 0
#define SUPPORT_NESTED_INTNUM_MOTORFAULTA 0
#define SUPPORT_NESTED_INTNUM_MOTORFAULTB 0

#define SUPPORT_NESTED_INTNUM_RESV5 0
#define SUPPORT_NESTED_INTNUM_MOTORQEI 0
#define SUPPORT_NESTED_INTNUM_PWM 0
#define SUPPORT_NESTED_INTNUM_RESV6 0

/******************************************************************************
	INTERNAL TIME OUT
******************************************************************************/
/*
	TWI Function에서 Status를 기다리는 최대 시간을 설정한다.
*/
#define TWI_RESP_TIME_OUT_COUNT (7200*100)	// About 100ms @ AHB 101MHz

/******************************************************************************
	External NAND/SD-Card/USB Memory Supports
******************************************************************************/

#define CONFIG_NAND 1
#define CONFIG_SDCARD 1

#if (CONFIG_SDCARD != 0)
/*
SDCARD 는 두 군데 Port Alternate 가 되어 있다.
만약 NAND 와 SDCARD 를 같은 Port 를 사용 한다면
SDK 내부에서 Port 를 변환 하게 되어 있다.
따라서 사용자가 어떤 Port 를 사용하는지 여기서 선언 해야만 한다.

SDCARD_PIN_USE_67 를 1 로 선언 한다면 nand 와 sd card 가 서로 다른 Port 를
사용하므로 내부적으로 Port 변환을 하지 않는다. 
따라서 사용자가 직접 최초 한번 Port alternative Function(0x800234xx) 를 초기화 해야 한다.
*/
#define SDCARD_PIN_USE_67 0
#endif


/******************************************************************************
	FAT File System
******************************************************************************/
//drive number
#define DRIVE_NAND 0
#define DRIVE_SDCARD 1

#define CONFIG_FAT_READONLY 0
#define CONFIG_FAT_VOLUMES 2

/*
	아래 설정 값은 include/fatfs/ffconf.h 파일을 참조.
	영문만을 사용 할 경우 
		CONFIG_CODE_PAGE 437 로 설정
		library 프로젝트에 lib_src/fatfs/option/ccsbcs.c 파일을 추가 한 후 rebuild 한다
	한글을 사용 할 경우 
		약 136,408 byte 가 늘어 난다. 
		CONFIG_CODE_PAGE 949 로 설정
		library 프로젝트에 lib_src/fatfs/option/cc949.c 파일을 추가 한 후 rebuild 한다
*/
#define CONFIG_CODE_PAGE 949


/******************************************************************************
	UART Config 
******************************************************************************/
/*
	인터럽트 모드일 경우 	uart 송/수신 data 를 임시로 저장 할 버퍼의 사이즈이다.
*/
// #define UART_BUF_SIZE 512
#define UART_BUF_SIZE (512*8)
#define CONFIG_UART_RX_INTERRUPT
//#define CONFIG_UART_TX_INTERRUPT

/*
	debugstring, debugprintf 함수의  최종 출력 채널
*/
#define DEBUG_CHANNEL 0

/******************************************************************************
	SYSTEM CLOCK
******************************************************************************/
/*
	외부 OSC 입력 값
	STK 보드의 경우 10Mhz OSC 를 사용한다.
	10Mhz 가 아닌 경우 adStarInit.c 의 PMUInit() 함수에서 PLL 설정 값을 수정 해야 한다.
	사용가능한 주파수는 doc/adStar_PLL_Setting.xls 파일을 이용.
*/
//#define OSC_CLOCK 8000000
#define OSC_CLOCK 10000000


/******************************************************************************
*	SOUND Mixer
******************************************************************************/
/*
	버퍼 사이즈를 줄이면 메모리 사용량은 줄어 들지만 
	인터럽트 발생 횟수가 늘어 난다.
	만약 wav 파일은 사용하지 않고 MP3 만 재생 한다면
	1 프레임씩 디코딩 하므로 이 사이즈를 약 5kbyte 만 설정 해도 충분하다.
*/
#define WAVE_BUF_MAX (512*1024) //if you use MP3, > 5Kbyte
#define DEFAULT_VOLUME 255//max 255
/*
	0,1 번 채널은 I2S 출력이 2,3 번 채널은 digital modulator 출력
	STK 보드의 경우 2번 채널을 이용한다.
*/
#define SND_OUTPUT_CHANNEL	2
/*
	최종 출력 sample rate
*/
#define SND_OUTPUT_HZ	48000




/******************************************************************************
*
******************************************************************************/
