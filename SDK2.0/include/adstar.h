/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

*****************************************************************************/
#pragma once

#ifndef _ADSTAR_H_
#define _ADSTAR_H_
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#define ISPM	__attribute__((section(".ispm")))
#define DSPM	__attribute__((section(".dspm")))


#ifdef __GNUG__ // The GNU C++ compiler defines this. Testing it is equivalent to testing (__GNUC__ && __cplusplus). 
extern "C"{
#endif
#include "lib_config.h"
#include "typedef.h"
#include "egl/egl.h"

#include "adstar/util.h"
#include "adstar/cacheutil.h"
#include "adstar/interrupt.h"	
#include "adstar/pmu.h"	
#include "adstar/uart.h"	
#include "adstar/timer.h"	
#include "adstar/nandctrl.h"	
#include "adstar/sdcard.h"	
#include "adstar/gpio.h"	
#include "adstar/dma.h"	
#include "adstar/twi.h"	
#include "adstar/crtc.h"	
#include "adstar/usb.h"	
#include "fatfs/ff.h"	
#include "adstar/soundmixer.h"
#include "adstar/error.h"
#include "adstar/adc.h"
#include "adstar/watchdogtimer.h"
#include "adstar/adStarInit.h"
#include "adstar/serialflash.h"
#include "adstar/spi.h"

	
#ifdef __GNUG__
}
#endif
