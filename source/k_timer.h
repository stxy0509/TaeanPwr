#pragma once

#define WATCHDOG_V	0x8F0D180

#define TM_1SEC     (1000/10)
#define TM_2SEC     (2000/10)
#define TM_5SEC     (5000/10)
#define TM_8SEC     (8000/10)
#define TM_10SEC    (10000/10)
#define TM_20SEC    (20000/10)
#define TM_30SEC    (30000/10)



//-----------------------------------------------
#ifdef __K_TIMER_C__
//-----------------------------------------------



//-----------------------------------------------
#else
//-----------------------------------------------


//timer 0

extern U32 tick_m1;
extern U32 tick_m2;
extern U32 tick_m3;

extern U32 tick_s1;
extern U32 tick_s2;
extern U32 tick_s3;

extern U32 tick_iri0;
extern U32 tick_pt100;
extern U32 tick_SHT;
extern U32 tick_fileTx;
extern U32 tick_cat;



//timer 0


// ----> 130313-1534
// Watcdog

// extern u32 TM_watchSMS;
// extern u32 TM_watchAIO;
extern u32 TM_watchCT3919;
// extern u32 TM_watchPTB210;
// extern u32 TM_watchWAVE;
// extern u32 TM_watchDCS;
// extern u32 TM_watchHMP155;
extern u32 TM_watchGPS;
extern u32 TM_watchMAIN;
// extern u32 TM_watchATM1;
// extern u32 TM_watchATM2;
// extern u32 TM_watchSHOCK;

extern u32 Tm_setting_mode;
//  <----



//-----------------------------------------------
#endif
//-----------------------------------------------


void timer0isr();
int getTick();

s32 get_tm_stage(void);
void set_tm_stage(s32 a_tm);
void clear_tm_stage(void);

void clearMCP79410_update_flag(void);
int isMCP79410_updated(void);

void clr_fg_SEC_elapsed(void);
int  get_fg_SEC_elapsed(void);


