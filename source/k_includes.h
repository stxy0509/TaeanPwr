#pragma once

#include "adstar.h"
#include "k_config.h"



typedef union
{
  struct
  {
    u32 aio:2;          //lsb
    u32 mose:2;
    u32 dum1:2;
    u32 ct3919:2;


    u32 dum2:2;
    u32 dcs:2;
    u32 dum3:2;
    u32 gps:2;

    u32 atm1:2;
    u32 atm2:2;
    u32 hmp155:2;
    u32 internal:2;

    u32 ptb210:2;
    u32 dum4:2;
    u32 drift:2;
    u32 shock:2;       //msb

  } b;
  u32 w;
} SensorStatus_T;


// GPS-----------------------------------------------
typedef struct
{
    int valid;
    u32 latitude;
    u32 longitude;
    char s_lat[7+1];
    char s_lon[8+1];
} gps_data_T;



// RTC-----------------------------------------------
typedef struct
{
	int	sec;
	int	min;
	int	hour;
	int	day;
	int	week; // 0:Sun,1:Mon,...
	int	mon; // 1: Jan, 2:Feb,...
	int	year;
}rtcTime;

// MSG-----------------------------------------------
#define MSG_LENGTH		1024

// SD CARD-------------------------------------------
#define FN_AIO		"ai.dat"
#define FN_MOSE		"mo.dat"
#define FN_CT3919	"ct.dat"
#define FN_DCS		"zp.dat"
#define FN_ATM1		"t1.dat"
#define FN_ATM2		"t2.dat"
#define FN_HMP155	"hm.dat"
#define FN_GPS		"gp.dat"
#define FN_PTB210	"pt.dat"
#define FN_SHOCK	"sh.dat"
#define FN_SEND		"se.dat"
#define FN_HISTORY	"hi.dat"

#define SDC_BOOT_TAG    1
#define SDC_MOUNT_TAG   2
#define SDC_TIME_TAG    3



// =============================================
#include "k_main.h"
#include "k_aio.h"
#include "k_atm.h"
#include "k_gps.h"
#include "k_cli.h"
#include "k_cli_func.h"
#include "k_ct3919.h"
#include "k_dcs.h"
#include "k_hmp155.h"
#include "k_iridium.h"
#include "k_misc.h"
#include "k_mose.h"
#include "k_msg.h"
#include "k_ptb210.h"
#include "k_rtc.h"
#include "k_sb16c1058.h"
#include "k_sdc.h"
#include "k_shock.h"
#include "k_sysMCU.h"
#include "k_timer.h"
#include "k_twi.h"
#include "k_wifi.h"

//#endif
