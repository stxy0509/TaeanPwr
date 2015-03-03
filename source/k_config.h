#pragma once


// MAIN----------------------------------------------
#define KOGA_VER_STRING "1.3  150303-1045"


#if 0
#define BUOY_SPEC   'D'
#define BUOY_ID     "E01"
//#define E01_SHOCK
#define C_LOW_BAT_LIMIT 220
#endif

#if 0
#define BUOY_SPEC   'D'
#define BUOY_ID     "E02"
#define C_LOW_BAT_LIMIT 160
#define E02_BAT_ADJUST
#endif

#if 0
#define BUOY_SPEC   'D'
#define BUOY_ID     "E88"
#define C_LOW_BAT_LIMIT 220
#endif


#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "S88"
#define C_LOW_BAT_LIMIT 220
#endif


#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "S21"
#define C_LOW_BAT_LIMIT 220
#endif

#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "S22"
#define C_LOW_BAT_LIMIT 160
#endif

#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "S23"
#define C_LOW_BAT_LIMIT 220
#endif

#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "S24"
#define C_LOW_BAT_LIMIT 220
#endif

#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "S25"
#define C_LOW_BAT_LIMIT 220
#endif

#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "W01"
#define BUOY_WEST
#define C_LOW_BAT_LIMIT 160
#endif

#if 1
#define BUOY_SPEC   'K'
#define BUOY_ID     "W02"
#define BUOY_WEST
#define SHOCK_W02               //binary type (0xXX 0x00)
#define C_LOW_BAT_LIMIT 160
#endif

#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "W03"
#define BUOY_WEST
#define C_LOW_BAT_LIMIT 160
#define W03_BAT_ADJUST
#endif

#if 0
#define BUOY_SPEC   'K'
#define BUOY_ID     "W88"
#define BUOY_WEST
#define C_LOW_BAT_LIMIT 220
#endif


/*-------------------------------------------------------
 * debug only
 */

// #define FLAG_FOR_DEBUGGING          // debug & test only
// #define DEBUG_CH_0                  // change debug channel to uart_0
// #define DEBUG_SENSOR_SHOCK          // Shock sensor test
// #define DEBUG_SPEED_BOOT            // booting time : 120sec --> 3sec
// #define RTC_INIT_AT_BOOT            // booting time : rtc init
// #define MODEM_TEST_ONLY		           // booting time : rtc init
