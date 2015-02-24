/*============================================
	Simple Command Line Test Shell

	08/27/04	mdj, created
============================================*/
#define __K_CLI_C__

#define APP_DBG_PREFIX		""

#include "k_includes.h"



#define MAX_LEN_CMD_STR		100      //100
#define MAX_NO_ARGV			10       //10
#define MAX_NO_CMD_REG		50      //20


static int cli_start = 1;       //1: start , 0: stop
static int cli_use_pt100 = 1;
static int cli_use_bmp850 = 0;

static float t_offset = 0;
static float p_offset = 0;
static s32 t_coef_00 = 1058;
static s32 t_coef_40 = 2865;

char fwbuf[1024*512];

void measure_BAT_leval(void);
void set_ch4_switch(int a_onoff);


int roundToInt(float x) {
  if (x >= 0) return (int) (x + 0.5);
  return (int) (x - 0.5);
}

float get_t_offset(void)
{
    return t_offset;
}
int get_p_offset(void)
{
    int tmp;
    tmp = roundToInt(p_offset*100.0);
    return tmp;
}

s32 get_t_coef_00(void)
{
    return t_coef_00;
}
s32 get_t_coef_40(void)
{
    return t_coef_40;
}


void set_t_offset(float a_off)
{
    t_offset = a_off;
}
void set_p_offset(float a_off)
{
    p_offset = a_off;
}
void set_t_coef_00(int a_off)
{
    t_coef_00 = a_off;
}
void set_t_coef_40(int a_off)
{
    t_coef_40 = a_off;
}

extern u32 d1_bat_level;
void cmd_chkBat(void)
{
    //checkBattery();
    //debugprintf("\r\n bat( %d )\r\n",d1_bat_level);
    debugprintf("\r\n bat\r\n");
}




int cli_is_start(void)
{
    return cli_start;
}

int cli_is_use_pt100(void)
{
    return cli_use_pt100;
}

int cli_is_use_bmp850(void)
{
    return cli_use_bmp850;
}


static inline void SKIP_ONE_WORD(char **str)
{
	while (**str != ' ' && **str != 0)	 (*str)++;
	while (**str == ' ' && **str != 0)	 (*str)++;
}

typedef struct
{
	char *cmd;						// short command string
	void (*fn)(int argc, char *args);		// function pointer to call to process command
	char *help_str;					// help string to be printed by "help" command
}
cmd_registry_t;

//static tTaskID thTestSh;

static int cmd_cnt=0;
static cmd_registry_t cmd_registry[MAX_NO_CMD_REG];

static char cmd_str[MAX_LEN_CMD_STR];
// static char cmd_str[MAX_LEN_CMD_STR];
static int argc;
//static int argv[MAX_NO_ARGV];
static char args[MAX_NO_ARGV][20];

//static char *format_str[] = {"%x", "%d"};
//static int format_str_index = 0;


/*
static char *get_argv(char *cmd_str)
{
	int i;
	char *pstr;

	pstr = cmd_str;

	SKIP_ONE_WORD(&pstr);

	for (i=0; i<MAX_NO_ARGV; i++)
	{
		if (*pstr == 0)	return 0;

		//sscanf(pstr, format_str[format_str_index], &argv[i]);
		argv[i] = (int)strtol (pstr, (char **)NULL, format_str_index==0 ? 16 : 10);

		argc++;
		SKIP_ONE_WORD(&pstr);
	}

	return pstr;
}
*/


static char *get_args(char *cmd_str)
{
	char *pstr;

    //char p_data[10][10];
    int i = 0;
    int j = 0;
    //int k = 0;
    int spliter = 0;
    char ch;
    argc = 0;

	pstr = cmd_str;

    while (1)
    {
           ch = *pstr;  // a_str[k++];

           if ((ch== 0x0D) || (ch== 0x0A) || ch=='\0')
           {
               args[i][j] = '\0';
               //debugprintf("%s\r\n", args[i]);
               argc = (i+1);
               //PRINTVAR(argc);
               break;
           }
           else if (ch==' ')    // space
           {
               if (spliter == 0)
               {
                   spliter = 1;
                   args[i][j] = '\0';
                   //debugprintf("%s\r\n", args[i]);
                   i++;
                   j=0;
               }
           }
           else
           {
               spliter = 0;
               args[i][j++] = ch;
           }
           pstr++;
    }
	return pstr;
}




#if 0
static int get_cmd_org()
{
	int i;

	while (1)
	{
		debugstring ("CMD> ");
		fgets (cmd_str, MAX_LEN_CMD_STR, stdin);

		//if(cmd_str[0] == 0) continue;
		if (cmd_str[0]==0xa && cmd_str[1]==0) continue;

		for (i=0; i<cmd_cnt; i++)
		{
			if (strncmp(cmd_str, cmd_registry[i].cmd, strlen(cmd_registry[i].cmd)) == 0)
			{
				get_argv(cmd_str);
				return i;
			}
		}
	}
}
#endif

void wifistring ( const char *str )
{
    while ( *str )
    {
        // uart_putch ( 4, *str );
        uart_putch ( 0, *str );
        str++;
    }
}

void wifiprintf ( const char *rsp, ... )
{
    short   int len = 0;
    char print_buf[512];
    va_list args;

    if  ( rsp )
    {
        va_start ( args, rsp );
        len = vsnprintf ( print_buf, sizeof ( print_buf ), rsp, args );
        va_end ( args );
    }

    wifistring ( print_buf );
}




static int fgFW_updating = 0;
int get_fgFW_updating(void) {   return (fgFW_updating); }

static int fwUpadateWaitTime;
void setFwUpadateWaitTime(void)
{
// PRINTLINE;
// debugprintf("%s , %d \r\n",__FILE__,__LINE__);
    fwUpadateWaitTime = 10;
}
int getFwUpadateWaitTime(void)
{
    return(fwUpadateWaitTime);
}
void decFwUpadateWaitTime(void)
{
    if (fwUpadateWaitTime>0)
    {
// debugprintf("%s , %d \r\n",__FILE__,__LINE__);
// PRINTLINE;
        fwUpadateWaitTime--;
    }
}

static int fileSeletionID = 0;
static int fileTransfer = 0;
static int transFsize = 0;
static int transFidx = 0;
static int transTmpCnt = 0;

#ifdef FLAG_FOR_DEBUGGING
static int tst_index = 0;
#endif

//-----------------------------------------------------------
static int wifi_reset_tm = WIFI_ON_TM;
void revive_wifi(void)
{
    wifi_reset_tm = WIFI_ON_TM;
}
int dec_wifi_reset_tm(void)
{
    if (wifi_reset_tm > 0)
    {
        wifi_reset_tm--;
    }
    return wifi_reset_tm;
}
//-----------------------------------------------------------





static void get_cmd()
{

    static char fwbuf[1024*512];
    static int idx_getCmd = 0;
    static int idx_cmdStr = 0;
    static int fgFW_updateMode = 0;
    int i;
    char ch;

    static u32 rcv_cnt = 0;
    static u32 rcv_cnt1 = 0;


	// static int idx_getCmd = 0;
	// static int idx_cmdStr = 0;
	// int i;
	// char ch;

	switch (idx_getCmd)
	{
		case 0x00:
			//debugstring ("CMD>");
			idx_cmdStr = 0;
			idx_getCmd = 0x10;
			break;

		case 0x10:
			//get 1byte until meet 0x0D...
			if (uart_getch(0,&ch)) //receive
			{
                //uart_putch(0,ch);
                //sb_uart_putch ( SB_S_ATM2, ch );

				if (idx_cmdStr < MAX_LEN_CMD_STR-3)
				{
					if (ch==0x0D)
					{
						cmd_str[idx_cmdStr++] = 0x0D;
						cmd_str[idx_cmdStr++] = 0x0A;
						cmd_str[idx_cmdStr++] = 0;

						if (cmd_str[1]==0xa && cmd_str[2]==0)
						{
							idx_getCmd = 0x99;
						}
                        else
						{

                            //PRINTLINE;
							idx_getCmd = 0x20;
							//debugstring(cmd_str);
						}
					}
                    else if ( (ch== '#') )
                    {
                        int wifi_cmd = 0;
                        int i=0;
                        cmd_str[idx_cmdStr++] = '\0';

                        wifi_cmd= atoi(&cmd_str[0]);
                        debugprintf("command(%s): %d\r\n", cmd_str, wifi_cmd);
                        wifiprintf("command(%s): %d\r\n", cmd_str, wifi_cmd);


                        idx_getCmd = 0x99;

                        if ( (wifi_cmd>=6000) && (wifi_cmd<=6255) )
                        {
                            i = wifi_cmd;
                            wifi_cmd = 6000;
                        }

                        switch(wifi_cmd)
                        {
                            // LOG ON
                            case 5000:
                                break;
                            // LOG OFF
                            case 5001:
                                break;
                            // 전송주기
                            case 5501:  env_set_interval(10);   break;
                            case 5502:  env_set_interval(20);   break;
                            case 5503:  env_set_interval(30);   break;
                            case 5504:  env_set_interval(60);   break;
                            case 5505:  env_set_interval(120);  break;
                            // 기준위치 해제
                            case 5551:
                                env.ref_flag = 0;
                                env_save();
                                // debugstring("env.ref_flag is cleared.\r\n");
                                wifistring("env.ref_flag is cleared.\r\n");
                                break;
                            // TRBM RESET
                            case 5800:
                                break;
                            // TRBM Tync
                            case 5802:
                                {
                                    char tmp_buf[25];

                                    wifistring("TRBM timesync with RTC\r\n");
                                    // sprintf(tmp_buf,"%%+T1%02d%02d%02d%02d%02d%02d\r",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
                                    // sb_printstring(SB_S_ATM1, tmp_buf);
                                    // sb_printstring(SB_S_ATM2, tmp_buf);
                                    // sb_uart_putch(SB_S_ATM2, 0x13);
                                    // sb_uart_putch(SB_S_ATM2, 0x10);
                                    wifistring(tmp_buf);
                                }
                                break;
                            // 충격량 설정
                            case 6000:
                                env_set_shock(i-6000);
                                wifiprintf("env.shock(%d) is set.\r\n", i-6000);
                                break;

                            // 시스템 리셋
                            case 8001:
                                // debugstring("system reset.\r\n");
                                // wifistring("system reset.\r\n");
                                // delayms(100);
                                // cmdSensorControl('8', '0');
                                break;

                            // data retrieve
                            case 3001:  fileTransfer = 1;
                            case 2001:
                                fileSeletionID = 1;
                                idx_getCmd = 0x50;
                                break;
                            case 3002:  fileTransfer = 1;
                            case 2002:
                                fileSeletionID = 2;
                                idx_getCmd = 0x50;
                                break;
                            case 3003:  fileTransfer = 1;
                            case 2003:
                                fileSeletionID = 3;
                                idx_getCmd = 0x50;
                                break;
                            case 3004:  fileTransfer = 1;
                            case 2004:
                                fileSeletionID = 4;
                                idx_getCmd = 0x50;
                                break;
                            case 3005:  fileTransfer = 1;
                            case 2005:
                                fileSeletionID = 5;
                                idx_getCmd = 0x50;
                                break;
                            case 3006:  fileTransfer = 1;
                            case 2006:
                                fileSeletionID = 6;
                                idx_getCmd = 0x50;
                                break;
                            case 3007:  fileTransfer = 1;
                            case 2007:
                                fileSeletionID = 7;
                                idx_getCmd = 0x50;
                                break;
                            case 3008:  fileTransfer = 1;
                            case 2008:
                                fileSeletionID = 8;
                                idx_getCmd = 0x50;
                                break;
                            case 3009:  fileTransfer = 1;
                            case 2009:
                                fileSeletionID = 9;
                                idx_getCmd = 0x50;
                                break;
                            case 3010:  fileTransfer = 1;
                            case 2010:
                                fileSeletionID = 10;
                                idx_getCmd = 0x50;
                                break;
                            case 3011:  fileTransfer = 1;
                            case 2011:
                                fileSeletionID = 11;
                                idx_getCmd = 0x50;
                                break;
                            // case 2012:
                            //     fileSeletionID = 12;
                            //     idx_getCmd = 0x50;
                            //     break;

                            // FW update
                            case 9999:
                                wifistring("FW update mode...\r\n");
                                // deugstring("FW update mode...\r\n");
                                fgFW_updateMode = 1;

                                // set_debug_channel(0);

                                idx_getCmd = 0x40;
                                break;





                        }
                        // idx_getCmd = 0x99;

                    }

                    else if (ch==0x0A)
					{
                    }
                    else if (ch==0x08)    //BS
                    {
                        //if (idx_cmdStr>0)
                        //{
                        //    idx_cmdStr--;
                        //}
					}
                    else
					{
						cmd_str[idx_cmdStr++] = ch;
					}
				} else
				{
					idx_getCmd = 0x99;
					debugstring("input string is over MAX_LEN_CMD_STR.\r\n");
					debugstring("the string is discarded.\r\n");
				}
			}
			break;

		case 0x20:
			for (i=0; i<cmd_cnt; i++)
			{
                //PRINTVAR(i);

				if (strncmp(cmd_str, cmd_registry[i].cmd, strlen(cmd_registry[i].cmd)) == 0)
				{
					//get_argv(cmd_str);
                    //PRINTLINE;
					get_args(cmd_str);

					//cmd_registry[i].fn(argc, argv);
					cmd_registry[i].fn(argc, args);
                    argc = 0;
                    {
                        int j;
                        for (j=0; j<MAX_NO_ARGV; j++)
                        {
                            //argv[i] = 0;
                            args[j][0] = '0';
                        }
                    }

				}
			}
			idx_getCmd = 0x99;
			break;


        case 0x40:
            // FW update mode...
// debugprintf("%s , %d \r\n",__FILE__,__LINE__);
            fgFW_updating = 1;

            setFwUpadateWaitTime();
            idx_getCmd = 0x42;
            break;
        case 0x42:
            fgFW_updating = 1;

            // FW update mode...
            if (getFwUpadateWaitTime()==0)
            {
// debugprintf("%s , %d \r\n",__FILE__,__LINE__);
                wifistring("\r\nexit FW update mode...\r\n");
                idx_getCmd = 0x99;
            }
            else
            {
                if (uart_getch(0,&ch)) //receive
                {
                    wifi_reset_tm = WIFI_ON_TM;

                    if (ch=='#')
                    {
                        // wifistring("'0xC0' received (START SIGNAL)...\r\n");
                        rcv_cnt = 0;
                        rcv_cnt1 = 0;

                        idx_getCmd = 0x44;

                        // enable_interrupt(INTNUM_EIRQ0, FALSE);
                        // enable_interrupt(INTNUM_EIRQ1, FALSE);

                        setFwUpadateWaitTime();
                        debugprintf("\r\nstart\r\n00000: ");
                        wifiprintf("\r\nstart\r\n00000: ");


                    }
                }
            }
            break;
        case 0x44:
            fgFW_updating = 1;

            if (getFwUpadateWaitTime()==0)
            {
// PRINTLINE;
                for (i=0;i<1024;i++)
                {
                    if (uart_getch(0,&ch)) //receive
                    {
                        wifi_reset_tm = WIFI_ON_TM;
                    }
                    else
                        {   break;  }
                }

                wifistring("\r\nexit FW update mode...\r\n");

                debugprintf("rcv_cnt= %d (273*1024)\r\n", rcv_cnt);  // 279552
                // debugprintf("rcv_cnt= %d (358400)\r\n", rcv_cnt);
                // wifiprintf("rcv_cnt= %d (358400)\r\n", rcv_cnt);



                // if (rcv_cnt == 358400)
                if (rcv_cnt == 273*1024)
                {
                    //extern const unsigned int bootloader_trip_elf_bin_len;
                    int i;
                    int fileSize_bak = 273*1024;
                    // int sectors = bootloader_trip_elf_bin_len/flash_get_sectorsize();
                    int sectors = fileSize_bak/flash_get_sectorsize();
                    // if(bootloader_trip_elf_bin_len % flash_get_sectorsize())
                    if(fileSize_bak % flash_get_sectorsize())
                        sectors++;
                    // wifistring("erase application sectors...\r\n");
                    debugstring("erase application sectors...\r\n");
                    for(i=20;i<(sectors+20);i++)
                    {
                        // debugprintf("%d sector\r\n",i);
                        // wifistring(".");
                        debugstring(".");

                        flash_erase_sector(i,1);
                    }
                    // wifistring("\r\nupdate....");
                    debugstring("update....\r\n");
                    // flash_write(0,(BYTE*)bootloader_trip_elf_bin,bootloader_trip_elf_bin_len);
                    flash_write(0x14000,(BYTE*)fwbuf,fileSize_bak);
                    // wifistring("completed\r\n");
                    debugstring("completed\r\n");
                }
                else
                {
                    // wifistring("FW update canceled...\r\n");
                    debugstring("FW update canceled...\r\n");

                }


                // enable_interrupt(INTNUM_EIRQ0, TRUE);   // 131008
                // enable_interrupt(INTNUM_EIRQ1, TRUE);

                idx_getCmd = 0x99;
            }
            else
            {
                int  i;
                // for (i=0;i<512;i++)
                for (i=0;i<4000;i++)
                {
                    if (uart_getch(0,&ch)) //receive
                    {
                        wifi_reset_tm = WIFI_ON_TM;

                        // if (ch==0xC0)
                        // {
                        //     wifiprintf("%04X ( %d ) ", rcv_cnt,rcv_cnt);
                        //     wifistring("'0xC0' received (END SIGNAL)...\r\n");
                        //     idx_getCmd = 0x99;
                        // }
                        // else
                        {
                            // debugprintf("%02X ", (u8)ch);
                            // wifiprintf("%02X ", (u8)ch);
                            //debugstring(".");
                            //wifistring(".");
                            fwbuf[rcv_cnt++] = ch;
                            if (++rcv_cnt1 > 15)
                            {
                                rcv_cnt1 = 0;
                                // debugprintf("\r\n%05X: ",rcv_cnt);
                                // wifiprintf("\r\n%05X: ",rcv_cnt);
                                // debugstring(".");
                                wifiprintf("\r\n%05X",rcv_cnt);
                            }
                            //rcv_cnt++;

                            // if (rcv_cnt >(512*1024))
                            //     idx_getCmd = 0x46;

                        }
                        setFwUpadateWaitTime();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            break;

        case 0x46:
            debugstring("verify start\r\n");
            idx_getCmd = 0x99;
            break;

        case 0x50:
            if (fileTransfer==1)
            {
                // 파일전송모드로...
                transTmpCnt = 0;
                transFsize = 0;
                transFidx = 0;
                idx_getCmd = 0x60;

#ifdef FLAG_FOR_DEBUGGING
                tst_index = 0;
#endif
                break;
            }

            {
                FRESULT  res;
                FIL fp;

                char t_item[100];
                char buf[1460*3];
                int len;
                char *c;

                int fileSize;
                int nRead;
                //int i;
                int position = 0;
                //int tmp_idx = 0;

                c = &buf[0];

                switch(fileSeletionID)
                {

                    case 1: strcpy(t_item,"1:ai.dat");  break;
                    case 2: strcpy(t_item,"1:ct.dat");  break;
                    case 3: strcpy(t_item,"1:zp.dat");  break;
                    case 4: strcpy(t_item,"1:t1.dat");  break;
                    case 5: strcpy(t_item,"1:t2.dat");  break;
                    case 6: strcpy(t_item,"1:hm.dat");  break;
                    case 7: strcpy(t_item,"1:pt.dat");  break;
                    case 8: strcpy(t_item,"1:sh.dat");  break;
                    case 9: strcpy(t_item,"1:gp.dat");  break;
                    case 10: strcpy(t_item,"1:mo.dat");  break;
                    case 11: strcpy(t_item,"1:se.dat");  break;
                }

                debugprintf("t_item(%s)\r\n",t_item);

                res = f_open(&fp,t_item, FA_READ|FA_OPEN_EXISTING);   //delayms(5);
                if (res != FR_OK)
                {
                    debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
                    idx_getCmd = 0x99;
                    break;
                }

                fileSize = f_size(&fp);
                debugprintf("fileSize( %d )\r\n", fileSize);


                if (fileSize >(1460*2-1))
                {
// PRINTLINE;
                    len = (1460*2)-1;
                    position = fileSize - (1460*2-1);
                }
                else
                {
// PRINTLINE;
                    len = fileSize;
                    position = 0;
                }

// PRINTLINE;
                f_lseek(&fp, position);   //delayms(5);

// PRINTLINE;
                f_read(&fp,c,len,&nRead);
                f_close(&fp);

                buf[len]='\0';

// PRINTLINE;
                debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
                // debugstring(buf);
                wifistring(buf);
// PRINTLINE;

            }
            idx_getCmd = 0x99;
            break;

        case 0x60:
            {
                //static int tmp_cnt = 0;

                FRESULT  res;
                FIL fp;

                char t_item[100];
                char buf[1460*3];
                int len;
                char *c;

                //int fileSize;
                int nRead;
                //int i;
                int position = 0;
                //int tmp_idx = 0;

                c = &buf[0];

                switch(fileSeletionID)
                {

                    case 1: strcpy(t_item,"1:ai.dat");  break;  // aio
                    case 2: strcpy(t_item,"1:ct.dat");  break;  // aio
                    case 3: strcpy(t_item,"1:zp.dat");  break;  // aio
                    case 4: strcpy(t_item,"1:t1.dat");  break;  // aio
                    case 5: strcpy(t_item,"1:t2.dat");  break;  // aio
                    case 6: strcpy(t_item,"1:hm.dat");  break;  // aio
                    case 7: strcpy(t_item,"1:pt.dat");  break;  // aio
                    case 8: strcpy(t_item,"1:sh.dat");  break;  // aio
                    case 9: strcpy(t_item,"1:gp.dat");  break;  // aio
                    case 10: strcpy(t_item,"1:mo.dat");  break;  // aio
                    case 11: strcpy(t_item,"1:se.dat");  break;  // aio
                }

                debugprintf("t_item(%s)\r\n",t_item);

                res = f_open(&fp,t_item, FA_READ|FA_OPEN_EXISTING);   //delayms(5);
                if (res != FR_OK)
                {
                    debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
                    fileTransfer = 0;
                    transFsize = 0;
                    transFidx = 0;
                    idx_getCmd = 0x99;
                    break;
                }

                //fileSize = f_size(&fp);
                transFsize = f_size(&fp);
                debugprintf("Trans fileSize( %d, %d)\r\n", transFsize, transFidx);

                if (transFidx >= transFsize)
                {
                    // 전송완료...
                    fileTransfer = 0;
                    transFsize = 0;
                    transFidx = 0;

                    idx_getCmd = 0x99;
                    break;
                }

                // 전송한다...
                position = transFidx;

                if ((transFsize-transFidx) >(1460))
                {
// PRINTLINE;
                    // len = (1460);
                    len = (1460);
                    transFidx += len;
                }
                else
                {
// PRINTLINE;
                    len = transFsize-transFidx;

                    fileTransfer = 0;
                    transFsize = 0;
                    transFidx = 0;
                    idx_getCmd = 0x99;

                }

// PRINTLINE;
                f_lseek(&fp, position);   //delayms(5);

// PRINTLINE;
                f_read(&fp,c,len,&nRead);
                f_close(&fp);

                buf[len]='\0';

// PRINTLINE;
                debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
                // debugstring(buf);

// #ifdef FLAG_FOR_DEBUGGING
//                 sprintf(buf,"%07d: 123456789abcdefghijklmnopqrstuvwxyz_123456789abcdefghijklmnopqrstuvwxyz\r\n",tst_index++);
// #endif
                wifistring(buf);

                if (fileTransfer>0)
                {
                    tick_fileTx=3;  //1s
                    idx_getCmd = 0x61;
                    break;




                    if(++transTmpCnt >300)
                    {
                        transTmpCnt = 0;
                    }
                    else
                    {
                        switch(transTmpCnt)
                        {
                            case 25:
                            case 75:
                            case 125:
                            case 175:
                            case 225:
                            case 275:
                                tick_fileTx=100;  //1s
                                break;
                            case 50:
                            case 100:
                            case 150:
                            case 200:
                            case 250:
                            case 300:
                                tick_fileTx=300;  //1s
                                break;
                        }

                    }
                    idx_getCmd = 0x61;
                }
            }
            //idx_getCmd = 0x99;
            break;

        case 0x61:
            if (tick_fileTx==0)
            // if (uart_getch(4,&ch)) //receive

            {
                idx_getCmd = 0x60;
            }
            break;


		case 0x99:
		default:
			idx_getCmd = 0x00;
			break;
	}
}




int task_cmdshell()
{
    get_cmd();
	// get_cmd4();
	return 0;
}

void testSh_RegisterCmd (char *cmd, void (*fn)(int argc, int *argv), char *help_str)
{
	cmd_registry[cmd_cnt].cmd = (char *)malloc(strlen(cmd)+1);
	strcpy(cmd_registry[cmd_cnt].cmd, cmd);

	cmd_registry[cmd_cnt].fn = fn;

	cmd_registry[cmd_cnt].help_str = (char *)malloc(strlen(help_str)+1);
	strcpy(cmd_registry[cmd_cnt].help_str, help_str);

	cmd_cnt++;
}




static void cmd_help()
{
	int i;

	for (i=0; i<(cmd_cnt); i++)
	{
		debugstring (cmd_registry[i].help_str);
	}
}


static void cmd_pos()
{
    // double p1x, p1y, p2x, p2y;
    // double dist;

    // if (argc<5)
    // {
    //     debugstring ("  need <5 value>\r\n");
    //     return;
    // }

    // debugprintf("\r\np1x= %s, p1y= %s, p2x= %s, p2y= %s", args[1], args[2], args[3], args[4]);
    // p1x = atof(args[1]);
    // p1y = atof(args[2]);
    // p2x = atof(args[3]);
    // p2y = atof(args[4]);
    // debugprintf("\r\np1x= %f, p1y= %f, p2x= %f, p2y= %f", p1x, p1y, p2x, p2y);

    // dist = pos_distance(p1x, p1y, p2x, p2y);
    // debugprintf("\r\ndist= %f", dist);

}

void cmd_sbr(void)
{}

void cmd_sbw(void)
{}


static void cmd_at(void)
{
    // print_string (3, "AT\r\n");
}

static void cmd_mv(void)
{
    if (argc<2)
    {
        debugstring ("  mv <mo zp ai all>\r\n");
        return;
    }

    if ( (!strncmp( args[1], "mo", 2)))
    {
        SensorBakSize.b.mose = 1;
    }

    if ( (!strncmp( args[1], "zp", 2)))
    {
        SensorBakSize.b.dcs = 1;
    }
    if ( (!strncmp( args[1], "ai", 2)))
    {
        SensorBakSize.b.aio = 1;
    }
    if ( (!strncmp( args[1], "all", 3)))
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
    chk_file_size();
}


static void cmd_gg(void)
{
    // debugstring("hello wifi???");
}


static void cmd_qq(void)
{
    // (*R_GPOLOW(5)  |= (1<<2) );
    // delayms(10);
    // (*R_GPOHIGH(5) |= (1<<2) );
}

static void cmd_ss(void)
{
}

static void cmd_uu(void)
{
    // int a_chan = get_debug_channel();

    // if (a_chan == 0)
    // {
    //     // set_debug_channel(4);
    //     // wifistring("debug out: wifi\r\n");
    //     // debugstring("debug out: wifi\r\n");
    // }
    // else
    // {
    //     // set_debug_channel(0);
    //     // wifistring("debug out: serial\r\n");
    //     // debugstring("debug out: serial\r\n");
    // }
}
// static void cmd_msg(void)
// {
//     char buf[21];
//     if (argc<2)
//     {
//         debugstring ("  msg d10/d11\r\n");
//         return;
//     }

//     debugprintf("\r\nmsg : %s", args[1]);

//     if (strncmp( args[1], "d10", 3)==0)
//     {
//         make_msg_k1();
//     }
//     else if (strncmp( args[1], "d1105", 3)==0)
//     {
//         make_msg_press_5();
//     }
//     else if (strncmp( args[1], "d1130", 3)==0)
//     {
//         make_msg_press_30();
//     }
// }
static void cmd_xx(void)
{
    // if (get_ch4_switch())
    // {
    //     debugstring("wifi debug message OFF\r\n");
    //     set_ch4_switch(0);
    // }
    // else
    // {
    //     debugstring("wifi debug message ON\r\n");
    //     set_ch4_switch(1);
    // }

}
static void cmd_yy(void)
{
}
static void cmd_zz(void)
{
}


static void cmd_env(void)
{
    //char buf[21];
    if (argc<2)
    {
        debugstring ("  env <[init] [save] [read]>\r\n");
        return;
    }

    debugprintf("\r\nenv : %s", args[1]);

    if (strncmp( args[1], "init", 4)==0)
    {
        //init
        env_init();
    }
    else if (strncmp( args[1], "save", 4)==0)
    {
        env_save();
    }
    else if (strncmp( args[1], "read", 4)==0)
    {
        env_read();
    }
    else if ( (!strncmp( args[1], "print", 4)) ||
              (!strncmp( args[1], "pr", 2)) )
    {
        env_print();
    }
}

static void cmd_ts(void)
{
    // //char buf[21];
    // if (argc<2)
    // {
    //     debugstring ("  ts <10,11(CR),12(CRLF),13(CRLF2)/20,21,22,23>\r\n");
    //     return;
    // }

    // debugprintf("\r\nts : %s", args[1]);

    // if (strncmp( args[1], "10", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     // sb_printstring(SB_S_ATM1, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "11", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d\r",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     // sb_printstring(SB_S_ATM1, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "12", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d\r\n",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     // sb_printstring(SB_S_ATM1, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "13", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d\r\n\r\n",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     // sb_printstring(SB_S_ATM1, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "20", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     sb_printstring(SB_S_ATM2, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "21", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d\r",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     sb_printstring(SB_S_ATM2, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "22", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d\r\n",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     sb_printstring(SB_S_ATM2, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "23", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r\n%%+T1%02d%02d%02d%02d%02d%02d\r\n\r\n",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     sb_printstring(SB_S_ATM2, tmp_buf);
    //     debugstring(tmp_buf);
    // }
    // else if (strncmp( args[1], "24", 2)==0)
    // {
    //     char tmp_buf[50];

    //     sprintf(tmp_buf,"\r");//,rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);
    //     sb_printstring(SB_S_ATM2, tmp_buf);
    //     debugstring(tmp_buf);
    // }
}

void SHT7x_Transmission_Start();
u32 SHT7x_Write(unsigned char a_data);   //read=1, write = 0
int  SHT7x_ACK_Delay_Check();
void SHT7x_Soft_Reset();
void SHT7x_Connection_Reset_Sequence();
unsigned char SHT7x_Read(int ack);
void SHT7x_init();



static void cmd_msg(void)
{
//     //char buf[21];
//     if (argc<2)
//     {
//         debugstring ("  msg <[init] [save] [read]>\r\n");
//         return;
//     }

//     debugprintf("\r\nmsg : %s", args[1]);

//     if (strncmp( args[1], "k0", 2)==0)
//     {
//         make_msg_kx('0');
//     }
//     else if (strncmp( args[1], "k1", 2)==0)
//     {
//         make_msg_k1();
//     }
//     else if (strncmp( args[1], "kx", 2)==0)
//     {
//         make_msg_kx('9');
//     }
//     else if (strncmp( args[1], "ky", 2)==0)
//     {
//         make_msg_ky('E',0);
//     }
//     else if (strncmp( args[1], "d0", 2)==0)
//     {
//         make_msg_kx('0');
//     }
//     else if (strncmp( args[1], "d1", 2)==0)
//     {
//         make_msg_k1();
//     }
// #if (BUOY_SPEC == 'D')

//     else if (strncmp( args[1], "d5", 2)==0)
//     {
//         make_msg_press_5();
//     }
//     else if (strncmp( args[1], "d30", 2)==0)
//     {
//         make_msg_press_30();
//     }
// #endif
}



static void cmd_syscmd(void)
{
}

static void cmd_set(void)
{
    //char buf[21];
    if (argc<2)
    {
        debugstring ("  set <arg...>\r\n");
        return;
    }

    //debugprintf("\r\nenv : %s", args[1]);

    if ( (!strncmp( args[1], "heartbeat", 9)) ||
         (!strncmp( args[1], "hb", 2)) )
    {
        // enable,disable Heartbeat
        if (argc<3)
        {
            debugstring ("  ex) set heartbeat ['0'||'1']\r\n");
            return;
        }
        switch(args[2][0])
        {
            // case '0':   setEnableHeartbeat(0);  break;
            // case '1':   setEnableHeartbeat(1);  break;
        }
    }
    else if ( (!strncmp( args[1], "env", 3)))
    {
        if ( (!strncmp( args[2], "interval", 8)) ||
             (!strncmp( args[2], "int", 3)) )
        {
            if (argc<4)
            {
                debugstring ("  ex) set env interval(int) [10||20||30||60||120]\r\n");
                return;
            }
            {
                int k = atoi(args[3]);
                switch(k)
                {
                    case 5:
                    case 10:
                    case 20:
                    case 30:
                    case 60:
                    case 120:
                        env.interval = k;
                        debugprintf("set : env.interval = %d\r\n", k);
                        break;
                }
            }
        }
        else if ( (!strncmp( args[2], "mode", 4)) ||
             (!strncmp( args[2], "md", 2)) )
        {
            if (argc<4)
            {
                debugstring ("  ex) set env mode(md) [0|1]\r\n");
                return;
            }
            {
                int k = atoi(args[3]);
                switch(k)
                {
                    case 0:
                    case 1:
                        env.mode = k;
                        debugprintf("set : env.mode = %d\r\n", k);
                        break;
                }
            }
        }
        else if ( (!strncmp( args[2], "shock", 5)) ||
             (!strncmp( args[2], "shk", 3)) )
        {
            if (argc<4)
            {
                debugstring ("  ex) set env shock(shk) [value]\r\n");
                return;
            }
            {
                int k = atoi(args[3]);
                //switch(k)
                {
                    env.ref_shock = k;
                    debugprintf("set : env.ref_shock = %d\r\n", k);
                    //break;
                }
            }
        }
        else if ( (!strncmp( args[2], "port", 4)) /* || (!strncmp( args[2], "shk", 3)) */)
        {
            if (argc<4)
            {
                debugstring ("  ex) set env port [value]\r\n");
                return;
            }
            {
                int k = atoi(args[3]);
                //switch(k)
                {
                    env.port = k;
                    debugprintf("set : env.port = %04d\r\n", k);
                    //break;
                }
            }
        }
        else if ( (!strncmp( args[2], "id", 2)) /* || (!strncmp( args[2], "shk", 3)) */)
        {
            if (argc<4)
            {
                debugstring ("  ex) set env id [value]\r\n");
                return;
            }
            {
                int k = atoi(args[3]);
                //switch(k)
                {
                    env.id = k;
                    debugprintf("set : env.id = %03d\r\n", k);
                    //break;
                }
            }
        }
        else if ( (!strncmp( args[2], "ip", 2)) /* || (!strncmp( args[2], "shk", 3)) */)
        {
            if (argc<4)
            {
                debugstring ("  ex) set env ip [xxx.xxx.xxx.xxx]\r\n");
                return;
            }
            {
                //int k = atoi(args[3]);
                //switch(k)
                {
                    strcpy(env.ip, args[3]);
                    debugprintf("set : env.ip = %s\r\n", args[3]);
                    //break;
                }
            }
        }

    }
    else if ( (!strncmp( args[1], "sensor", 5)) ||
         (!strncmp( args[1], "s", 1)) )
    {
        char s_id = '0';
        char c_code = '1';

        if (argc<4)
        {
            debugstring ("  ex) set sensor s_no control_no\r\n");
            return;
        }
        else
        {
            s_id = args[2][0];
            c_code = args[3][0];

            switch(s_id)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'R':
                    switch(c_code)
                    {
                        case '0':
                        case '1':
                        case '2':
                            // ctrl_sensor(s_id, c_code);
                            // ctrl_sensor_set_env(s_id, c_code);
                            debugprintf("sensor control (%c, %c)\r\n", s_id, c_code);
                            break;
                    }
                    break;
            }
        }
    }

}

static void cmd_rtc(void)
{
    if (argc<2)
    {
        debugstring ("  rtc <arg...>\r\n");
        return;
    }

    // INIT
    if ( (!strncmp( args[1], "init", 4)))
    {
        debugprintf("rtcc init..\r\n");
        ini_rtcc();
        ini_time();
    }
    // PRINT
    else if ( (!strncmp( args[1], "print", 5)) ||
              (!strncmp( args[1], "pr", 2)))
    {
        printTimeTag();
    }
    else if ( (!strncmp( args[1], "sync", 4)) )
    {
        set_rtc_sysclk_sync_req(1);
    }
    // SETTIME
    else if ( (!strncmp( args[1], "settime", 6)) ||
              (!strncmp( args[1], "st", 2)))
    {
        rtcTime t1;

        t1.year = 2000+atoi(args[2]);
        t1.mon = atoi(args[3]);
        t1.day = atoi(args[4]);
        t1.hour = atoi(args[5]);
        t1.min = atoi(args[6]);
        t1.sec = atoi(args[7]);

        debugstring("set time\r\n");
        rtc_settime(&t1);
        printTimeTag();
    }
}

static void cmd_chdir(void)
{
    //float offset = 0;
    //float tmp = 0;
    char buf[21];
    if (argc<2)
    {
        debugstring ("  chdir <arg...>\r\n");
        return;
    }

    debugprintf("\r\nchdir : %s", args[1]);
    f_chdir(args[1]);
    f_getcwd(buf,20);
    debugprintf("\r\n%s", buf);
}


static void cmd_del(void)
{
    //float offset = 0;
    //float tmp = 0;
    //char buf[21];
    if (argc<2)
    {
        debugstring ("  del <arg...>\r\n");
        return;
    }

    debugprintf("\r\ndel : %s", args[1]);
    f_unlink(args[1]);
    // f_getcwd(buf,20);
    // debugprintf("\r\n%s", buf);
}

char* readfile(char* fname,int* len)
{
    FIL fp;
    int size;
    char* pngbuf;
    U32 nRead;
    FRESULT res = f_open(&fp,fname,FA_READ|FA_OPEN_EXISTING);
    if( res != FR_OK )
    {
        debugprintf("Cannot open : %s\r\n", fname);
        return 0;
    }
    size = f_size(&fp);
    pngbuf=malloc(size);
    if(pngbuf==0)
        return 0;
    f_read(&fp,pngbuf,size,&nRead);
    f_close(&fp);
    *len=size;
    return pngbuf;
}


u8 cat_fid = 0;
u8 cat_next = 0;

static void cmd_bak(void)
{/*
    if (argc<2)
    {
        cat_next= 1;
    }
    else
    {
        // u32 param = 99;
        cat_fid = atoi(args[1]);
        debugprintf("cat_fid = %d\r\n", cat_fid);
        // if (param == 0)
        // {
        //     cat_fid = 0;
        // }
    }
*/}


static void cmd_cat(void)
{
    // cat 1:mo<CR>
    {
        FRESULT  res;
        FIL fp;

        char t_item[100];
        char buf[1000*210];
        int len;
        char *c;

        int fileSize;
        int nRead;
        //int i;
        int position = 0;
        //int tmp_idx = 0;

        //int a_position = 0;
        int a_blockSize = 50;

        debugprintf("argc(%d)\r\n", argc);
        debugprintf("args[1](%s), [2](%s), [3](%s)\r\n", args[1], args[2], args[3]);

        // start position
        if (argc > 3)
        {
            // debugprintf("args[3](%s)\r\n", args[3]);
            a_blockSize = atoi(args[3]);
            debugprintf("a_blockSize( %d )\r\n", a_blockSize);
        }
        if (a_blockSize <= 0)
        {
            //error
            // debugprintf("error : a_blockSize( %d )\r\n", a_blockSize);
            wifiprintf("error : a_blockSize( %d )\r\n", a_blockSize);
            return;
        }
        else if (a_blockSize > 200)
        {
            //error
            // debugprintf("error : a_blockSize is too big( %d )\r\n", a_blockSize);
            wifiprintf("error : a_blockSize is too big( %d )\r\n", a_blockSize);
            return;
        }
        // debugprintf("a_blockSize( %d )\r\n", a_blockSize);
        // wifiprintf("a_blockSize( %d )\r\n", a_blockSize);



        if (argc < 3)
        {
            position = 0;
        }
        else
        {
            position = (atoi(args[2]) * (1000*a_blockSize));
        }
        // debugprintf("position( %d )\r\n", position);
        // wifiprintf("position( %d )\r\n", position);

        c = &buf[0];
        sprintf(t_item,"1:%s_EB.TXT",args[1]);
        // debugprintf("t_item(%s)\r\n",t_item);
        // wifiprintf("t_item(%s)\r\n",t_item);

        res = f_open(&fp,t_item, FA_READ|FA_OPEN_EXISTING);   //delayms(5);
        if (res != FR_OK)
        {
            // debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            wifiprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize = f_size(&fp);
        // debugprintf("fileSize( %d )\r\n", fileSize);
        // wifiprintf("fileSize( %d )\r\n", fileSize);

        // position = a_position;

        if (position >= fileSize)
        {
            // debugprintf("error: fileSize(%d) is less than start position(%d)\r\n", fileSize, position);
            wifiprintf("error: fileSize(%d) is less than start position(%d)\r\n", fileSize, position);
            return;
        }
#if 1
        if (fileSize == ((1000*a_blockSize)+position) )
        {

        }
        if (fileSize > ((1000*a_blockSize)+position) )
        {
            len = (1000*a_blockSize);
            // position = ((1000*50)+a_position);
        }
        else
        {
            len = fileSize - position;
            // position = 0;
        }
#else

        //----------------------20141013---
        a_blockSize = 20;

        // if (fileSize == ((1000*a_blockSize)) )
        // {

        // }
        if (fileSize >= ((1000*a_blockSize)) )
        {
            len = (1000*a_blockSize);
            position = fileSize - len;
            // position = ((1000*50)+a_position);
        }
        else
        {
            len = fileSize;
            position = 0;
        }
#endif
        f_lseek(&fp, position);   //delayms(5);

        f_read(&fp,c,len,&nRead);
        f_close(&fp);

        buf[len]='\0';


        sprintf(t_item,"%06d",len);

        // debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
        // wifiprintf("len( %d ), nRead( %d )\r\n", len, nRead);
        // debugstring(buf);
        wifistring(t_item);
        wifistring(buf);

    }
}


void Task_cat(void)
{/*
    static u32 idx=0;
    static u32 position=0;
    static u32 blockSize=5;
    static u32 count=10;
    static char buf[5000+1];

    // int fileSize;

    switch (idx)
    {
        case 0:
            if (cat_fid > 0 )
            {
PRINTLINE;
                position = 0;
                count=10;
                idx = 50;
            }
            break;

        case 50:
            switch (cat_fid)
            {
                case 1:
                    {
PRINTLINE;
                        // ai.dat
                        FRESULT  res;
                        FIL fp;
                        int nRead;
                        char *c;
                        char t_item[20];
                        u32 len = 1000;

                        c = &buf[0];

                        strcpy(t_item, "1:mo.dat");
                        res = f_open(&fp,t_item, FA_READ|FA_OPEN_EXISTING);   //delayms(5);
                        if (res != FR_OK)
                        {
                            wifiprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
                            cat_fid = 0;
                            idx = 0;
                        }
                        else
                        {

                            f_lseek(&fp, position);   //delayms(5);

                            f_read(&fp, c, len, &nRead);
                            f_close(&fp);

                            buf[len]='\0';

                            position += nRead;
                            idx = 100;
                        }
                    }
                    break;
                default:
                    idx = 0;
                    break;
            }
            break;

        case 100:
PRINTLINE;
            wifiprintf(buf);
            idx = 150;
            tick_cat = 10;
            break;

        case 150:
            // if (cat_next==1)
            if (tick_cat==0)
            {
                cat_next = 0;
                idx=50;
PRINTLINE;
           if (--count==0)
           {
               idx = 0;
               cat_fid = 0;
           }
        }
            break;

    }
*/}



static void cmd_fs(void)
{
    if (argc<2)
    {
        debugstring ("  fs <arg...>\r\n");
        return;
    }

    // ls, ll - list files in current folder.
    if ( (!strncmp( args[1], "ls", 2)) ||
         (!strncmp( args[1], "ll", 2)))
    {
        switch (argc)
        {
            case 2:
                print_files ("1:");
                break;
            case 3:
                {
                    // print_files ("1:");
                    char buf[30];

                    sprintf(buf,"1:/%s",args[2]);
                    // print_files (args[2]);
                    print_files (buf);
                }
                break;
        }
    }

    else if ( (!strncmp( args[1], "clear", 5)) ||
              (!strncmp( args[1], "clr"  , 3)))
    {
        sdc_unmount();
        sdc_clear(); 
    }


    // cpsr cp_sr - copy file from SD to RAM
    else if ( (!strncmp( args[1], "cp_sr", 5)) ||
              (!strncmp( args[1], "cpsr", 4)))
    {
        FRESULT  res;
        FIL fp;

        char t_item[100];
        char buf[1024*512];
        int len;
        char *c;

        int fileSize;
        int nRead;
        int i;
        int position = 0;
        int tmp_idx = 0;

        c = &buf[0];
        strcpy(t_item,"1:boot.bin");
        debugprintf("t_item(%s)\r\n",t_item);

PRINTLINE;

        res = f_open(&fp,t_item, FA_READ|FA_OPEN_EXISTING);   delayms(5);
        if (res != FR_OK)
        {
            debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize = f_size(&fp);
        debugprintf("fileSize( %d )\r\n", fileSize);

PRINTLINE;

        while (1)
        {
            if (fileSize >1024)
            {
                len = 1024;
                position += 1024;
            }
            else
            {
                len = fileSize;
                position += fileSize;
            }
            f_read(&fp,c,len,&nRead);
            c += nRead;
            f_lseek(&fp, position);   //delayms(5);

            debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
            debugprintf("[%04X] ", tmp_idx);
            for (i = 0; i < 16; ++i)
            {
                debugprintf("%02X ", (u8)buf[tmp_idx+i]);
            }
            debugstring("\r\n");
            tmp_idx += nRead;

            fileSize -= nRead;
            debugprintf("remain fileSize( %d )\r\n", fileSize);

            if ( fileSize <= 0 )
            {
                break;
            }
        }

PRINTLINE;

        f_close(&fp);

        //debugprintf("size( %d ), nRead( %d )\r\n", size, nRead);

PRINTLINE;

        // *len=size;
        //return pngbuf;

        // //f_lseek(fp, (fp->fsize));   delayms(5);
        // sz_msg = strlen(a_msg);
        // f_write(fp, a_msg, sz_msg, &bw);   delayms(5);

        // *fsz = (u32)(fp->fsize);
        // f_close(fp);   delayms(5);


    }

    else if ( (!strncmp( args[1], "ai", 2)) ||
              (!strncmp( args[1], "ct", 2)) ||
              (!strncmp( args[1], "zp", 2)) ||
              (!strncmp( args[1], "t1", 2)) ||
              (!strncmp( args[1], "t2", 2)) ||
              (!strncmp( args[1], "hm", 2)) ||
              (!strncmp( args[1], "pt", 2)) ||
              (!strncmp( args[1], "sh", 2)) ||
              (!strncmp( args[1], "gp", 2)) ||
              (!strncmp( args[1], "mo", 2)) ||
              (!strncmp( args[1], "se", 2))
        )
    {
        FRESULT  res;
        FIL fp;

        char t_item[100];
        char buf[1024*4];
        int len;
        char *c;

        int fileSize;
        int nRead;
        //int i;
        int position = 0;
        //int tmp_idx = 0;

        c = &buf[0];
        sprintf(t_item,"1:%s.dat",args[1]);
        // strcpy(t_item,"1:ai.dat");
        debugprintf("t_item(%s)\r\n",t_item);

        res = f_open(&fp,t_item, FA_READ|FA_OPEN_EXISTING);   //delayms(5);
        if (res != FR_OK)
        {
            debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize = f_size(&fp);
        debugprintf("fileSize( %d )\r\n", fileSize);

        if (fileSize >(1024*1)-1)
        {
            len = (1024*1)-1;
            position = fileSize - (1024*1)-1;
        }
        else
        {
            len = fileSize;
            position = 0;
        }

        f_lseek(&fp, position);   //delayms(5);

        f_read(&fp,c,len,&nRead);
        f_close(&fp);

        buf[len]='\0';

        debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
        debugstring(buf);

    }

    // cpsw cp_sw - copy file from SD to WiFi
    else if ( (!strncmp( args[1], "cp_sw", 5)) ||
              (!strncmp( args[1], "cpsw", 4)))
    {
        print_files ("1:");
    }
    // cprs cp_rs - copy file from RAM to SD
    else if ( (!strncmp( args[1], "cp_rs", 5)) ||
              (!strncmp( args[1], "cprs", 4)))
    {
        FRESULT  res;
        FIL fp;

        char t_item[100];
        char buf[1024*512];
        int len;
        char *c;

        int fileSize, fileSize_bak;
        int nRead;
        int nWrite;
        int i;
        int position = 0;
        int tmp_idx = 0;

        c = &buf[0];
        strcpy(t_item,"1:boot.bin");
        debugprintf("t_item(%s)\r\n",t_item);

PRINTLINE;

        res = f_open(&fp,"1:boot.bin", FA_READ|FA_OPEN_EXISTING);   delayms(5);
        if (res != FR_OK)
        {
            debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize_bak = fileSize = f_size(&fp);
        debugprintf("fileSize( %d )\r\n", fileSize);

PRINTLINE;

        while (1)
        {
            if (fileSize >1024)
            {
                len = 1024;
                position += 1024;
            }
            else
            {
                len = fileSize;
                position += fileSize;
            }
            f_read(&fp,c,len,&nRead);
            c += nRead;
            f_lseek(&fp, position);   //delayms(5);

            debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
            debugprintf("[%04X] ", tmp_idx);
            for (i = 0; i < 16; ++i)
            {
                debugprintf("%02X ", (u8)buf[tmp_idx+i]);
            }
            debugstring("\r\n");
            tmp_idx += nRead;

            fileSize -= nRead;
            debugprintf("remain fileSize( %d )\r\n", fileSize);

            if ( fileSize <= 0 )
            {
                break;
            }
        }

PRINTLINE;

        f_close(&fp);
        debugstring("close file : boot.bin\r\n");

PRINTLINE;

        debugstring("open file : boot1.bin\r\n");
        res = f_open(&fp,"1:boot1.bin", FA_WRITE|FA_OPEN_ALWAYS);   delayms(5);
        if (res != FR_OK)
        {
            debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize = fileSize_bak;    //tmp_idx;
PRINTVAR(fileSize);
        position = 0;

PRINTLINE;
        while (1)
        {
            // debugprintf("fp.fsize(%d)\r\n",fp.fsize);
PRINTVAR(fp.fsize);
            f_lseek(&fp, (fp.fsize));   //delayms(5);
            if (fileSize >1024)
            {
                len = 1024;
                fileSize -= 1024;
                f_write(&fp, (buf+position), len, &nWrite);   //delayms(5);
                position += 1024;
                debugprintf("write(%d)\r\n",position);
            }
            else
            {
                len = fileSize;
                fileSize -= len;
                f_write(&fp, (buf+position), len, &nWrite);   //delayms(5);
                position += fileSize;
                debugprintf("write(%d)\r\n",position);
                break;
            }
        }

        f_close(&fp);   //delayms(5);
        debugstring("close file : boot1.bin\r\n");
        return;
    }

    // cprf cp_rf - copy file from RAM to FLASH
    else if ( (!strncmp( args[1], "cp_rf", 5)) ||
              (!strncmp( args[1], "cprf", 4)))
    {
        print_files ("1:");
    }

    // cpss cp_ss - copy file from SD to SD
    else if ( (!strncmp( args[1], "cp_ss", 5)) ||
              (!strncmp( args[1], "cpss", 4)))
    {
        FRESULT  res;
        FIL fp;

        char t_item[100];
        char buf[1024*512];
        int len;
        char *c;

        int fileSize, fileSize_bak;
        int nRead;
        int nWrite;
        int i;
        int position = 0;
        int tmp_idx = 0;

        c = &buf[0];
        strcpy(t_item,"1:boot.bin");
        debugprintf("t_item(%s)\r\n",t_item);

PRINTLINE;

        res = f_open(&fp,"1:boot.bin", FA_READ|FA_OPEN_EXISTING);   delayms(5);
        if (res != FR_OK)
        {
            debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize_bak = fileSize = f_size(&fp);
        debugprintf("fileSize( %d )\r\n", fileSize);

PRINTLINE;

        while (1)
        {
            if (fileSize >1024)
            {
                len = 1024;
                position += 1024;
            }
            else
            {
                len = fileSize;
                position += fileSize;
            }
            f_read(&fp,c,len,&nRead);
            c += nRead;
            f_lseek(&fp, position);   //delayms(5);

            debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
            debugprintf("[%04X] ", tmp_idx);
            for (i = 0; i < 16; ++i)
            {
                debugprintf("%02X ", (u8)buf[tmp_idx+i]);
            }
            debugstring("\r\n");
            tmp_idx += nRead;

            fileSize -= nRead;
            debugprintf("remain fileSize( %d )\r\n", fileSize);

            if ( fileSize <= 0 )
            {
                break;
            }
        }

PRINTLINE;

        f_close(&fp);
        debugstring("close file : boot.bin\r\n");

PRINTLINE;

        debugstring("open file : boot1.bin\r\n");
        res = f_open(&fp,"1:boot1.bin", FA_WRITE|FA_OPEN_ALWAYS);   delayms(5);
        if (res != FR_OK)
        {
            debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize = fileSize_bak;    //tmp_idx;
PRINTVAR(fileSize);
        position = 0;

PRINTLINE;
        while (1)
        {
            // debugprintf("fp.fsize(%d)\r\n",fp.fsize);
PRINTVAR(fp.fsize);
            f_lseek(&fp, (fp.fsize));   //delayms(5);
            if (fileSize >1024)
            {
                len = 1024;
                fileSize -= 1024;
                f_write(&fp, (buf+position), len, &nWrite);   //delayms(5);
                position += 1024;
                debugprintf("write(%d)\r\n",position);
            }
            else
            {
                len = fileSize;
                fileSize -= len;
                f_write(&fp, (buf+position), len, &nWrite);   //delayms(5);
                position += fileSize;
                debugprintf("write(%d)\r\n",position);
                break;
            }
        }

        f_close(&fp);   //delayms(5);
        debugstring("close file : boot1.bin\r\n");
        return;
    }

    // cpsf cp_sf - copy file from SD to FLASH
    else if ( (!strncmp( args[1], "cp_sf", 5)) ||
              (!strncmp( args[1], "cpsf", 4)))
    {
        FRESULT  res;
        FIL fp;

        char t_item[100];
        char buf[1024*512];
        int len;
        char *c;

        int fileSize, fileSize_bak;
        int nRead;
        //int nWrite;
        int i;
        int position = 0;
        int tmp_idx = 0;

        c = &buf[0];
        strcpy(t_item,"1:boot.bin");
        debugprintf("t_item(%s)\r\n",t_item);

PRINTLINE;

        res = f_open(&fp,"1:boot.bin", FA_READ|FA_OPEN_EXISTING);   delayms(5);
        if (res != FR_OK)
        {
            debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
            return;
        }

        fileSize_bak = fileSize = f_size(&fp);
        debugprintf("fileSize( %d )\r\n", fileSize);

PRINTLINE;

        while (1)
        {
            if (fileSize >1024)
            {
                len = 1024;
                position += 1024;
            }
            else
            {
                len = fileSize;
                position += fileSize;
            }
            f_read(&fp,c,len,&nRead);
            c += nRead;
            f_lseek(&fp, position);   //delayms(5);

            debugprintf("len( %d ), nRead( %d )\r\n", len, nRead);
            debugprintf("[%04X] ", tmp_idx);
            for (i = 0; i < 16; ++i)
            {
                debugprintf("%02X ", (u8)buf[tmp_idx+i]);
            }
            debugstring("\r\n");
            tmp_idx += nRead;

            fileSize -= nRead;
            debugprintf("remain fileSize( %d )\r\n", fileSize);

            if ( fileSize <= 0 )
            {
                break;
            }
        }

PRINTLINE;

        f_close(&fp);
        debugstring("close file : boot.bin\r\n");

        //--------------------------------------------------

        {
            //extern const unsigned int bootloader_trip_elf_bin_len;
            int i;
            // int sectors = bootloader_trip_elf_bin_len/flash_get_sectorsize();
            int sectors = fileSize_bak/flash_get_sectorsize();
            // if(bootloader_trip_elf_bin_len % flash_get_sectorsize())
            if(fileSize_bak % flash_get_sectorsize())
                sectors++;
            debugprintf("erase bootloader sectors...\r\n");
            for(i=20;i<(sectors+20);i++)
            {
                debugprintf("%d sector\r\n",i);
                flash_erase_sector(i,1);
            }
            debugstring("update....\r\n");
            // flash_write(0,(BYTE*)bootloader_trip_elf_bin,bootloader_trip_elf_bin_len);
            flash_write(0x14000,(BYTE*)buf,fileSize_bak);
            debugstring("completed\r\n");

        }


// PRINTLINE;

//         debugstring("open file : boot1.bin\r\n");
//         res = f_open(&fp,"1:boot1.bin", FA_WRITE|FA_OPEN_ALWAYS);   delayms(5);
//         if (res != FR_OK)
//         {
//             debugprintf("faild f_open(%s) : res : %d\r\n",t_item, res);
//             return;
//         }

//         fileSize = fileSize_bak;    //tmp_idx;
// PRINTVAR(fileSize);
//         position = 0;

// PRINTLINE;
//         while (1)
//         {
//             // debugprintf("fp.fsize(%d)\r\n",fp.fsize);
// PRINTVAR(fp.fsize);
//             f_lseek(&fp, (fp.fsize));   //delayms(5);
//             if (fileSize >1024)
//             {
//                 len = 1024;
//                 fileSize -= 1024;
//                 f_write(&fp, (buf+position), len, &nWrite);   //delayms(5);
//                 position += 1024;
//                 debugprintf("write(%d)\r\n",position);
//             }
//             else
//             {
//                 len = fileSize;
//                 fileSize -= len;
//                 f_write(&fp, (buf+position), len, &nWrite);   //delayms(5);
//                 position += fileSize;
//                 debugprintf("write(%d)\r\n",position);
//                 break;
//             }
//         }

//         f_close(&fp);   //delayms(5);
//         debugstring("close file : boot1.bin\r\n");
        return;
    }
}


static void cmd_gps(void)
{
    set_gps_rawdata_display();
}

static void cmd_alti(void)
{
    set_alti_rawdata_display();
}

static void cmd_sec(void)
{
    set_secData_display();
}
static void cmd_min(void)
{
    set_minData_display();
}


static void cmd_get(void)
{
    //char buf[21];
    if (argc<2)
    {
        return;
    }

    if ( (!strncmp( args[1], "bat", 3)))
    {
        measure_BAT_leval();
        debugprintf("bat : %03d\r\n", get_battery_level());
    }
}

void App_TestShInit ()
{

	//debugstring("\r\nTestShell init\r\n");
	//testSh_RegisterCmd ("exit", NULL, "exit --- exit test tool\n");					// 0
	testSh_RegisterCmd ("help", cmd_help, "help --- show available commands\n");		// 1
	//testSh_RegisterCmd ("hex", NULL, "hex --- command line radix hexa\n");			// 2
	//testSh_RegisterCmd ("dec", NULL, "dec --- command line radix decimal\n");		// 3

    // testSh_RegisterCmd ("at", cmd_at,    "temp command aa\r\n"); //
    testSh_RegisterCmd ("mv", cmd_mv,    "temp command aa\r\n"); //
    testSh_RegisterCmd ("gps", cmd_gps,  "toggle GPS sensor data.\r\n"); //
    testSh_RegisterCmd ("alti",cmd_alti, "toggle ALTI sensor data.\r\n"); //
    testSh_RegisterCmd ("sec", cmd_sec,  "toggle SEC data.\r\n"); //
    testSh_RegisterCmd ("min", cmd_min,  "toggle MIN data.\r\n"); //

    testSh_RegisterCmd ("chdir", cmd_chdir,  "sdc: chdir\r\n"); //
    // testSh_RegisterCmd ("dd", cmd_dd,    "temp command dd\r\n"); //
    // testSh_RegisterCmd ("bak", cmd_bak,    "temp command ee\r\n"); //
    testSh_RegisterCmd ("env", cmd_env,  "env [save init read print]\r\n"); //
    // testSh_RegisterCmd ("msg", cmd_msg,  "msg\r\n"); //
    // testSh_RegisterCmd ("ts", cmd_ts,    "ATM timesync test\r\n"); //
    testSh_RegisterCmd ("fs", cmd_fs,    "command file system\r\n"); //
    testSh_RegisterCmd ("cat", cmd_cat,    "command file system\r\n"); //
    testSh_RegisterCmd ("del", cmd_del,    "command file delete\r\n"); //
    // testSh_RegisterCmd ("gg", cmd_gg,    "temp command gg\r\n"); //
    // testSh_RegisterCmd ("mm", cmd_mm,    "temp command mm\r\n"); //
    // testSh_RegisterCmd ("qq", cmd_qq,    "temp command qq\r\n"); //
    // testSh_RegisterCmd ("rr", cmd_rr,    "temp command rr\r\n"); //
    // testSh_RegisterCmd ("ss", cmd_ss,    "temp command ss\r\n"); //
    // testSh_RegisterCmd ("tt", cmd_tt,    "temp command tt\r\n"); //
    // testSh_RegisterCmd ("uu", cmd_uu,    "temp command uu\r\n"); //
    // testSh_RegisterCmd ("vv", cmd_vv,    "temp command vv\r\n"); //
    // testSh_RegisterCmd ("ww", cmd_ww,    "temp command ww\r\n"); //
    // testSh_RegisterCmd ("xx", cmd_xx,    "temp command xx\r\n"); //
    // testSh_RegisterCmd ("yy", cmd_yy,    "temp command yy\r\n"); //
    // testSh_RegisterCmd ("zz", cmd_zz,    "temp command zz\r\n"); //
    // testSh_RegisterCmd ("sbr", cmd_sbr,  "sb16c1058 get reg, sbr ch regno\r\n"); //
    // testSh_RegisterCmd ("sbw", cmd_sbw,  "sb16c1058 set reg, sbr ch regno data\r\n"); //
    testSh_RegisterCmd ("set", cmd_set,  "set [heartbeat(hb)]\r\n"); //
	testSh_RegisterCmd ("get", cmd_get,  "get [bat]\r\n"); //
    // testSh_RegisterCmd ("pos", cmd_pos,  "pos --- position distance calc.\r\n");
    testSh_RegisterCmd ("rtc", cmd_rtc,  "rtc --- rtc .\r\n");
    // testSh_RegisterCmd ("sh",  cmd_sh,   "sh  --- sht11 port ctrl test.\r\n");
    // testSh_RegisterCmd ("syscmd", cmd_syscmd,     "syscmd --- sensor control command\r\n");
}


