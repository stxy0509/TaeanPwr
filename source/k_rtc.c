#define __K_RTC_C__

#include "k_includes.h"

/*
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
*/

//RTC
rtcTime rtc_time;
rtcTime t1;

void rtc_init(rtcTime a_time)
{
 	debugstring("RTC init\r\n");
	rtc_settime(&a_time);
}

void rtc_set_to_zero(void)
{
/*
	rtcTime ref_time;

	ref_time.min = 0;
	ref_time.hour = 0;
	ref_time.sec = 0;
	ref_time.day = 0;
	ref_time.week = 0;
	ref_time.mon = 0;
	ref_time.year = 0;

	rtc_settime(&ref_time);
*/
}

void rtc_setSleepInterval(int a_interval)
{
/*
	u32 temp;
	*R_RTCCON = a_interval;
	do
		{
		temp = *R_RTCCON;
		}while ( temp & (1<<5) );
*/
}

void rtc_setAlarmTime_60min(void)
{
/*
	rtcTime a_t1;
	//int i;

	rtc_gettime(&a_t1);
	PRINT_TIME;

#if 1
	// 1½Ã°£ ÁÖ±â
	u8 set_minValue;

	a_t1.sec = 0;

	if (a_t1.min>50)
		{
		set_minValue = 59;	//ºÐ´ÜÀ§·Î´Â ÃÖ´ë°ªÀÌ¹Ç·Î...
		}
	else
		{
		set_minValue = 51-a_t1.min;	//ºÐ´ÜÀ§·Î´Â ÃÖ´ë°ªÀÌ¹Ç·Î...
		}
	*R_RTCALM = (set_minValue<<8);
#endif
	debugprintf("\r\n ----> ALM: after %d min\r\n\r\n",set_minValue);
*/
}

void rtc_setAlarmTime_10min(int a_interval)
{
/*
	rtcTime a_t1;
	//int i;

	rtc_gettime(&a_t1);
	PRINT_TIME;
	u8 set_minValue=0;
	a_t1.sec = 0;

	if (a_interval == 10)
		{
		// 10 min

		if (a_t1.min>52)
			{
				set_minValue = (63-a_t1.min);
			}
		else if ((a_t1.min>=50)||(a_t1.min>42))
			{
				set_minValue = (53-a_t1.min);
			}
		else if ((a_t1.min>=40)||(a_t1.min>32))
			{
			set_minValue = (43-a_t1.min);
			}
		else if ((a_t1.min>=30)||(a_t1.min>22))
			{
			set_minValue = (33-a_t1.min);
			}
		else if ((a_t1.min>=20)||(a_t1.min>12))
			{
			set_minValue = (23-a_t1.min);
			}
		else if ((a_t1.min>=10)||(a_t1.min>02))
			{
			set_minValue = (13-a_t1.min);
			}
		else
			{
			set_minValue = (03-a_t1.min);
			}
		*R_RTCALM = (set_minValue<<8);
		}
	else if (a_interval == 20)
		{
		// 20 min
		if (a_t1.min>52)
			{
				set_minValue = (73-a_t1.min);
			}
		else if ((a_t1.min>32))
			{
			set_minValue = (53-a_t1.min);
			}
		else if ((a_t1.min>12))
			{
			set_minValue = (33-a_t1.min);
			}
		else
			{
			set_minValue = (13-a_t1.min);
			}
		*R_RTCALM = (set_minValue<<8);
		}

	debugprintf("\r\n ----> ALM: after %d min\r\n\r\n",set_minValue);
*/	
}


void set_rtc_sysclk_sync_req(int a_val)	{	fg_RTC_sync_req = a_val;	}
int get_rtc_sysclk_sync_req(void)		{	return fg_RTC_sync_req;		}

// static int fg_TRBM_sync_req = 0;
// void set_trbm_sysclk_sync_req(int a_val)	{	fg_TRBM_sync_req = a_val;	}
// int get_trbm_sysclk_sync_req(void)		{	return fg_TRBM_sync_req;		}



void rtc_set_test_time(void)
{
/*	debugstring("\r\n\r\n RTC set \r\n");
	rtcTime t2;

	rtc_gettime(&t2);
	t2.min = 49;
	rtc_settime(&t2);
*/}

void printTimeTag(void)
{
	rtc_isSecUpdate();
	debugprintf("\r\n%04d.%02d.%02d %02d:%02d:%02d\r\n",
				rtc_time.year, rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min,rtc_time.sec);
}

int evt_chged_min = 0;


int rtc_isSecUpdate()
{
	static rtcTime rt_before={9999,9999,9999,9999,9999,9999,9999};
	//static rtcTime rt_now;

	if(isMCP79410_updated())
	{
		rtc_gettime(&rtc_time);
		clearMCP79410_update_flag();

		//rt_now = rtc_time;
		if (rtc_time.min != rt_before.min)
		{
			evt_chged_min = 1;		// 시간이 흘러 '분'이 변했다.
		}

		rt_before = rtc_time;			// 현재 시간으로 갱신.
		return 1;
	}
	return 0;
}

int is_updated_min(void)
{
	rtc_isSecUpdate();
	if ( evt_chged_min==1 )
	{
		evt_chged_min = 0;
		return 1;
	}
	return 0;
}

void clr_updated_min(void)
{
	evt_chged_min = 0;
}


#if 1
void RTC_ISR(void)
{
/*	delayms(20);

	*R_PLLCTRL = 0x00002040; //96MHz # 95.9616MHz XIN: 11.2896MHz //PLL ON //!!!!
	delayms(50);

	*R_CKR = 1;	//PLL Clock //Switching ( PLL OFF & Select External Clock )

	*R_PCLKGATE |= 0xf1Bf;
	*R_HCLKGATE = 0xff;

	debugstring("\r\nRTC_ISR\r\n");

	*R_RTCCON = 0;	// Alarm Interrupt

	U32 temp;
	do
	{
		temp = *R_RTCCON;
	}while ( temp & (1<<5) );
*/}
#endif



//void rtc_gettime(rtcTime* tm)
//{
//}


