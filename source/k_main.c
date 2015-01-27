#define __K_MAIN_C__


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "k_includes.h"




int is_checkTime4Send(void);

void measure_BAT_leval(void);

// #define C_LOW_BAT_LIMIT	220

void m_boot_proc(void);
void m_get_time_pos_proc(void);

void m_sensor_proc(void);
void m_system_robust_proc(void);
void m_time_orinted_evt_proc(void);
void m_env_orinted_evt_proc(void);
void m_iridium_proc(void);

void chk_file_size(void);

extern void Task_sh11(void);

int main_step_000 = 1;
int main_step_100 = 0;
int main_step_200 = 0;
int main_step_300 = 0;
int main_step_400 = 0;

int main_step = 0;

int sb1_ok_cnt = 0;
int sb2_ok_cnt = 0;

int fgReqTRBM_timeSync = 0;
int fgReqTRBM_timeSync_1st = 2;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//			int main()
//.................................................................................
int main()
{
	//char ch;


	//==========< system_init >===================================================

	set_rtc_sysclk_sync_req(1);	//fg_RTC_sync_req = 1;	// request set & sync to clk source( GPS / IRIDIUM / MOSE )

	sensor_status.w = 0xFFFFFFFF;


	boardinit();

	sysWDT_init();


// #ifdef DEBUG_CH_0
// #else
//     set_debug_channel(4);
// #endif

	debugstring("\r\n================================================\r\n");
	debugprintf(    "      Echo Buoy \r\n");
	debugstring(    "------------------------------------------------\r\n");
	// debugprintf(    "      Buoy : %s \r\n", BUOY_ID);
	// debugprintf(    "      Ver  : %s \r\n", KOGA_VER_STRING);
	debugstring(    "================================================\r\n\r\n");


    // 환경변수 복원
    env_read();
    if (env.init_flag != 0x55AA)
    {
    	env_init();
    	env_save();
    }

    // init iridium
    set_iri1_init(1);

// #if (BUOY_SPEC == 'D')
//     memset(s_msg_e05,'9',MSG_LENGTH);
//     memset(s_msg_e30,'9',MSG_LENGTH);
// 	dp_stk_init();
// #endif

	// q_init_HF_mbuf();
	// q_init_LF_mbuf();

	while (1)
	{
		sysWDT_CntClear();

		switch (main_step)
		{
			case 0:
				m_boot_proc();			// main_step_000
				break;

			case 100:
				m_get_time_pos_proc();		// main_step_100

			case 200:
				m_sensor_proc();

				if (main_step_200 == 1)
				{
					m_time_orinted_evt_proc();
					m_env_orinted_evt_proc();
			    	m_system_robust_proc();		// heartbeat, watchdog


					m_iridium_proc();
				    task_iridium();
				    task_iri_rcv();
				}
				break;
		}

#if 0 	/* WM-211 */
        {
        	char ch;
	        if (uart_getch(3, &ch))
	        {
	        	uart_putch(0, ch);
	        }
	        if (uart_getch(0, &ch))
	        {
	        	uart_putch(3, ch);
	        }
        }
#endif


#if 0    /* SS510 */
        {
        	char ch;
	        if (uart_getch(2, &ch))
	        {
	        	uart_putch(0, ch);
	        }
	        if (uart_getch(0, &ch))
	        {
	        	uart_putch(2, ch);
	        }
        }
#endif

        task_cmdshell();
	    task_sysMCU();
	}
}







void sensor_q_init(void)
{
	debugstring("\r\ninit ");

 //    aio_q_init();		debugstring("AIO ");
 //    mose_q_init();		debugstring("MOSE ");
     ct_q_init();		debugstring("CT ");
 //    dcs_q_init();		debugstring("DCS ");

    gps_q_init();		debugstring("GPS ");
 //    atm1_q_init();		debugstring("ATM1 ");
 //    atm2_q_init();		debugstring("ATM2 ");
 //    hmp155_q_init();	debugstring("HMP ");
 //    ptb_q_init();		debugstring("PTB ");
 //    // SHOCK, internal TEMP/HUMI
	// shock_q_init();		debugstring("SHOCK ");
	// iri1_rcv_q_init();	debugstring("IRI1 ");
	// iri2_rcv_q_init();	debugstring("IRI2 ");
	debugstring(" queue..\r\n");
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//		m_get_time_pos_proc()
//.................................................................................
void m_get_time_pos_proc(void)
{
	static int idx = 0;

	if (main_step_100 == 0)
	{
		return;
	}

	switch (idx)
	{
		case 0:
			idx = 50;
			break;

		case 50:
			//cmdSensorControl_All_ON();
			//debugprintf("\r\npower_on : MOSE, GPS\r\n");
			debugprintf("  ... checking TIME, POSITION\r\n");

			// cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_CHK1);
			// cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_CHK2);
			//cmdSensorControl_All_ON();

			tick_s2 = (3*1000)/10;	//3sec
			idx = 60;
			break;

		case 60:
			// if (tick_s2==0)
			{
				// output to CHK1,CHK2 port
			    // sb_printstring(SB_S_CHK1, "1234567890");
			    // sb_printstring(SB_S_CHK2, "1234567890");

				// tick_s2 = (3*1000)/10;	//3sec
				idx = 70;
			}
			break;

		case 70:
		case 100:
			//tick_s2 = (30*1000)/10;	//20sec
			idx = 110;
			break;

		case 110:
			// if ( (sb1_ok_cnt > 5) && (sb2_ok_cnt > 5) )
			{
				// debugprintf("%d %d\r\n", sb1_ok_cnt, sb2_ok_cnt);

				// tick_s2 = (120*1000)/10;	//20sec
// PRINTLINE;
				// idx = 200;
			}
// 			else
// 			{
// 				if (tick_s2==0)
// 				{
// 				    SB_init();
// // PRINTLINE;
// 					tick_s2 = (3*1000)/10;	//3sec
// 					// idx = 100;
// 					idx = 60;
// 				}
// 			}
// 			break;


		case 200:
			// sb1_high_cnt = 0;
			// sb2_high_cnt = 0;

			// cmdSensorControl_All_ON();
			cmdSensorControl_byEnv();

            cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_IRI1);
            cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_IRI2);


// #ifdef DEBUG_SPEED_BOOT
// 			tick_s2 = (3*1000)/10;	//3sec
// #else
// 			tick_s2 = (120*1000)/10;	//120sec
// #endif

			idx = 210;
			break;

		case 210:
// 			if (is_gps_valid())
// 			{
// 				debugstring("GPS: time & position - valid\r\n");
// 				fg_start_sensing = 1;
// // PRINTLINE;
// 				idx = 300;
// 			}
// 			else if (get_mose_pos_valid())
// 			{
// 				debugstring("MOSE: time & position - valid\r\n");
// 				fg_start_sensing = 1;
// // PRINTLINE;
// 				idx = 300;
// 			}
// 			else if (tick_s2 == 0)
// 			{
// 				idx = 300;
// 			}


// #if 0
// 			// **chk** time limit 130719-1151
// 			if (tick_s2==0)
// 			{
// 			    SB_init();
// // PRINTLINE;
// 				idx = 100;
// 			}
// #endif
// 			break;

		case 300:
		    printTimeTag();

			fg_start_sensing = 1;	//(+)130828
			// fgReqTRBM_timeSync = 1;	//(+)130912
			{
	            // char tmp_buf[50];

		        // debugstring("TRBM timesync with RTC\r\n");
	            //sb_printstring(SB_S_ATM1, "+++");
	            // sb_printstring(SB_S_ATM2, "+++");
	            // debugstring("+++\r\n");
	            // delayms(500);
	            // delayms(500);

	            // sprintf(tmp_buf,"%%+T1%02d%02d%02d%02d%02d%02d\r\n",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
	            // sb_printstring(SB_S_ATM1, tmp_buf);
	            // sb_printstring(SB_S_ATM2, tmp_buf);
	            // debugstring(tmp_buf);

	            //fgReqTRBM_timeSync = 0;
			}
			idx = 400;
			break;

		case 400:
			idx = 500;
			break;

		case 500:
// PRINTLINE;
			idx = 600;
			break;

		case 600:
		    printTimeTag();
		    //sdc_mount();
// PRINTLINE;
			idx = 700;
			break;

		case 700:
			//
			if (sdc_read_detectPin()==SDC_INSERTED)
			{
			    //u32 fsz;
			    sdc_set_time_tag(SDC_BOOT_TAG);
			}


           //  if (sdc_read_detectPin()==SDC_INSERTED)
           //  {
           //  	debugstring("read defered file..\r\n");
     		   	// //readDeferedMsgFile();
           //  }


// PRINTLINE;
			idx = 800;
			break;

		case 800:
			//make_msg_k0();		// start msg.
			// make_msg_kx('0');
			idx = 850;
			break;

		case 850:
		    // power on (sensor, etc.)
        	//debugstring("Sensor Power ON.\r\n");

			//enable interrupt
			//enable_interrupt(INTNUM_EIRQ0, TRUE);
			//enable_interrupt(INTNUM_EIRQ1, TRUE);


// PRINTLINE;
			idx = 900;
			break;

		case 900:
			main_step_100 = 0;
			main_step_200 = 1;
// PRINTLINE;
			idx = 999;
			break;

		case 999:
		default:
			break;
	}

}





/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/

DWORD get_fattime (void)
{
	rtc_gettime(&rtc_time);
	return	  ((DWORD)(rtc_time.year - 1980) << 25)	/* Year = 2012 */
			| ((DWORD)rtc_time.mon << 21)				/* Month = 1 */
			| ((DWORD)rtc_time.day << 16)				/* Day_m = 1*/
			| ((DWORD)rtc_time.hour << 11)				/* Hour = 0 */
			| ((DWORD)rtc_time.min << 5)				/* Min = 0 */
			| ((DWORD)rtc_time.sec >> 1);				/* Sec = 0 */
}


int is_checkTime4Send(void)
{
	static u8 saved = 100;
	rtc_isSecUpdate();
	if (rtc_time.min == saved)
	{
		return 0;
	}

	saved = rtc_time.min;
	return 1;
}




int bat = 0;

int get_battery_level(void)
{
	return bat;
}

void measure_BAT_leval(void)
{
#define BAT_20	0x26d
#define BAT_24	0x2e6
#define BAT_STEP ((BAT_24 - BAT_20)/40.0f)
	int count = 0;
	volatile u32 val = 0;
	volatile U32 ADC_Result;
	volatile U32 status;

	//debugstring("ADC Polling Mode \r\n");
	*R_ADCCTRL = F_ADCCTRL_CH0 | F_ADCCTRL_PMS |F_ADCCTRL_APB256 | F_ADCCTRL_EN | F_ADCCTRL_FIFO;
	while(1)
	{
		status = *R_ADCSTAT;
		if((status & (1<<4))==0)//fifo not empty
		{
			ADC_Result = *R_ADCFIFO;
			ADC_Result &= 0x3ff;

			count++;


			// samples[samplecnt]=ADC_Result<<6;
			// samplecnt++;
			// if(samplecnt>=SAMPLE_COUNT)
				//break;
		}
		if (count > 5)
		{
			/* code */
			val += ADC_Result;
			//debugprintf("*** BAT : %x %x\r\n", ADC_Result, val);
			delayus(200);
			if (count == (21+16) )
			{
				float f;

				val >>= 5;
				f = (float)val;
				// debugprintf("        val= %f step= %f\r\n", f, BAT_STEP);
				f -= BAT_20;
				// debugprintf("  ..-BAT_20= %d\r\n", f);
				f /= (float)BAT_STEP;
				// debugprintf("../BAT_STEP= %f\r\n", f);
				// debugprintf("../BAT_STEP= %d\r\n", (int)f);
				f += 200;
				// debugprintf("        BAT= %d\r\n", (int)f);

#ifdef E02_BAT_ADJUST
				// BAT check를 위한 VR이 조정되어 있지 않아 강제로 보정함
				// 23.9V 일때 21.8V로 읽혀서... 21.8 * 1.0963 = 23.9
				// KOGA14
				f *= 1.0963f;
#endif

#ifdef W03_BAT_ADJUST
				// 25.1(Vin) --> 22.6(표출)
				// 25.1 / 22.6 = 1.110619469026549
				f *= 1.111f;
#endif

				bat = (int)floor(f+0.5);	//반올림
				// if ( val >= BAT_20)
				{
					//bat = (int)( ((val - BAT_20) / (float)BAT_STEP) +200.0f );
					//debugprintf("*** BAT_val : %d (%x)\r\n", bat, val);
					//bat += 200;
				}
				// else
				// {
				// 	bat = (int)( ((val - BAT_20) / BAT_STEP) );
				// 	//debugprintf("*** BAT_val : %d (%x)\r\n", bat, val);
				// 	bat += 200;
				// }
				debugprintf("*** BAT_val : %d (%x)\r\n", bat, val);
				break;
			}

		}
	}
	*R_ADCCTRL = 0;//off

}

void m_boot_proc(void)
{
	static int idx = 0;

	if (main_step_000 == 0)
	{
		return;
	}

	switch (idx)
	{
		case 0:
// PRINTLINE;
			idx = 100;
			break;

		case 100:
			//-----< init Interrupt >----------------------------------------

			//--< Timer0 INT >
			set_interrupt(INTNUM_TIMER0, timer0isr);
			set_timer(0,10);							// channel=0, interval=10ms, enable interrupt

			//--< EINT (P5.4, P5.5) >
			// enable_interrupt(INTNUM_EIRQ0, FALSE);
			// enable_interrupt(INTNUM_EIRQ1, FALSE);

			// set_interrupt(INTNUM_EIRQ0, EIRQ0_ISR);
			// set_interrupt(INTNUM_EIRQ1, EIRQ1_ISR);


			//-----< init Periperal >----------------------------------------

			//--< IIC >
			twi_set_freq (100000);

		    set_sdc_mount(0);   // sdc init status = unmount


			// set_debug_channel(4);



			// enable interrupt
			//enable_interrupt(INTNUM_EIRQ0, TRUE);
			//enable_interrupt(INTNUM_EIRQ1, TRUE);

            sensor_q_init();
            init_q();			// queue for SEND data

	        // init_aio_data();


// PRINTLINE;
		    idx = 200;
		    break;

	    case 200:
			//cmdSensorControl_All_OFF();	// sensor power off

// PRINTLINE;
		    idx = 300;
		    break;

	    case 300:
		    // SB16C1058 init
		    // SB_init();
		    // tick_s1 = (3000/10);
		    // idx = 340;
		    // break;

		case 340:
			// if (tick_s1 == 0)
			{
			    // SB_init();
			    // tick_s1 = (3000/10);
			    // idx = 350;
			}
		    // break;

		case 350:
			// if (tick_s1 == 0)
			{
			    // idx = 400;
			}
		    // break;

	    case 400:
		    // TestShell
		    App_TestShInit();


// PRINTLINE;
		    idx = 500;
		    break;

	    case 500:
#ifdef RTC_INIT_AT_BOOT
	        debugprintf("rtcc init..\r\n");
	        ini_rtcc();
	        ini_time();
#endif
		    idx = 600;
		    break;

	    case 600:
		    idx = 700;
		    break;

	    case 700:

		    idx = 800;
		    break;

	    case 800:
		    idx = 900;
		    break;

	    case 900:
	    	main_step_000 = 0;
	    	main_step_100 = 1;

// PRINTLINE;
	    	main_step = 100;
// PRINTLINE;
		    idx = 999;
	    	break;

	    case 999:
		default:
			break;
	}
}


void m_sensor_proc(void)
{
	// task_aio();
	// task_mose();
	// task_calc();

	task_ct3919();
	// task_dcs();

	task_gps();
	// task_atm1();
	// task_atm2();
	// task_hmp155();
	// task_ptb210();
	// Task_sh11();
	// task_shock();

}


void m_system_robust_proc(void)
{
    //-----< EIRQ0 / EIRQ1 : Watching interrupt PIN... >---------------------------
    //if (main_idx >= 220)

    if (get_fgFW_updating())	return;	// 131008



    // {
    // 	if (fg_sb_high_10ms ==1)
    // 	{
    // 		fg_sb_high_10ms = 0;

    // 		if((1<<4) == (*R_GP5ILEV & (1<<4)) )	// EIRQ0
    // 		{
    // 			if (sb2_high_cnt > (1000/10) )
    // 			{
    //     			//debugstring("\r\n.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+.+ SB2 HIGH .+.+.+.+.+.+\r\n");
    // 				debugstring("call EIRQ0_ISR()\r\n");
    // 				EIRQ0_ISR();
    // 			}
    // 		}

    // 		if((1<<5) == (*R_GP5ILEV & (1<<5)) )	// EIRQ1
    // 		{
    // 			if (sb1_high_cnt > (1000/10) )
    // 			{
    //     			//debugstring("\r\n.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.=.= SB1 HIGH .=.=.=.=.=.=\r\n");
    // 				debugstring("call EIRQ1_ISR()\r\n");
    // 				EIRQ1_ISR();
    // 			}
    // 		}

    // 	}
    // }

}


static int lowbat_interval = 0;
void dec_lowbat_interval(void)
{
	if ( lowbat_interval > 0 )
		lowbat_interval--;
}

static int pos_interval = 0;
void dec_pos_interval(void)
{
	if ( pos_interval > 0 )
		pos_interval--;
}

void position_chk(void)
{
	static int pos_cnt = 0;
	double lat, lon;
	int pos_valid = 0;

    if (get_mose_pos_valid())
    {
    	pos_valid= 1;
    	lat= get_mose_lat();
    	lon= get_mose_lon();
    }
    else if (is_gps_valid())
    {
    	pos_valid= 1;
    	lat= (double)(gps.latitude/100000.0f);
    	lon= (double)(gps.longitude/100000.0f);
    }

#if 1
	if ( (env.ref_flag == 1) && (pos_valid==1) )	// 기준위치 설정?
	{
		double k = 0.0f;
		k = pos_distance(env.ref_lat, env.ref_lon, lat, lon);

		debugprintf("\r\n++++++ %f, %f, %f, %f \r\n", env.ref_lat, env.ref_lon, lat, lon);
		debugprintf("\r\n++++++ Distance : %f \r\n", k);
		debugstring("+++++++++++++++++++++++++++++++++\r\n");
		if (k > env.ref_distance)
		{
			// 위치를 이탈했음.
			debugstring("++++++ Buoy is drifted...\r\n");
			if (pos_interval==0)
			{
				if (++pos_cnt < 6)
				{
					pos_interval = (10);	// 10분 간격
				}
				else
				{
					pos_interval = (24*60);	// 1일 간격
				}
				make_msg_kx('8');
			}
		}
		else
		{
			pos_cnt = 0;
			pos_interval = 0;
		}
	}
#else
	//if ( (env.ref_flag == 1) && (pos_valid==1) )	// 기준위치 설정?
	{
		double k = 0.0f;


		env.ref_lat= 32.070000;
		env.ref_lon= 125.150000;
		k = pos_distance(env.ref_lat, env.ref_lon, lat, lon);
		debugprintf("\r\n++++++ %f, %f, %f, %f \r\n", env.ref_lat, env.ref_lon, lat, lon);
		debugprintf("++++++ Distance : %f \r\n", k);
		debugstring("+++++++++++++++++++++++++++++++++\r\n");

		if (k > env.ref_distance)
		{
			// 위치를 이탈했음.
			debugstring("++++++ Buoy is drifted...\r\n");
			if (pos_interval==0)
			{
				if (++pos_cnt < 6)
				{
					pos_interval = (10);	// 10분 간격
				}
				else
				{
					pos_interval = (24*60);	// 1일 간격
				}
				//make_msg_kx('8');
				debugstring("++++++ message SEND...\r\n");
			}
		}
		else
		{
			pos_cnt = 0;
			pos_interval = 0;
		}
	}
#endif
}

static int tm_chk_sbdreg = 0;
int  get_tm_sbdreg(void)       {   return tm_chk_sbdreg;   }
void set_tm_sbdreg(int a_val)  {   tm_chk_sbdreg = a_val;   }



void m_time_orinted_evt_proc(void)
{
	int t_day;	//, t;

	if (get_fg_SEC_elapsed()==1)
	{
		clr_fg_SEC_elapsed();

		// 여기는 매 1초마다 실행되는 곳이다.


#if 0		
		dec_shk_interval();		// 충돌메시지 전송간격


		if ( dec_wifi_reset_tm() <= 0)
		{
			wifi_reset();
		}
#endif

	}


	if (is_checkTime4Send())
	{
		// 여기는 매 1분마다 실행되는 곳이다.
		t_day = ( rtc_time.hour*60 + rtc_time.min);


		debugprintf("It's time to send data : %02d:%02d:%02d\r\n",rtc_time.hour,rtc_time.min, rtc_time.sec);

		/*
		1. Make send-data.
		2. Push data to STACK, Tringgering for Transfer

		*/
		make_msg_k1();

	//----------------< 1 분마다 하는 작업 >----------------
	// 	1)
		#if 0
		dec_lowbat_interval();
		dec_pos_interval();		// 위치이탈 전송간격
		#endif

	// 	2) 이탈판정
		#if 0
		position_chk();
		#endif

		#if 0
	//----------------< 매월 1일 데이타 백업  >----------------
		if ( (t_day == 0) && (rtc_time.day==1))
		{
			SensorBakSize.b.shock = 1;
			SensorBakSize.b.send = 1;
			SensorBakSize.b.atm1 = 1;
			SensorBakSize.b.atm2 = 1;
			SensorBakSize.b.history = 1;
			SensorBakSize.b.ptb210 = 1;
			SensorBakSize.b.dcs = 1;
			SensorBakSize.b.ct3919 = 1;
			SensorBakSize.b.hmp155 = 1;
			SensorBakSize.b.gps = 1;
			SensorBakSize.b.aio = 1;
			SensorBakSize.b.mose = 1;
		}
		#endif


	//----------------< 전송할 데이타를 만든다(주기적인것... 기상) >----------------
		#if 0
		// 센서데이타 clear.
		if ( ((t_day % 60) == 20) || ((t_day % 60) == 50) )
		{
			PRINT_TIME;
			debugprintf("Clear data buffer (every 20,50min)\r\n");
		    set_aio_valid(0);   //clear
		    init_aio_data();

            set_hmp155_valid(0);   //clear
            init_hmp155_data();

            set_ptb210_valid(0);   //clear
            init_ptb210_data();
		}
		#endif


		#if 0
		// 기상데이타, 저전압감시
		if ( (t_day % env.interval) == 0)
		{
			PRINT_TIME;
			measure_BAT_leval();

			// 기상데이타
		    sdc_set_time_tag(SDC_TIME_TAG);
			make_msg_k1();

			chk_file_size();


			// 저전압 감시
			if (get_battery_level() < C_LOW_BAT_LIMIT)
			{
				if (lowbat_interval == 0)
				{
					lowbat_interval = (24*60);
					make_msg_kx('9');
				}
			}
		}
		#endif


		// 동해부이 (지진모드에서 5분)
#if (BUOY_SPEC == 'D')

// #ifdef FLAG_FOR_DEBUGGING
// 		//env.mode = 1;
// #endif
		if ( env.mode == 1)
		{
			// 지진모드
			if ( (t_day % 5) == 4)
			{
				sprintf(time_buf,"%02d%02d%02d%02d%02d00",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min-4);
			}
			if ( (t_day % 5) == 0)
			{
				//if (!is_dp_stk_empty())
				{
					make_msg_press_5();
					// dp_stk_init();
				}
			}
		}
		else
		{
			// Normal 모드
			if ( (t_day % 30) == 29)
			{
				sprintf(time_buf,"%02d%02d%02d%02d%02d00",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min-4);
			}
			if ( (t_day % 30) == 0)
			{
				//if (!is_dp_stk_empty())
				{
					make_msg_press_30();
					// dp_stk_init();
				}
			}
		}
#endif

		// // 기상데이타, 저전압감시
		// if ( (t_day % env.interval) == 0)
		// {
		// 	PRINT_TIME;
		// 	measure_BAT_leval();

		// 	// 저전압 감시
		// 	if (get_battery_level() < C_LOW_BAT_LIMIT)
		// 	{
		// 		if (lowbat_interval == 0)
		// 		{
		// 			lowbat_interval = (24*60);
		// 			make_msg_kx('9');
		// 		}
		// 	}

		// 	// 기상데이타
		//     sdc_set_time_tag(SDC_TIME_TAG);
		// 	make_msg_k1();

		// 	chk_file_size();
		// }

#if 0
		//----------------< MOSE 파고계산 >----------------
		if ( (t_day % 30) == 28)	// 매시 28 58분에 계산
		{
			if (env.s1== '1')
			{
				evt_calc_wave = 1;
				debugstring("\r\n\r\n=====>>>>> mose calc <<<<=========\r\n\r\n");
			}
		}

		//----------------< 매일 23:07 ---> RTC & sysTime sync >----------------
		if ( t_day == (23*60 + 7))
		{
			set_rtc_sysclk_sync_req(1);
		}

		//----------------< 매일 23:17 ---> TRBM & sysTime sync >----------------
		if ( (t_day == (23*60 + 17)) || (fgReqTRBM_timeSync==1) )
		// if ( (t_day % 10) == 0)	//for test only
		{
            char tmp_buf[25];
            if (fgReqTRBM_timeSync_1st > 0)
            {
            	fgReqTRBM_timeSync_1st--;
            }
            else
            {
		        debugstring("TRBM timesync with RTC\r\n");
	            // sb_printstring(SB_S_ATM2, "+++");
	            //sb_printstring(SB_S_ATM1, "+++");
	            // debugstring("+++\r\n");
	            // delayms(500);
	            // delayms(500);

	            sprintf(tmp_buf,"%%+T1%02d%02d%02d%02d%02d%02d\r\n",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
	            sb_printstring(SB_S_ATM1, tmp_buf);
	            // sb_printstring(SB_S_ATM2, tmp_buf);
	            debugstring(tmp_buf);

	            fgReqTRBM_timeSync = 0;
        	}
		}

		//----------------< SBDREG 상태체크 >----------------
		if ( (t_day % 30) == 13)
		{
			set_tm_sbdreg(1);
		}
#endif		
	}
}



void m_env_orinted_evt_proc(void)
{

}

void m_iridium_proc(void)
{}



void chk_file_size(void)
{
    char buf1[40];

	if (SensorBakSize.w > 0)
    {
	    PRINTLINE;
	    debugprintf("Data File Backup...\r\n");

        if (sdc_read_detectPin()==SDC_INSERTED)
        {
            sprintf(buf1, "1:%02d%02d", (rtc_time.year-2000), rtc_time.mon);
            f_mkdir(buf1);

            // sentMsg
            if (SensorBakSize.b.send == 1)
            {
                sprintf(buf1, "%02d%02d/se_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:se.dat", buf1);
                debugprintf("Data File Backup... sentMsg, ");
            }

            // rDcs.dat
            if(SensorBakSize.b.dcs == 1)
            {
                sprintf(buf1, "%02d%02d/zp_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:zp.dat", buf1);
                debugprintf("dcs, ");
            }

            // rCompass.dat
            if (SensorBakSize.b.aio == 1)
            {
                sprintf(buf1, "%02d%02d/ai_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:ai.dat", buf1);
                debugprintf("aio, ");
            }

            // rCt3919.dat
            if (SensorBakSize.b.ct3919==1)
            {
                sprintf(buf1, "%02d%02d/ct_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:ct.dat", buf1);
                debugprintf("ct3919, ");
            }

            // rPTB210.dat
            if (SensorBakSize.b.ptb210==1)
            {
                sprintf(buf1, "%02d%02d/pt_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:pt.dat", buf1);
                debugprintf("ptb210, ");
            }

            // rMose.dat
            if (SensorBakSize.b.mose==1)
            {
                sprintf(buf1, "%02d%02d/mo_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:mo.dat", buf1);
                debugprintf("mose, ");
            }


            // rMose2.dat
            if (SensorBakSize.b.shock==1)
            {
                sprintf(buf1, "%02d%02d/sh_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:sh.dat", buf1);
                debugprintf("shock, ");
            }

            if (SensorBakSize.b.atm1==1)
            {
                sprintf(buf1, "%02d%02d/t1_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:t1.dat", buf1);
                debugprintf("atm1, ");
            }

            if (SensorBakSize.b.atm2==1)
            {
                sprintf(buf1, "%02d%02d/t2_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:t2.dat", buf1);
                debugprintf("atm2, ");
            }

            if (SensorBakSize.b.gps==1)
            {
                sprintf(buf1, "%02d%02d/gp_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:gp.dat", buf1);
                debugprintf("gps, ");
            }

            if (SensorBakSize.b.hmp155==1)
            {
                sprintf(buf1, "%02d%02d/hm_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:hm.dat", buf1);
                debugprintf("hmp155, ");
            }

            if (SensorBakSize.b.history==1)
            {
                sprintf(buf1, "%02d%02d/hi_%02d%02d%02d%02d%02d.dat", (rtc_time.year-2000), rtc_time.mon, (rtc_time.year-2000), rtc_time.mon, rtc_time.day, rtc_time.hour,rtc_time.min);
                f_rename("1:hi.dat", buf1);
                debugprintf("history, ");
            }

            debugprintf(" <<<-------------\r\n\r\n");
            SensorBakSize.w = 0;    // flag all clear
        }//if (sdc_read_detectPin()==SDC_INSERTED)
    }
}


void env_init(void)
{
	debugstring("env: init\r\n");
    env.init_flag = 0x55AA;

    env.s0 = '1';		//'0'=Reset, '1'=On, '2'=Off
    env.s1 = '1';
    env.s2 = '1';
    env.s3 = '1';
    env.s4 = '1';
    env.s5 = '1';
    env.s6 = '1';
    env.s7 = '1';
    env.s8 = '1';
    env.s9 = '1';
    env.sa = '1';
    env.sb = '1';
    env.sc = '1';
    env.sd = '1';
    env.se = '1';
    env.sf = '1';
    env.sr = '1';

    env.interval = 30;		// 전송주기 (10, 20, 30, 60, 120)
    env.ref_lat = 0.0f;
    env.ref_lon = 0.0f;
    env.ref_flag = 0;		// 기준위치 설정유무
    env.ref_distance = 10;	// 1km // 1(100m)~100(10km)
    env.ref_shock = 180;	// 0 ~ 999
    env.mode = 0;			// 지진모드= 2: 해제		3: 설정
}


void env_save(void)
{
	debugstring("env: save\r\n");
    at24cxx_eeprom_write(10, &env, sizeof(env));
}

void env_read(void)
{
	debugstring("env: read\r\n");
    at24cxx_eeprom_read(10, &env, sizeof(env));
}

void env_print(void)
{
	debugprintf("sizeof(env)=%d\r\n",sizeof(env));

    debugprintf("env.init_flag : %x\r\n", env.init_flag);// = 0x55AA;
    debugprintf("env.s0 : %c\r\n", env.s0);// = 3;		// [센서상태] 0=off  3=on 	1,2=비정상off
    debugprintf("env.s1 : %c\r\n", env.s1);// = 3;
    debugprintf("env.s2 : %c\r\n", env.s2);// = 3;
    debugprintf("env.s3 : %c\r\n", env.s3);// = 3;
    debugprintf("env.s4 : %c\r\n", env.s4);// = 3;
    debugprintf("env.s5 : %c\r\n", env.s5);// = 3;
    debugprintf("env.s6 : %c\r\n", env.s6);// = 3;
    debugprintf("env.s7 : %c\r\n", env.s7);// = 3;
    debugprintf("env.s8 : %c\r\n", env.s8);// = 3;
    debugprintf("env.s9 : %c\r\n", env.s9);// = 3;
    debugprintf("env.sa : %c\r\n", env.sa);// = 3;
    debugprintf("env.sb : %c\r\n", env.sb);// = 3;
    debugprintf("env.sc : %c\r\n", env.sc);// = 3;
    debugprintf("env.sd : %c\r\n", env.sd);// = 3;
    debugprintf("env.se : %c\r\n", env.se);// = 3;
    debugprintf("env.sf : %c\r\n", env.sf);// = 3;
    debugprintf("env.sr : %c\r\n", env.sr);// = 3;
    debugprintf("env.interval     : %d\r\n", env.interval);//terval = 30;		// 전송주기 (10, 20, 30, 60, 120)
    debugprintf("env.ref_lat      : %f\r\n", env.ref_lat);// = 0.0f;
    debugprintf("env.ref_lon      : %f\r\n", env.ref_lon);// = 0.0f;
    debugprintf("env.ref_flag     : %d\r\n", env.ref_flag);// = 0;		// 기준위치 설정유무
    debugprintf("env.ref_distance : %d\r\n", env.ref_distance);// = 10;	// 1km // 1(100m)~100(10km)
    debugprintf("env.ref_shock    : %d\r\n", env.ref_shock);// = 180;	// 0 ~ 999
    debugprintf("env.mode         : %d\r\n", env.mode);// = 2;			// 지진모드= 2: 해제		3: 설정
}



void env_set_pos(double a_lat, double a_lon)
{
	env.ref_lat = a_lat;
	env.ref_lon = a_lon;
	env.ref_flag = 1;
	debugprintf("env: set_pos (%f, %d)\r\n", a_lat, a_lon);
	env_save();
}

void env_set_distance(int a_dist)
{
	env.ref_distance = a_dist;
	debugprintf("env: set_distance (%d)\r\n", a_dist);
	env_save();
}

void env_set_interval(int a_dist)
{
	env.interval = a_dist;
	debugprintf("env: set_interval (%d)\r\n", a_dist);
	env_save();
}

void env_set_shock(int a_shock)
{
	env.ref_shock = a_shock;
	debugprintf("env: set_shock (%d)\r\n", a_shock);
	env_save();
}

void env_set_mode(int a_mode)
{
	env.mode = a_mode;
	debugprintf("env: set_mode (%d)\r\n", a_mode);
	env_save();
}

void env_set_sensor(int a_sid, int a_val)
{
	//u8 cmd;
	u8 sno;


	switch (a_sid)
	{
		case '0': env.s0 = a_val;		break;
		case '1': env.s1 = a_val;		break;
		case '2': env.s2 = a_val;		break;
		case '3': env.s3 = a_val;		break;
		case '4': env.s4 = a_val;		break;
		case '5': env.s5 = a_val;		break;
		case '6': env.s6 = a_val;		break;
		case '7': env.s7 = a_val;		break;
		case '8': env.s8 = a_val;		break;
		case '9': env.s9 = a_val;		break;
		case 'A': env.sa = a_val;		break;
		case 'B': env.sb = a_val;		break;
		case 'C': env.sc = a_val;		break;
		case 'D': env.sd = a_val;		break;
		case 'E': env.se = a_val;		break;
		case 'F': env.sf = a_val;		break;
		case 'R': env.sr = a_val;		break;
	}



	switch (a_sid)
	{
		case '0': sno = SYS_SENSOR_AIO;		break; // AIO			'0'
		case '1': sno = SYS_SENSOR_MOSE;	break; // MOSE G-1000	'1'
		case '3': sno = SYS_SENSOR_CT3919;	break; // CT3919		'3'
		case '5': sno = SYS_SENSOR_DCS;		break; // DCS-Z pulse	'5'
		case '7': sno = SYS_SENSOR_GPS;		break; // GPS			'7'
		case '8': sno = SYS_SENSOR_ATM1;	break; // ATM1			'8'
		case '9': sno = SYS_SENSOR_ATM2;	break; // ATM2			'9'
		case 'B': sno = SYS_SENSOR_HMP155;	break; // HMP155		'B'
		case 'C': sno = SYS_SENSOR_PTB210;	break; // PTB210		'C'
		case 'F': sno = SYS_SENSOR_SHOCK;	break; // 충돌			'F'
			//if ( (a_val)


		case '2': sno = SYS_SENSOR_RESERVED;break; // Reserved		'2'
		case '4': sno = SYS_SENSOR_RESERVED;break; // Reserved		'4'
		case '6': sno = SYS_SENSOR_RESERVED;break; // Reserved		'6'
		case 'D': sno = SYS_SENSOR_RESERVED;break; // Reserved		'D'
		//case 'A': sno = SYS_SENSOR_AIO;	break; // 온도/습도		'A'
		//case 'E': sno = SYS_SENSOR_AIO;	break; // 위험반경이탈	'E'
		//case 'R': sno = SYS_SENSOR_AIO;	break; // 시스템 Reset  'R'
	}



	switch (a_val)
	{
		case '0':	//reset
			sno = SYS_SENSOR_RESERVED;
			break;

		case '1':	//ON
		case '2':	//OFF
			break;
	}

}



void wifi_reset(void)
{
    (*R_GPOLOW(5)  |= (1<<2) );    delayms(10);
    (*R_GPOHIGH(5) |= (1<<2) );

    revive_wifi();
}


void sysWDT_init(void) 
{
	;
}

void sysWDT_CntClear(void) 
{
	;
}




/*
AIO			'0'
MOSE G-1000	'1'
Reserved	'2'
CT3919		'3'
Reserved	'4'
DCS-Z pulse	'5'
Reserved	'6'
GPS			'7'
ATM1		'8'
ATM2		'9'
온도/습도	'A'
HMP155		'B'
PTB210		'C'
Reserved	'D'
위험반경이탈'E'
충돌		'F'
시스템 Reset'R'
*/



