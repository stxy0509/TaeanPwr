#define __K_TIMER_C__

#include "k_includes.h"


static u32 tick=0;
U32 tick_m1=0;
U32 tick_m2=0;
U32 tick_m3=0;

U32 tick_s1=0;
U32 tick_s2=0;
U32 tick_s3=0;

U32 tick_SHT=0;

U32 tick_iri0=0;
U32 tick_iri1=0;
U32 tick_sms=0;

U32 tick_pt100=0;
U32 tick_cat=0;

U32 tick_fileTx=0;

static U32 tick_1sec=100;
static s32 tm_stage=0;

//static int stMFP=0;
//static int stMFP1=0;
static int fg_rtc_updated=0;

u32 Tm_setting_mode = 0;

// ----> 130313-1534
// Watcdog
u32 fgWATCHDOG_ResetTrigger;

// u32 TM_watchSMS;
// u32 TM_watchAIO;
u32 TM_watchCT3919;
// u32 TM_watchPTB210;
// u32 TM_watchWAVE;
u32 TM_watchDCS;
// u32 TM_watchHMP155;
u32 TM_watchMAIN;
u32 TM_watchGPS;
// u32 TM_watchATM1;
// u32 TM_watchATM2;
// u32 TM_watchSHOCK;

#define WT_SMS          ( 5*60)
#define WT_AIO          ( 3*60)
#define WT_CT3919       ( 3*60)
#define WT_PTB210       ( 3*60)
#define WT_HMP155       ( 3*60)
#define WT_DCS          ( 3*60)
#define WT_GPS          ( 3*60)
#define WT_ATM1         ( 3*60)
#define WT_ATM2         ( 3*60)
#define WT_MAIN         ( 2*60)

//  <----



static int fg_SEC_elapsed = 0;
void clr_fg_SEC_elapsed(void)       {   fg_SEC_elapsed = 0;         }
int  get_fg_SEC_elapsed(void)       {   return (fg_SEC_elapsed);    }


//timer 0
//10ms

void timer0isr()
{

    tick++;

#if 0
    if (RTC_chkMfpPort())   // HIGH
    {
        if (stMFP == 0)
        {
            stMFP = 1;
            stMFP1 = 0;

            fg_rtc_updated= 1;
        }
        else
        {
            stMFP1++;
        }
    }
    else                    // LOW
    {
        if (stMFP == 1)
        {
            stMFP= 0;
            stMFP1 = 0;
        }
        else
        {
            stMFP1++;
        }
    }
    if (stMFP1 > 500)
    {
        stMFP1 = 0;
        fg_RTC_sync_req = 1;
    }
#endif

    // SB16C1058  ---------------
    fg_sb_high_10ms =1;

    sb1_high_cnt++;
    sb2_high_cnt++;


    // Tick counter -------------
    if (tick_m1>0) tick_m1--;
    if (tick_m2>0) tick_m2--;
    if (tick_m3>0) tick_m3--;

    if (tick_s1>0) tick_s1--;   // m_boot_proc()
    if (tick_s2>0) tick_s2--;   // m_get_time_pos_proc()
    if (tick_s3>0) tick_s3--;

    if (tick_iri0>0) tick_iri0--;
    if (tick_iri1>0) tick_iri1--;
    if (tick_sms>0) tick_sms--;
    // if (tick_pt100>0) tick_pt100--;
    // if (tick_SHT>0) tick_SHT--;

    if (tick_fileTx>0) tick_fileTx--;
    // if (tick_cat>0) tick_cat--;




    if (tick_1sec>0)
	{
		tick_1sec--;
	}
	else
	{
        // 1sec
        fg_SEC_elapsed = 1;
        fg_rtc_updated= 1;
        //tm_chk_sbdreg++;            // SBDREG 상태를 5분마다 주기적으로 모니터링하기 위해서

    // Raw Data Display interval-------
        // tmr_HMP_data_display++;
        // tmr_AIO_data_display++;
        tmr_GPS_data_display++;
        // tmr_MOSE_data_display++;
        // tmr_PTB_data_display++;
        //tmr_GPS_data_display++;
        // tmr_SHOCK_data_display++;

        Tm_setting_mode++;

		tm_stage++;
		tick_1sec = 100;

        // setHeartbeatEvent();
        decFwUpadateWaitTime();


        /* watchdog timer */
        fgWATCHDOG_ResetTrigger = 0;

        //if (++TM_watchSMS     > WT_SMS)     fgWATCHDOG_ResetTrigger += 0x01;
        // if (++TM_watchAIO     > WT_AIO)     fgWATCHDOG_ResetTrigger += 0x02;
        if (++TM_watchCT3919  > WT_CT3919)  fgWATCHDOG_ResetTrigger += 0x04;
        // if (++TM_watchPTB210  > WT_PTB210)  fgWATCHDOG_ResetTrigger += 0x08;
         // if (++TM_watchHMP155 > WT_HMP155)  fgWATCHDOG_ResetTrigger += 0x10;
        if (++TM_watchDCS     > WT_DCS)     fgWATCHDOG_ResetTrigger += 0x20;
        if (++TM_watchGPS     > WT_GPS)     fgWATCHDOG_ResetTrigger += 0x40;
        // if (++TM_watchMAIN    > WT_MAIN)    fgWATCHDOG_ResetTrigger += 0x80;

        if (fgWATCHDOG_ResetTrigger > 0)
        {
            /* reset */

            // debugprintf("\r\n\r\n !!!!************ WATCHDOG RESET ( %04X ) ****************!!!!\r\n", fgWATCHDOG_ResetTrigger);
            // *R_WDCNT=WATCHDOG_V;
            // *R_WDTCTRL = 1;
            // while(1){;};
        }

	}
}



int getTick()
{
    return tick;
}
s32 get_tm_stage(void)
{
    return tm_stage;
}
void set_tm_stage(s32 a_tm)
{
    tm_stage = a_tm;;
}
void clear_tm_stage(void)
{
    tm_stage = 0;;
}

int isMCP79410_updated(void)
{
    return fg_rtc_updated;
}

void clearMCP79410_update_flag(void)
{
    fg_rtc_updated = 0;
}

