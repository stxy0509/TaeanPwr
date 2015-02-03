#pragma once


#ifdef __K_CLI_C__

#else

#endif

#define UartGetCh	iri_rcv_get

#define SH_PIN_CLK_H()		 (*R_GPOHIGH(6) |= (1<<3) )
#define SH_PIN_CLK_L()		 (*R_GPOLOW(6)  |= (1<<3) )
#define SH_PIN_DATA_H()		 (*R_GPOHIGH(6) |= (1<<4) )
#define SH_PIN_DATA_L()		 (*R_GPOLOW(6)  |= (1<<4) )
#define SH_PIN_DATA_INPUT()	 (*R_GPIDIR(6)  |= (1<<4) )
#define SH_PIN_DATA_OUTPUT() (*R_GPODIR(6)  |= (1<<4) )

#define SH_PIN_DATA_LEVEL()	((*R_GP6ILEV) & (1<<4))


void checkBattery(void);


void App_TestShInit ();
int task_cmdshell();
//int APP_TestShTask();




int cli_is_start(void);
int cli_is_use_pt100(void);
int cli_is_use_bmp850(void);



float get_t_offset(void);
int get_p_offset(void);
s32 get_t_coef_00(void);
s32 get_t_coef_40(void);


void restore_status(void);

void decFwUpadateWaitTime(void);
int get_fgFW_updating(void);

void Task_cat(void);

void revive_wifi(void);
int dec_wifi_reset_tm(void);
#define WIFI_ON_TM      20*60  //10min

