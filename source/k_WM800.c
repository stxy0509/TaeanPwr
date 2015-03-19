#include "k_config.h"
#ifdef USE_WM800

#define __K_IRIDIUM_C__

#include "k_includes.h"

#define poPwrIridiumON()
#define poPwrIridiumOFF()


#define Q_IRI1_MAX  100
#define Q_IRI2_MAX  100

void iri1_rcv_q_put(char ch);
char iri1_rcv_q_get(void);

void iri2_rcv_q_put(char ch);
char iri2_rcv_q_get(void);
void iri2_rcv_q_init(void);

int iridium_init(int a_option);
int sbdreg_check(int a_option);
int sbdixa_check(int a_option);
int sbdrb_check(int a_option);




// u8 fg_sms_arrived = 0;
u8 mutex_cdma = 0;
u8 fg_telno_get_ok = 0;
u8 fg_sms_telno_get_ok = 0;



u8 modem_rxbuf_idx = 0;
char modem_rxbuf[256];
u8 fg_sms_arrived = 0;

void set_sms_arrived(void)
{
    fg_sms_arrived = 1;
}

void modem_rxbuf_clear(void)
{
    modem_rxbuf_idx = 0;
    modem_rxbuf[0] = '\0';
}

u8 modem_rxbuf_get_length(void)
{
    return(modem_rxbuf_idx);
}



void modem_rxbuf_print(void)
{
    char ch;
    int i=0;
    //긴 문자열은 출력하면 error 나옴....
    //debugprintf("%s",modem_rxbuf);
    while (1)
    {
        ch = modem_rxbuf[i++];
        if (ch == '\0' )
        {
            break;
        }
        uart_putch( debug_port_no, ch);
    }
}

void modem_rxbuf_printHex(void)
{
    int i = 0;
    char ch;
    debugstring("\r\n-----\r\n");
    while (1)
    {
        ch = modem_rxbuf[i++];
        if (ch != '\0')
        {
            if ( (ch==0x0D)||(ch==0x0A) )
            {
                debugprintf("%02d-%02X\r\n", (i-1), ch);
            }
            else
            {
                debugprintf("%02d-%02X-%c\r\n", (i-1), ch, ch);
            }
        }
        else
        {
            break;
        }
    }
    debugstring("-----\r\n");
}

int modem_rxbuf_substr(const char *a_str)
{
    int i = 0;
    int j;
    int matchFound;

    while (modem_rxbuf[i] != '\0')
    {
        j = 0;
        matchFound = 1;
        while (a_str[j] != '\0')
        {
            if (modem_rxbuf[i + j] == '\0' || a_str[j] != modem_rxbuf[i+j])
            {
                matchFound = 0;
                break;
            }
            j++;
        }
        if (matchFound)
        {
            //return 1;
            return i;
        }
        i++;
    }
    return 0;
}


#define IRIDIUM_CMD_POWER_ON    1
#define IRIDIUM_CMD_POWER_OFF   2
#define IRIDIUM_CMD_SEND_NEW    3
#define IRIDIUM_CMD_SEND_OLD    4


// iridium operating mode.
typedef enum {
    IM_IDLE,
    IM_INIT,
    IM_SEND,
    IM_RCV
} IRI_MODE_T;

//static IRI_MODE_T mode = IM_IDLE;

static u32 iri_event;
u32 iri_get_event(void)         {   return iri_event;   }
void iri_clr_event(void)        {   iri_event = 0;      }
void iri_set_event(u32 a_evt)   {   iri_event = a_evt;  }

static int iri1_init = 1;
int  get_iri1_init(void)        {   return iri1_init;   }
void set_iri1_init(int a_val)   {   iri1_init = a_val;  }

// static int iri2_init = 1;
// int  get_iri2_init(void)        {   return iri2_init;   }
// void set_iri2_init(int a_val)   {   iri2_init = a_val;  }

// static int mt1_status = 0;
// static int mt2_status = 0;
// int  get_mt1_status(void)       {   return mt1_status;   }
// int  get_mt2_status(void)       {   return mt2_status;   }
// int  get_mt_status(void)
// {
    // if (iri_port == C_IRIDIUM_1)
    // {
    //     return mt1_status;
    // }
    // else
    // {
    //     return mt2_status;
    // }
// }
// void set_mt1_status(int a_val)   {   mt1_status = a_val; }
// void set_mt2_status(int a_val)   {   mt2_status = a_val; }
// void set_mt_status(int a_val)
// {
//     if (iri_port == C_IRIDIUM_1)
//     {
//         mt1_status = a_val;
//     }
//     else
//     {
//         mt2_status = a_val;
//     }
// }

static int ring1 = 0;
// static int ring2 = 0;
void set_sbdring1(int a_val)   {   ring1 = a_val; }
// void set_sbdring2(int a_val)   {   ring2 = a_val; }
int  get_sbdring1(void)       {   return ring1;   }
// int  get_sbdring2(void)       {   return ring2;   }


char ctrl_msg[50];

static int iri_init_time_10sec = 1;     //맨 처음에는 10초만 전원 OFF 한다.

//----------------------------------------
// task_wm800()
// 1    : OK
//
// 2    : Timeout error
// 0    : NG
// 999  : Waiting...
//----------------------------------------
void task_wm800(void)
{
    static u32 idx=100;

    switch (idx)
    {
        case 0:
        case 50:
            // 이리듐 초기화(전원투입 후)


        case 100:
            // event 를 기다린다.
            // 발생한 event에 따라 해당 모드로 진입하여 작업을 수행한 후
            //   다시 돌아온다.

            if ((get_iri1_init() == 1 ))
            {
                idx = 500;      break;
            }

            // read mail




            // send message
            // if ( (0 == is_q_empty()) || (1==fgMsgC_ready) )    // not empty...
            if ( (0 == is_q_empty()) )    // not empty...
            {
                if (get_fgFW_updating())
                    return;

                if (mutex_cdma == 0)
                {
                    mutex_cdma = 1;
PRINTVAR(mutex_cdma);

                    debugstring(    "-------------------------------\r\n");
                    debugstring(    "     Send msg \r\n");
                    debugstring(    "-------------------------------\r\n");

                    // if (fgMsgC_ready==1)
                    // {
                    //     int i;
                    //     // strcpy(newMsg, s_msg_c);
                    //     for (i=0;i<MSG_LENGTH; i++)
                    //     {
                    //         newMsg[i] = s_msg_c[i];
                    //     }
                    // }
                    // else
                    {
                        int i;
                        q_getData_only();
                        // strcpy(newMsg, s_msg);
                        for (i=0;i<MSG_LENGTH; i++)
                        {
                            newMsg[i] = s_msg[i];
                        }
                    }
                    idx = 400;
                }
                break;
            }
            break;



            //---------------------------------------------------
        case 400:
            // iridium_Process_1(1);
            cdma_process_send(1);
            idx = 900;
            break;

            //---------------------------------------------------// iridium-1 init
        case 500:
            // power OFF
            if ((get_iri1_init() == 1))
            {
                debugstring("--## IRIDIUM-1 POWER_OFF for initialize ##--\r\n"); //(+)130905
//                cmdSensorControl(SYS_CMD_SENSOR_OFF, SYS_SENSOR_IRI1);
                // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_OFF, '1');                 //(+)130905
                cmdSensorControl(SYS_CMD_IRIDIUM_EXT_ON, '1');                 //(+)130905
            }

            if (iri_init_time_10sec == 1)
                tick_iri0 = 10000/10;    //10000/10;   //10sec
            else
                tick_iri0 = 60000/10;    //60000/10;   //60sec

            idx = 510;
            break;
        case 510:
            // power ON
            if (tick_iri0 == 0)
            {
                if ((get_iri1_init() == 1))
                {
                    idx = 520;
                }

                //tick_iri0 = 10000/10;    //10000/10;   //10sec
            }
            break;
        case 520:
            debugstring("--## IRIDIUM-1 POWER_ON ##--\r\n");                 //(+)130905
            cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_IRI1);
            // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_ON, '1');              //(+)130905
            cmdSensorControl(SYS_CMD_IRIDIUM_EXT_OFF, '1');              //(+)130905
            tick_iri0 = 10000/10;    //10000/10;   //10sec
            idx = 525;
            break;
        case 525:
            if (tick_iri0 == 0)
            {
                // iri_port = COMM_CHANNEL;
                wm800_process_init(0);
                idx = 530;
            }
            break;
        case 530:
/*
            if (wm800_process_init(1)==1)
            {
                // init IR1 is ended.
                set_iri1_init(0);

                // check IR2 init needed ?
                {
                    idx = 8000;
                }
            }
*/
            switch (wm800_process_init(1))
            {
                case 0:
                    break;
                case 1:
                    set_iri1_init(0);
                    idx = 8000;
                    break;
                case 2:
                    idx = 8000;
                    break;
                default:
                    break;
            }
            break;



            //---------------------------------------------------// SBDREG chk
        // case 700:
        //     // SBDREG chk
        //     debugprintf("===============>>>> SBDREG chk [1]\r\n");
        //     iri_port = C_IRIDIUM_1;
        //     sbdreg_check(1);
        //     idx = 710;
        //     break;
        // case 710:
        //     if (sbdreg_check(0)==1)
        //     {
        //     debugprintf("===============>>>> SBDREG chk [2]\r\n");
        //         iri_port = C_IRIDIUM_2;
        //         sbdreg_check(1);
        //         idx = 720;
        //     }
        //     break;
        // case 720:
        //     if (sbdreg_check(0)==1)
        //     {
        //         idx = 8000;
        //     }
        //     break;




        case 800:
            idx = 8000;
            break;

            //---------------------------------------------------// wait Result
        case 900:
            {
                //int i;
                // int iri_retval = iridium_Process_1(0);  //0: 상태체크용...
                int iri_retval = cdma_process_send(11);  //0: 상태체크용...
                switch (iri_retval)
                {
                    case 0: //err
                        debugprintf("\r\nSBD status :%d - send error\r\n",iri_retval);

                        mutex_cdma = 0;
PRINTVAR(mutex_cdma);

                        // q_pop();
                        // for (i=0;i<MSG_LENGTH; i++)
                        // {
                        //     s_msg[i] = newMsg[i];
                        // }
                        // q_putData();
                        // debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());


                        idx = 8000;
                        break;

                    case 1:
                        debugprintf("\r\nSBD status :%d - send success\r\n",iri_retval);

                        mutex_cdma = 0;
PRINTVAR(mutex_cdma);

                        // if (fgMsgC_ready==1)
                        // {
                        //     fgMsgC_ready = 0;
                        // }
                        // else
                        {
                            q_pop();
                            debugprintf("q_pop --> Q[%d]\r\n",is_q_dataNum());
                        }

                        if (sdc_read_detectPin()==SDC_INSERTED)
                        {
                            u32 fsz;
                            char t_item[130];

                            rtc_isSecUpdate();
                            sprintf(t_item,"%02d%02d%02d-%02d:%02d:%02d %s\r\n",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec,"send_ok");


                            sdc_saveDataToFile(FN_HISTORY, t_item, &fsz);
                            //PRINTVAR(fsz);
                            // if (fsz > FSZ_MAX)
                            // {
                            //     SensorBakSize.b.mose = 1;
                            // }
                        }

                        idx = 8000;
                        break;

                    case 2:
                        debugprintf("\r\nSBD status :%d - Timeout...\r\n",iri_retval);

                        mutex_cdma = 0;
PRINTVAR(mutex_cdma);
                        // q_pop();
                        // for (i=0;i<MSG_LENGTH; i++)
                        // {
                        //     s_msg[i] = newMsg[i];
                        // }
                        // q_putData();
                        // debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());

                        idx = 8000;
                        break;
                }
            }
            break;

        case 8000:
        default:
            idx = 100;
            break;
    }

}


void setReqInitIridium(void)
{
    debugprintf("####### init iridium [");
    // if ( iri_port==C_IRIDIUM_1)
    {
        // debugstring("1]\r\n");
        set_iri1_init(1);
    }
    // else if ( iri_port==C_IRIDIUM_2)
    // {
    //     debugstring("2]\r\n");
    //     set_iri2_init(1);
    // }
}


/*char modem_cmd[20][50] = {
    {"ATZ\r"},                                  // 0
    {"ATE0\r"},                                 // 1
    {"AT$NOTIFY=00000000\r"},                   // 2
    {"AT$ALERT=0,0,0,0,0,0,0,0,0,0,0\r"},       // 3
    {"AT$SIOFLOW=1\r"},                         // 4
    {"AT$KEYE\r"},                              // 5

    //{"AT$TCPCLOSE\r"},
    //{"AT$TCPEXIT\r"},

    {"AT+CRM=251\r"},                           // 6

    {"ATDT1501\r"},                             // 7

    {"AT$TCPOPEN=1.214.193.186,5000\r"},        // 8
    {"AT$TCPWRITE="},                           // 9
    {"AT$TCPCLOSE\r"},                          // 10
    {"AT$TCPEXIT\r"}                            // 11
};
*/


char iri_response[50];
int mo_status;
//int mt_status;
int csq;
//----------------------------------------
// iridium_Process_1()
// 1    : OK
//
// 2    : Timeout error
// 0    : NG
// 999  : Waiting...
//----------------------------------------
#if 0
int iridium_Process_1(int a_option)
{
    static int idx = 0;
    static int cmd_ptr = 0;
    static int resp_idx = 0;
    //static int retry_cnt = 0;
    //static char csq_value;
    //static char sbdi_value;
    static int sbdi_error;

    static int at_cnt = 0;;

    static int chg_n_retry=0;   // 이리듐을 교체하여  ATV 재시도 했는가??
    static int chg_atv_retry=0;   // 이리듐을 교체하여  ATV 재시도 했는가??
    static int chg_csq_retry=0;   // 이리듐을 교체하여  CSQ 재시도 했는가??
    static int chg_sbdi_retry=0;  // 이리듐을 교체하여 SBDI 재시도 했는가??
    char ch;
    int i;
    int ret_val = 999;
    //char s_msg[100];

    switch (a_option)
    {
        case 1:
            // 새로 시작
            idx = 0;
            cmd_ptr = nATZ;

            chg_n_retry = 0;
            chg_atv_retry = 0;
            chg_sbdi_retry = 0;
            chg_csq_retry = 0;

            break;
        case 2:
            idx = 100;
            break;
        case 0:
            // idx는 현재값을 유지하고...---> 상태체크용.
        default:
            break;
    }


    switch (idx)
    {
        //ATV1 ------------------------------------------------ATV1
        case 0:
            at_cnt = 0;
            cmd_ptr = nATZ;

            //debugprintf("\r\nIridium-[%d] is active.\r\n", ((iri_port==C_IRIDIUM_1) ? 1:2) );
            idx = 450;
            break;

        case 450:
            resp_idx = 0;
            debugprintf("ATZ ---> ");
            iridium_printf("ATZ\r");
            tick_iri0 = 1000/10;    //1000/10;   //1sec
            idx = 500;
            break;

        case 500:
            //wait 'OK'
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
            {
                iri_response[resp_idx++] = ch;
                // debugprintf("%c",ch);
                if ( (ch=='K') || (ch=='0') )
                {
                    debugprintf("[OK]\r\n");
                    idx = 505;
                }
            }
            if (tick_iri0==0)
            {
                idx = 505;
            }
            break;


        case 505:
            at_cnt++;
            if (at_cnt >5)
            {
                idx = 510;
            }
            else
            {
                idx = 450;
            }
            break;

        case 510:
            //PRINT_TIME;
            //debugprintf("\r\n-----ATV1 [%d]\r\n", ((iri_port==C_IRIDIUM_1) ? 1:2) );
            debugprintf("ATE0 ---> ");

            resp_idx = 0;
            //iridium_printf("ATV1\r");
            iridium_printf("ATE0\r");
            tick_iri0 = 1000/10;    //1000/10;   //1sec
            idx = 515;
            break;

        case 515:
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$NOTIFY=00000000 ---> ");
                iridium_printf("AT$NOTIFY=00000000\r");
                tick_iri0 = 1000/10;    //1000/10;   //1sec
                idx = 520;
            }
            break;

        case 520:
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$ALERT=0,0,0,0,0,0,0,0,0,0,0 ---> ");
                iridium_printf("AT$ALERT=0,0,0,0,0,0,0,0,0,0,0\r");
                tick_iri0 = 1000/10;    //1000/10;   //1sec
                idx = 525;
            }
            break;

        case 525:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$SIOFLOW=1 ---> ");
                iridium_printf("AT$SIOFLOW=1\r");
                tick_iri0 = 1000/10;    //1000/10;   //1sec
                idx = 530;
            }
            break;

        case 530:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$KEYE ---> ");
                iridium_printf("AT$KEYE\r");
                tick_iri0 = 1000/10;    //1000/10;   //1sec
                idx = 535;
            }
            break;

        case 535:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$TCPCLOSE ---> ");
                iridium_printf("AT$TCPCLOSE\r");
                tick_iri0 = 2000/10;    //1000/10;   //1sec
                idx = 540;
            }
            break;

        case 540:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$TCPEXIT ---> ");
                iridium_printf("AT$TCPEXIT\r");
                tick_iri0 = 2000/10;    //1000/10;   //1sec
                idx = 545;
            }
            break;

        case 545:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT+CRM=251 ---> ");
                iridium_printf("AT+CRM=251\r");
                tick_iri0 = 2000/10;    //1000/10;   //1sec
                idx = 550;
            }
            break;

        case 550:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("ATDT1501 ---> ");
                iridium_printf("ATDT1501\r");
                tick_iri0 = 10000/10;    //1000/10;   //1sec

#if 1
                idx = 555;
#else
                idx = 570;
#endif







            }
            break;

        case 555:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {

// ------------------->>>
// CHKPOINT
// 전송포트 결정

// #ifdef CDMA_TEST_PORT
//         // TEST_PORT
//                 debugprintf("AT$TCPOPEN=1.214.193.186,5000 ---> ");
//                 iridium_printf("AT$TCPOPEN=1.214.193.186,5000\r");
// #else //CDMA_RELRASE_PORT
        // RELEASE PORT
                debugprintf("AT$TCPOPEN=124.243.127.180,5000 ---> ");
                iridium_printf("AT$TCPOPEN=124.243.127.180,5000\r");
// #endif
// <<<------------------------
//
//
                tick_iri0 = 3000/10;    //1000/10;   //1sec
                idx = 560;
            }
            break;

        case 560:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                // tick_iri0 = 2000/10;    //1000/10;   //1sec
                idx = 565;
            }
            break;

        case 565:
            {
                /*
                //debugprintf("%s\r\n",s_msg);
                char *p;
                p = &s_msg[0];
                while(*p)
                {
                        UartPutCh(DEBUG_CHANNEL,*p);
                        p++;
                }
                UartPutCh(DEBUG_CHANNEL,'\r');
                UartPutCh(DEBUG_CHANNEL,'\n');
                */
            }

            debugprintf("AT$TCPWRITE= ---> ");
#ifndef NOT_SENDING
            iridium_printf("AT$TCPWRITE=");

            {
                char hex_buf[5];
                i=0;
                for (;;)
                {
                    ch = s_msg[i++];
                    if ( (ch==0) || (ch==0x0A) || (ch==0x0D) )
                    {
                        break;
                    }
                    else
                    {

                        debugprintf("%c",ch);
                        sprintf(hex_buf, "%02X", ch);
                        iridium_printf(hex_buf);
                    }
                }
                debugprintf("\r");
                iridium_printf("\r");

                tick_iri0 = TM_10SEC;
                //PRINTLINE;
            }
#endif
            tick_iri0 = 5000/10;    //1000/10;   //1sec
            idx = 570;
            break;

        case 570:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$TCPCLOSE ---> ");
                iridium_printf("AT$TCPCLOSE\r");
                tick_iri0 = 3000/10;    //1000/10;   //1sec
                idx = 575;
            }
            break;

        case 575:
            // if (UartGetCh(iri_port,&ch))    debugprintf("%c",ch);
            ch= iri1_rcv_q_get();
            if (ch != -1)
            // if (UartGetCh(iri_port,&ch))
                debugprintf("%c",ch);
            if (tick_iri0==0)
            {
                debugprintf("AT$TCPEXIT ---> ");
                iridium_printf("AT$TCPEXIT\r");
                tick_iri0 = 2000/10;    //1000/10;   //1sec
                idx = 580;
            }
            break;

        case 580:
            idx = 160;
            break;


            // post process  ------------------------------------------------
        case 160:
            if (sbdi_error==1)
            {
                ret_val = 0;    //sbdi error
            }
            else
            {
                ret_val = 1;    //success
            }

            // change_iridium();
            break;

        case 999:
            //timeout error
            ret_val = 2;

            // change_iridium();
            break;
    }
    return(ret_val);
}
#endif


// char s_msg[320];
char c_buf[50];
u8 cdma_process_send(u8 a_option)
{
    /*
    eturn : ret_val
        0: 전송중...
        1: 완료
        2: error
    */
    static u32 idx = 0;
    static u8 retry_cnt = 0;
    u8 ret_val = 99;
    int i;
    char ch;
    //char t_buf[15];
    static u8 buf_idx = 0;
    static u8 st_tcpopen = 0;

    if (a_option < 10)
    {
        idx = 0;
    }
    //if ((a_option%10)==2)
    //{
    //interval = 30;
    //}
   switch (idx)
    {
        case 0:
            debugstring("----- ATZ\r\n");
            retry_cnt = 0;
            idx = 12;
            break;
        case 12:
            PRINT_TIME;
            modem_rxbuf_clear();
            modem_printf("ATZ\r");
            tick_iri0 = TM_2SEC;
            idx = 13;
            break;
        case 13:
            if (tick_iri0==0)
            {
                if (modem_rxbuf_get_length() > 1)
                {
                    if (modem_rxbuf_substr("OK") > 0)
                    {
                        idx = 14;
                        break;
                    }
                }
                // 여기서 retry 하는 이유는 cdma 전원투입후 안정화 시간을 기다리는 것이 목적이다.
                // 2sec*30회 = 60sec (이정도면 전원안정화는 될것으로 생각함.)
                if (++retry_cnt < 5)
                {
                    debugprintf("ATZ NG: timeout(%d/5)\r\n", retry_cnt);
                    idx = 12;
                }
                else
                {
                    debugprintf( "ATZ NG: cdma connection error..\r\n" );
                    idx = 900;
                }
            }
            break;

        case 14:
            debugstring("----- ATE0\r\n");
            modem_rxbuf_clear();
            modem_printf("ATE0\r");
            tick_iri0 = 1500/10;
            idx = 15;
            break;
        case 15:
            if (tick_iri0==0)
            {
                debugstring("----- ATV1\r\n");
                modem_rxbuf_clear();
                modem_printf("ATV1\r");
                tick_iri0 = TM_1SEC;
                idx = 16;
            }
            break;
        case 16:
            if (tick_iri0==0)
            {
                debugstring("----- AT$NOTIFY\r\n");
                modem_rxbuf_clear();
                modem_printf("AT$NOTIFY=00000000\r");
                tick_iri0 = 1500/10;
                idx = 17;
            }
            break;
        case 17:
            if (tick_iri0==0)
            {
                debugstring("----- AT$ALERT\r\n");
                modem_rxbuf_clear();
                modem_printf("AT$ALERT=0,0,0,0,0,0,0,0,0,0,0\r");
                tick_iri0 = 1500/10;
                idx = 18;
            }
            break;

        case 18:
            if (tick_iri0==0)
            {
                //AT$SIOFLOW=1   // 흐름제어 없음
                //debugstring("----- init cdma : (2) AT$SIOFLOW=1\r\n");
                debugstring("----- AT$SIOFLOW\r\n");
                modem_rxbuf_clear();
                modem_printf("AT$SIOFLOW=1\r");
                tick_iri0 = 2000/10;    // 2sec
                idx = 19;
            }
            break;
        case 19:
            if (tick_iri0==0)
            {
                //AT$KEYE  // 전화 끊기
                debugstring("----- AT$KEYE\r\n");
                modem_rxbuf_clear();
                modem_printf("AT$KEYE\r");
                tick_iri0 = TM_10SEC;
                idx = 20;
            }
            break;
        case 20:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 11) )
            {
                // tick_iri0 = TM_1SEC;
                idx = 22;
            }
            break;
        case 22:
            // AT+CRM=251   //내장 TCP/IP를 사용
            debugstring("----- AT+CRM\r\n");
            modem_rxbuf_clear();
            modem_printf("AT+CRM=251\r");
            tick_iri0 = TM_5SEC;
            buf_idx = 0;
            idx = 24;
            break;
        case 24:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 2) )
            {
                tick_iri0 = TM_1SEC;
                idx = 26;
            }
            break;
        case 26:
            //wait 'OK'
            if (tick_iri0==0)
            {
                modem_rxbuf_print();
                //modem_rxbuf_printHex();

                if (modem_rxbuf_substr("OK") > 0)
                {
                    //OK
                    tick_iri0 = 1000/10;
                }
                idx = 28;
            }
            break;

        case 28:
            //AT$TCPCLOSE   //TCP socket close
            debugstring("----- AT$TCPCLOSE\r\n");
            modem_rxbuf_clear();
            modem_printf("AT$TCPCLOSE\r");
            tick_iri0 = TM_10SEC;
            idx = 30;
            break;
        case 30:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 11) )
            {
                tick_iri0 = TM_1SEC;
                idx = 32;
            }
            break;
        case 32:
            if (tick_iri0==0)
            {
                /*
                if (modem_rxbuf_get_length() > 0)
                {
                    modem_rxbuf_print();
                    modem_rxbuf_printHex();
                }
                */
                idx = 34;
            }
            break;

        case 34:
            // AT$TCPEXIT   //호 단절
            debugstring("----- AT$TCPEXIT\r\n");
            modem_rxbuf_clear();
            modem_printf("AT$TCPEXIT\r");
            tick_iri0 = TM_10SEC;
            idx = 36;
            break;
        case 36:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 5) )
            {
                tick_iri0 = TM_1SEC;
                idx = 38;
            }
            break;
        case 38:
            if (tick_iri0==0)
            {
                /*
                if (modem_rxbuf_get_length() > 0)
                {
                    modem_rxbuf_print();
                    modem_rxbuf_printHex();
                }
                */
                idx = 40;
            }
            break;

        case 40:
            // ATDT1501  //전화걸기
            debugstring("----- ATDT1501\r\n");
            modem_rxbuf_clear();
            modem_printf("ATDT1501\r");
            tick_iri0 = TM_30SEC;
            buf_idx = 0;
            idx = 45;
            break;
        case 45:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 8) )
            {
                tick_iri0 = TM_1SEC;
                idx = 50;
            }
            break;
        case 50:
            //wait 'CONNECT'
            if (tick_iri0==0)
            {
                modem_rxbuf_print();
                //modem_rxbuf_printHex();

                if (modem_rxbuf_substr("CONNECT") > 0)
                {
                    idx = 60;
                    break;
                }
                else
                {
                    // retry_cnt++;
                    // if (retry_cnt == 5)
                    // {
/*
                        st_tcpopen = 0;
                        poPwrCdmaOFF();
                        tick_iri0 = TM_1SEC;
*/
                    //     idx = 52;

                    // }
                    // else if (retry_cnt < 10)
                    // {
                    //     debugprintf("[ATDT1501] NG: timeout(%d/5)\r\n", retry_cnt);
                    //     idx = 40;
                    // }
                    // else
                    // {
                        debugprintf( "[ATDT1501] NG: cdma connection error..\r\n" );
                        idx = 900;
                    // }
                }
            }
            break;
        case 52:
/*
            //1초동안 poPwrCdmaOFF
            if (tick_iri0==0)
            {
                poPwrCdmaON();
                idx = 54;
            }
            break;
        case 54:
            //5초동안 wait
            if (tick_iri0==0)
            {
                cdma_process_init(0);
                idx = 10;
            }
            break;
*/
//------------------------

        case 55:
        case 60:
            //PRINTLINE;
            st_tcpopen = 0;
            modem_rxbuf_clear();
// #ifdef CDMA_TEST_PORT
            if (env.mode == 1)
            {
                // TEST IP,PORT
                debugstring("-----[TEST] AT$TCPOPEN=1.214.193.186,5000\r\n");
                modem_printf("AT$TCPOPEN=1.214.193.186,5000\r");        // 124.243.127.180,10000
            }
            else
            {
                // RELEASE IP,PORT
                debugstring("-----[RELEASE] AT$TCPOPEN=124.243.127.180,5000\r\n");
                modem_printf("AT$TCPOPEN=124.243.127.180,5000\r");
                        modem_printf("AT$TCPOPEN=124.243.127.180,5000\r");
                        // modem_printf("AT$TCPOPEN=124.243.127.180,5000\r");
}
// #endif
            tick_iri0 = TM_20SEC;
            buf_idx = 0;
            idx = 65;
            break;
        case 65:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 8) )
            {
                tick_iri0 = TM_1SEC;
                idx = 70;
            }
            break;
        case 70:
            if (tick_iri0==0)
            {
                modem_rxbuf_print();
                //modem_rxbuf_printHex();

                if (modem_rxbuf_substr("$TCPOPEN") > 0)
                {
                    //PRINTLINE;

                    idx = 75;
                    break;
                }
                //}
                else
                {
                    // if (++retry_cnt < 5)
                    // {
                    //     debugprintf("[AT$TCPOPEN] NG: timeout(%d/5)\r\n", retry_cnt);
                    //     //PRINTLINE;

                    //     idx = 60;
                    // }
                    // else
                    // {
                        debugprintf( "[AT$TCPOPEN] NG: cdma connection error..\r\n" );
                        //PRINTLINE;

                        idx = 900;
                    // }
                }
            }
            break;

        case 75:
        case 80:
// AT$TCPWRITE=

            //PRINTLINE;
            //debugprintf("%s",s_msg);
            {
                //debugprintf("%s\r\n",s_msg);
                char *p;
                p = &s_msg[0];
                while(*p)
                {
                        uart_putch(debug_port_no, *p);
                        p++;
                }
                debugstring("\r\n");
            }

            modem_rxbuf_clear();
            modem_printf("AT$TCPWRITE=");
            i=0;
            for (;;)
            {
                ch = s_msg[i++];
                if ( (ch==0) || (ch==0x0A) || (ch==0x0D) )
                {
                    break;
                }
                else
                {
                    char tbuf[5];
                    debugprintf("%X",ch);
                    sprintf(tbuf,"%X", ch);
                    modem_printf(tbuf);
                }
            }
            debugprintf("\r");
            modem_printf("\r");

            tick_iri0 = TM_10SEC;
            //PRINTLINE;

            idx = 85;
            break;
        case 85:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 8) )
            {
                tick_iri0 = TM_1SEC;
                //PRINTLINE;

                idx = 90;
            }
            break;

        case 90:
            //wait '$TCPSENDDONE'------------------
            if (tick_iri0 == 0)
            {
                modem_rxbuf_print();
                //modem_rxbuf_printHex();
                if (modem_rxbuf_substr("$TCPSENDDONE") > 0)
                {
/*
                    //send success
                    q_pop();
                    debugprintf("q_pop --> Q[%d]\r\n",is_q_dataNum());

                    if (is_q_empty()==0)
                    {
                        //PRINTLINE;

                        saveDeferredMsgFile();
                    }
                    else
                    {
                        //PRINTLINE;

                        clearDeferedMsgFile();
                    }
*/

                    tick_iri0 = TM_1SEC;
                    idx = 95;
                }
                else
                {
                    // if (++retry_cnt < 5)
                    // {
                    //     debugprintf("[AT$TCPWRITE] NG: timeout(%d/5)\r\n", retry_cnt);
                    //     tick_iri0 = TM_5SEC;
                    //     idx = 92;
                    // }
                    // else
                    // {
                        debugprintf( "[AT$TCPWRITE] NG: cdma connection error..\r\n" );
                        idx = 900;
                    // }
                }
            }
            break;
        case 92:
            if (tick_iri0 == 0)
            {
                idx = 80;
            }
            break;
        case 95:
            if (tick_iri0==0)
            {
                // if (is_q_empty())
                {
                    debugstring("q: empty --> stop sending...\r\n");
                    idx = 100;
                }
                /*else
                {
                    q_getData_only();
                    idx = 80;
                }*/
            }
            break;

        case 100:
// AT$TCPCLOSE------------------
            debugstring("----- AT$TCPCLOSE\r\n");
            modem_rxbuf_clear();
            modem_printf("AT$TCPCLOSE\r");
            tick_iri0 = TM_10SEC;
            idx = 105;
            break;
        case 105:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 8) )
            {
                tick_iri0 = TM_1SEC;
                idx = 110;
            }
            break;

        case 110:
            //wait '$TCPCLOSED'
            if (tick_iri0==0)
            {
                    //modem_rxbuf_print();
                    //modem_rxbuf_printHex();

                    if (modem_rxbuf_substr("$TCPCLOSED") > 0)
                    {
                        idx = 112;
                    }
            }
            break;

        case 112:
// AT$TCPEXIT   //호 단절
            debugstring("----- AT$TCPEXIT\r\n");
            modem_rxbuf_clear();
            modem_printf("AT$TCPEXIT\r");
            tick_iri0 = 10000/10;
            idx = 113;
            break;
        case 113:
            if ( (tick_iri0==0) || (modem_rxbuf_get_length() > 17) )
            {
                tick_iri0 = 1000/10;
                idx = 114;
            }
            break;
        case 114:
            if (tick_iri0==0)
            {
                if (modem_rxbuf_get_length() > 0)
                {
                    //modem_rxbuf_print();
                    //modem_rxbuf_printHex();
                }
                idx = 115;
            }
            break;

        case 115:
            if (tick_iri0==0)
            {
                idx = 120;
            }
            break;

        case 120:
            ret_val = 1;
            break;
        case 900:
            ret_val = 2;
            break;
    }
    return(ret_val);
}





//static int result_no = 999;
//static int sbdring = 0;
char result_s[30];
char result_sbd[50];

char cdma_tel_no[15];
char sms_tel_no[15];


void task_wm800_rcv(void)
{
    char ch;
    int saveToBuf = 1;
    static char noti_buf[30];
    static int noti_buf_idx = 0;


    if (uart_getch(COMM_CHANNEL, &ch))
    {

#if 1
        // PRINTLINE;
        uart_putch(0, ch);
        // debugprintf("%X\r\n",ch);
#endif

//20120510113532,01033533825,4098,16,0,7265736574

        if (ch == '$')
        {
            noti_buf[0] = ch;
            noti_buf_idx = 1;
            saveToBuf = 0;
        }
        else
        {
            switch (noti_buf_idx)
            {
                case 0:
                    break;
                case 1:
                    if (ch == '0')
                    {
                        noti_buf[noti_buf_idx++] = ch;
                        noti_buf[noti_buf_idx] = '\0';
                        saveToBuf = 0;
                    }
                    else
                    {
                        modem_rxbuf[modem_rxbuf_idx++] = '$';
                        modem_rxbuf[modem_rxbuf_idx++] = ch;
                        modem_rxbuf[modem_rxbuf_idx] = '\0';
                        noti_buf_idx = 0;
                        saveToBuf = 0;
                    }
                    break;
                default:
                    if (noti_buf_idx < 28)
                    {
                        noti_buf[noti_buf_idx++] = ch;
                        noti_buf[noti_buf_idx] = '\0';
                        saveToBuf = 0;
                    }
                    if ( ch == 0x0A )
                    {
                        //debugprintf("##[%d]%s\r\n",noti_buf_idx,noti_buf);
                        if ( (noti_buf[3]=='8') && (noti_buf[4]==':') && (noti_buf[5]=='0') )
                        {
                            debugprintf("##[%d]%s\r\n", noti_buf_idx, noti_buf);
                            fg_sms_arrived = 1;
                        }
                        else if ( (noti_buf[1]=='0') && (noti_buf[2]=='0') && (noti_buf[3]=='2') )
                        {
                            debugprintf("##[RING]-hang up the phone\r\n", noti_buf_idx, noti_buf);
                            //fg_sms_arrived = 1;
                            modem_printf("AT$KEYE\r");
                            //
                        }
                        noti_buf_idx = 0;
                        saveToBuf = 0;
                    }
                    break;
            }
        }

        if (saveToBuf == 1)
        {
            if (modem_rxbuf_idx < 254)      // buf over를 방지...
            {
                modem_rxbuf[modem_rxbuf_idx++] = ch;
                modem_rxbuf[modem_rxbuf_idx] = '\0';
            }
        }
    }
    return;
}



void task_cdma_sms(void)
{
    cdma_process_sms(1);

    TM_watchSMS = 0;
}



//-------------------------------------------------------------------
t_SENSOR_Q iri1_q;
// t_SENSOR_Q iri2_q;

ALIGN4 char iri1_q_buf[Q_IRI1_MAX];
// ALIGN4 char iri2_q_buf[Q_IRI2_MAX];


// int iri_rcv_get(int no, char *ch)
// {
//     char tmp;
//     if (no == C_IRIDIUM_1)
//     {
//         tmp = iri1_rcv_q_get();
//     }
//     // else
//     // {
//     //     tmp = iri2_rcv_q_get();
//     // }

//     if (tmp != -1)
//     {
//         *ch = tmp;
//         return(1);
//     }
//     else
//     {
//         return(0);
//     }
// }

void iri1_rcv_q_put(char ch)
{
    iri1_q_buf[iri1_q.rear] = ch;
    ++iri1_q.rear;
    iri1_q.rear %= Q_IRI1_MAX;
    iri1_q.len++;
}

// char iri1_rcv_q_get(void)
// {
//     char ch;
//     if (iri1_q.front == iri1_q.rear)
//     {
//         ch = -1;    //empty
//     }
//     else
//     {
//         ch = iri1_q_buf[iri1_q.front];
//         ++iri1_q.front;
//         iri1_q.front %= Q_IRI1_MAX;
//         iri1_q.len--;
//     }
//     return ch;
// }
char iri1_rcv_q_get(void)
{
    char ch;
    if (iri1_q.front == iri1_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = iri1_q_buf[iri1_q.front];
        ++iri1_q.front;
        iri1_q.front %= Q_IRI1_MAX;
        iri1_q.len--;
    }
    return ch;
}
void iri1_rcv_q_init(void)
{
    iri1_q.rear = 0;
    iri1_q.front = 0;
    iri1_q.len = 0;
}


u32 get_iri1_rcv_len(void)
{
    return iri1_q.len;
}

//-------------------------------------------------------------------

// void iri2_rcv_q_put(char ch)
// {
//     iri2_q_buf[iri2_q.rear] = ch;
//     ++iri2_q.rear;
//     iri2_q.rear %= Q_IRI2_MAX;
//     iri2_q.len++;
// }

// char iri2_rcv_q_get(void)
// {
//     char ch;
//     if (iri2_q.front == iri2_q.rear)
//     {
//         ch = -1;    //empty
//     }
//     else
//     {
//         ch = iri2_q_buf[iri2_q.front];
//         ++iri2_q.front;
//         iri2_q.front %= Q_IRI2_MAX;
//         iri2_q.len--;
//     }
//     return ch;
// }

// void iri2_rcv_q_init(void)
// {
//     iri2_q.rear = 0;
//     iri2_q.front = 0;
//     iri2_q.len = 0;
// }

// u32 get_iri2_rcv_len(void)
// {
//     return iri2_q.len;
// }




u8 wm800_process_init(u8 a_option)
{
    /*
    return : ret_val
        0: 초기화중...
        1: 완료
        2: error             atdt
    */
    static u32 idx = 0;
    static u8 retry_cnt = 0;
    u8 ret_val = 0;
    //char ch;


    // iri_port = COMM_CHANNEL;



    if (a_option==0)
    {
        idx = 0;
    }

    switch (idx)
    {
        case 0:
// PRINTVAR(idx);
            debugstring("----- init cdma : (1) atz\r\n");
            retry_cnt = 0;
            idx = 5;
            break;

        case 5:
// PRINTVAR(idx);

            modem_rxbuf_clear();
            modem_printf("ATZ\r");
            debugprintf("ATZ-->");
            // iridium_printf("ATZ\r");
            tick_iri1 = 2000/10;
            idx = 10;
            break;

        case 10:
            if (tick_iri1==0)
            {
                if (modem_rxbuf_get_length() > 0)
                {
                    // debugstring("atz-->");
                    modem_rxbuf_print();
                    //modem_rxbuf_printHex();

                    if (modem_rxbuf_substr("OK") > 0)
                    {
                        idx = 12;
                        break;
                    }
                }
                // 여기서 retry 하는 이유는 cdma 전원투입후 안정화 시간을 기다리는 것이 목적이다.
                // 2sec*30회 = 60sec (이정도면 전원안정화는 될것으로 생각함.)
                if (++retry_cnt < 5)
                {
                    debugprintf("ATZ NG: timeout(%d/5)\r\n", retry_cnt);
                    idx = 5;
                }
                else
                {
                    debugprintf( "ATZ NG: cdma connection error..\r\n" );
                    idx = 900;
                }
            }
            // else
            // {
            //     if (modem_rxbuf_get_length() > 0)
            //     {
            //         modem_rxbuf_print();
            //         // modem_rxbuf_printHex();

            //         if (modem_rxbuf_substr("OK") > 0)
            //         {
            //             idx = 12;
            //             // break;
            //         }
            //     }
            // }
            break;

        case 12:
            modem_rxbuf_clear();
            modem_printf("ATE1\r");
            // debugprintf("ATE0-->");
            tick_iri1 = 1500/10;
            idx = 13;
            break;
        case 13:
            if (tick_iri1==0)
            {
                modem_rxbuf_print();
                // modem_rxbuf_printHex();
                idx = 16;
            }
            break;

        case 16:
            modem_rxbuf_clear();
            modem_printf("AT$NOTIFY=00000000\r");
            tick_iri1 = 1500/10;
            idx = 17;
            break;
        case 17:
            if (tick_iri1==0)
            {
                modem_rxbuf_print();
                idx = 18;
            }
            break;

        case 18:
            modem_rxbuf_clear();
            modem_printf("AT$ALERT=0,0,0,0,0,0,0,0,0,0,0\r");
            tick_iri1 = 1500/10;
            idx = 19;
            break;
        case 19:
            if (tick_iri1==0)
            {
                modem_rxbuf_print();
                idx = 20;
            }
            break;

        case 20:
        case 30:
            //AT$SIOFLOW=1   // 흐름제어 없음
            //debugstring("----- init cdma : (2) AT$SIOFLOW=1\r\n");
            modem_rxbuf_clear();
            modem_printf("AT$SIOFLOW=1\r");
            tick_iri1 = 2000/10;    // 2sec
            idx = 40;
            break;
        case 40:
            if (tick_iri1==0)
            {
                modem_rxbuf_print();
                /*
                if (modem_rxbuf_get_length() > 0)
                {
                    modem_rxbuf_print();
                    modem_rxbuf_printHex();
                }
                */
                idx = 42;
            }
            break;
        case 42:
            // phone number
            modem_rxbuf_clear();
            modem_printf("AT$PHONENUM?\r");
            tick_iri1 = 5000/10;    // 2sec
            idx = 44;
            break;
        case 44:
            if ( (tick_iri1==0)  )
            {
// PRINTLINE;
               idx = 50;
               tick_iri1 = 500/10;
            }
            else
            {
                if ( modem_rxbuf_get_length() > 30 )
                {
// PRINTLINE;
                    modem_rxbuf_print()
;
                    {
                        int i,j=0;
                        char ch1;

                        for(i=15; i<30; i++)
                        {
                           ch1 = modem_rxbuf[i];
                           // uart_putch(0, ch1);
                           // debugprintf("%d: %x\r\n",i,ch1);
//                            if ( (ch1 == ',') )
                           if ( (ch1 == ',') )
                           {
                               cdma_tel_no[j] = '\0';
                               debugprintf("PHONE_NO: %s\r\n", cdma_tel_no);
                               idx = 50;
                               break;
                           }
                           else
                           {
// PRINTLINE;
                               cdma_tel_no[j] = ch1;
                               j++;
                           }
                        }
                        idx = 50;
                        tick_iri1 = 500/10;
                    }
                }
            }
            break;


        case 50:
            if (tick_iri1==0)
            {
                //AT$KEYE  // 전화 끊기
                debugstring("----- init cdma : (3) AT$KEYE\r\n");
                modem_rxbuf_clear();
                modem_printf("AT$KEYE\r");
                tick_iri1 = TM_10SEC;
                idx = 55;
            }
            break;
        case 55:
            if ( (tick_iri1==0) || (modem_rxbuf_get_length() > 11) )
            {
                tick_iri1 = TM_1SEC;
                idx = 60;
            }
            break;
        case 60:
            if (tick_iri1==0)
            {
                modem_rxbuf_print();
                /*
                if (modem_rxbuf_get_length() > 0)
                {
                    //modem_rxbuf_print();
                    modem_rxbuf_printHex();
                }
                */
                idx = 70;
            }
            break;

        case 70:
            //AT$TCPCLOSE   //TCP socket close
            //debugstring("----- init cdma : (4) AT$TCPCLOSE\r\n");
            modem_rxbuf_clear();
            modem_printf("AT$TCPCLOSE\r");
            tick_iri1 = TM_10SEC;
            idx = 75;
            break;
        case 75:
            if ( (tick_iri1==0) || (modem_rxbuf_get_length() > 11) )
            {
                tick_iri1 = TM_1SEC;
                idx = 80;
            }
            break;
        case 80:
            if (tick_iri1==0)
            {
                /*
                if (modem_rxbuf_get_length() > 0)
                {
                    modem_rxbuf_print();
                    modem_rxbuf_printHex();
                }
                */
                idx = 90;
            }
            break;

        case 90:
            // AT$TCPEXIT   //호 단절
            //debugstring("----- init cdma : (5) AT$TCPEXIT\r\n");
            modem_rxbuf_clear();
            modem_printf("AT$TCPEXIT\r");
            tick_iri1 = TM_10SEC;
            idx = 95;
            break;
        case 95:
            if ( (tick_iri1==0) || (modem_rxbuf_get_length() > 5) )
            {
                tick_iri1 = TM_1SEC;
                idx = 100;
            }
            break;
        case 100:
            if (tick_iri1==0)
            {
                /*
                if (modem_rxbuf_get_length() > 0)
                {
                    modem_rxbuf_print();
                    modem_rxbuf_printHex();
                }
                */
                idx = 110;
            }
            break;
        case 110:
            ret_val = 1;
            break;
        case 900:
            ret_val = 2;
            break;
    }
    return(ret_val);
}



int iridium_init(int a_option)
{
    static int idx = 0;
    static int resp_idx = 0;
    static int retry_cnt = 0;
    static int at_cnt = 0;;
    char ch;
    // int i;
    int ret_val = 999;

    switch (a_option)
    {
        case 1:
            // 새로 시작
            idx = 0;
            break;
        case 2:
            idx = 100;
            break;
        case 0:
            // idx는 현재값을 유지하고...---> 상태체크용.
            //break;  <--- 불필요...
        default:
            break;
    }


    switch (idx)
    {
        //ATV1 ------------------------------------------------ATV1
        case 0:
            at_cnt = 0;
            // debugprintf("\r\nIridium-[%d] init start.\r\n", ((iri_port==C_IRIDIUM_1) ? 1:2) );
            idx = 450;
            break;

        case 450:
            resp_idx = 0;
            debugprintf("AT ---> ");
            iridium_printf("AT\r");
            tick_iri0 = 1000/10;    //1000/10;   //1sec
            idx = 500;
            break;
        case 500:
            //wait 'OK'
            // if (UartGetCh(iri_port,&ch))
            ch = iri1_rcv_q_get();
            if (ch != -1)
            {
                // debugprintf("%c", ch);
                iri_response[resp_idx++] = ch;
                if ( (ch=='K') || (ch=='0') )
                {
                    debugprintf("[OK]\r\n");
                    idx = 505;
                }
            }
            if (tick_iri0==0)
            {
                idx = 505;
            }
            break;

        case 505:
            at_cnt++;
            if (at_cnt >5)
            {
                idx = 550;
            }
            else
            {
                idx = 450;
            }
            break;

        case 550:
            //PRINT_TIME;
            debugprintf("ATV0 ---> ");

            resp_idx = 0;
            //iridium_printf("ATV1\r");
            iridium_printf("ATV0\r");
            tick_iri0 = 5000/10;    //10000/10;   //10sec
            idx = 2;
            break;

        case 2:
            //wait 'OK'
            // if (UartGetCh(iri_port,&ch))
            // if (UartGetCh(iri_port,&ch))
            ch = iri1_rcv_q_get();
            if (ch != -1)
            {
                iri_response[resp_idx++] = ch;
                // debugprintf("%c",ch);
                if ( (ch=='K') || (ch=='0'))
                {
                    debugprintf("[OK]\r\n");
                    idx = 20;
                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                idx = 999;
            }
            break;

            //ATE0 ------------------------------------------------ATE0
        case 20:
            debugstring("ATE0 ---> ");

            //PRINT_TIME;
            iridium_printf("ATE0\r");
            tick_iri0 = 5000/10;    //5sec
            idx = 25;
            break;
        case 25:
            //wait 'OK'
            // if (UartGetCh(iri_port,&ch))
            // if (UartGetCh(iri_port,&ch))
            ch = iri1_rcv_q_get();
            if (ch != -1)
            {
                // debugprintf("%c",ch);
                if (( ch=='K') || (ch=='0') )
                {
                    debugprintf("[OK]\r\n");
                    idx = 30;
                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                //debugstring("ATE0 NG\r\n");
                idx = 999;
            }
            break;

            //AT&K0 ------------------------------------------------AT&K0
        case 30:
            // for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            iri1_rcv_q_init();

            debugstring("AT&K0 ---> ");

            //PRINT_TIME;
            iridium_printf("AT&K0\r");
            tick_iri0 = 5000/10;    //5sec
            idx = 35;
            break;
        case 35:
            //wait 'OK'
            // if (UartGetCh(iri_port,&ch))
            // if (UartGetCh(iri_port,&ch))
            ch = iri1_rcv_q_get();
            if (ch != -1)
            {
                // debugprintf("%c",ch);
                if ( (ch=='K') || (ch=='0') )
                {
                    debugprintf("[OK]\r\n");
                    idx = 40;
                    //idx = 50;
                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                //debugstring("AT&K0 NG\r\n");
                idx = 999;
            }
            break;

            //AT&D0 ------------------------------------------------AT&D0
        case 40:
            // for (i=0;i<10;i++) iri1_rcv_q_get(ch);//UartGetCh(iri_port,&ch);
            iri1_rcv_q_init();
            debugstring("AT&D0 ---> ");

            //PRINT_TIME;
            iridium_printf("AT&D0\r");
            tick_iri0 = 5000/10;    //5sec
            idx = 45;
            break;
        case 45:
            //wait 'OK'
            // if (UartGetCh(iri_port,&ch))
            ch = iri1_rcv_q_get();
            if (ch!= -1)
            {
                // debugprintf("%c",ch);
                if ((ch=='K') || (ch=='0') )
                {
                    debugprintf("[OK]\r\n");
                    idx = 50;
                }
            }
            else if (tick_iri0==0)
            {
                //PRINT_TIME;
                debugprintf("[NG]\r\n");
                // debugstring("----AT&D0 NG\r\n");
                idx = 999;
            }
            break;

            // ------------------------------------------------------ATS0=2
            //  자동응답 Autoanswer.
        case 50:
            // for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            iri1_rcv_q_init();

            debugstring("ATS0=2 ---> ");

            //PRINT_TIME;
            iridium_printf("ATS0=2\r");
            tick_iri0 = 5000/10;    //5sec
            idx = 55;
            break;
        case 55:
            //wait 'OK'
            // if (UartGetCh(iri_port,&ch))
            ch = iri1_rcv_q_get();
            if (ch!= -1)
            {
                // debugprintf("%c",ch);
                if ((ch=='K') || (ch=='0') )
                {
                    debugprintf("[OK]\r\n");
                    idx = 56;
                    //initial RETRY_CNT:  in case of 'CSQ=0'
                    retry_cnt = 0;
                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                // debugstring("----ATS0=2 NG\r\n");
                idx = 999;
            }
            break;

            // ---------------------------------------------------AT+SBDMTA=1
            //  Enable or disable ring indications for SBD Ring Alerts.
            //      1 : Enable ring indication (default).
        case 56:
            // for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            iri1_rcv_q_init();

            debugstring("AT+SBDMTA=1 ---> ");

            //PRINT_TIME;
            iridium_printf("AT+SBDMTA=1\r");
            tick_iri0 = 5000/10;    //5sec
            idx = 57;
            break;
        case 57:
            //wait 'OK'
            // if (UartGetCh(iri_port,&ch))
            ch = iri1_rcv_q_get();
            if (ch!= -1)
            {
                // debugprintf("%c",ch);
                if ((ch=='K') || (ch=='0') )
                {
                    debugprintf("[OK]\r\n");
                    idx = 60;
                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                // debugstring("----AT+SBDMTA=1 NG\r\n");
                idx = 999;
            }
            break;

        case 60:
        case 999:
        default:
            ret_val = 1;
            break;
    }
    return(ret_val);
}


//--------------------------------------------------------------
int
sbdreg_check(int a_option)
{
    return 1;
}



int sbdixa_check(int a_option)
{
    return 1;
}


void ctrl_sensor_set_env(char a_id, char a_code)
{
    if (a_code == '0')  return;

    switch(a_id)
    {
        case '0': env.s0 = a_code;  break;
        case '1': env.s1 = a_code;  break;
        case '3': env.s3 = a_code;  break;
        case '5': env.s5 = a_code;  break;
        case '7': env.s7 = a_code;  break;
        case '8': env.s8 = a_code;  break;
        case '9': env.s9 = a_code;  break;
        case 'a':
        case 'A': env.sa = a_code;  break;
        case 'b':
        case 'B': env.sb = a_code;  break;
        case 'c':
        case 'C': env.sc = a_code;  break;
        case 'e':
        case 'E': env.se = a_code;  break;
        case 'f':
        case 'F': env.sf = a_code;  break;
    }
    env_save();
}

void ctrl_sensor(char a_id, char a_code)
{
    char cmd = '1';
    //'0' : Reset, '1' : On, '2' : Off

    switch (a_code)
    {
        case '0':   cmd = '4';  break;  // reset
        case '1':   cmd = '1';  break;  // on
        case '2':   cmd = '0';  break;  // off
    }

    debugstring("+++ control : ");
    switch(a_id)
    {
        case '0': debugstring("AIO");    break;
        case '1': debugstring("MOSE");   break;
        case '3': debugstring("CT3919"); break;
        case '5': debugstring("DCS");    break;
        case '7': debugstring("GPS");    break;
        case '8': debugstring("ATM1");   break;
        case '9': debugstring("ATM2");   break;
        case 'b':
        case 'B': debugstring("HMP155"); break;
        case 'c':
        case 'C': debugstring("PTB210"); break;
        case 'f':
        case 'F': debugstring("SHOCK"); break;
        case 'r':
        case 'R': debugstring("SYSTEM"); break;
    }
    if      (a_code == '0')      debugstring(" RESET");
    else if (a_code == '1')      debugstring(" ON");
    else if (a_code == '2')      debugstring(" OFF");
    debugstring("\r\n");

    switch(a_id)
    {
        case '0': cmdSensorControl(cmd, SYS_SENSOR_AIO);    break;
        case '1': cmdSensorControl(cmd, SYS_SENSOR_MOSE);   break;
        case '3': cmdSensorControl(cmd, SYS_SENSOR_CT3919); break;
        case '5': cmdSensorControl(cmd, SYS_SENSOR_DCS);    break;
        case '7': cmdSensorControl(cmd, SYS_SENSOR_GPS);    break;
        case '8': cmdSensorControl(cmd, SYS_SENSOR_ATM1);   break;
        case '9': cmdSensorControl(cmd, SYS_SENSOR_ATM2);   break;
        case 'b':
        case 'B': cmdSensorControl(cmd, SYS_SENSOR_HMP155); break;
        case 'c':
        case 'C': cmdSensorControl(cmd, SYS_SENSOR_PTB210); break;
        case 'f':
        case 'F': cmdSensorControl(cmd, SYS_SENSOR_SHOCK); break;
        case 'r':
        case 'R': cmdSensorControl('8', '0');               break;
    }
}



int sbdrb_check(int a_option)
{
    return 1;
}



u8 fg_system_reset_req = 0;



u8 cdma_process_sms(u8 a_option)
{
    /*
    a_option
        0: idx=0
        1:  idx

    return : ret_val
        0: processing...
        1: 완료
        2: error
    */

    static u32 idx = 0;
    u8 ret_val = 0;
    char ch;
    static u8 buf_idx = 0;

    if (a_option == 0)
    {
        idx = 0;
    }

    switch (idx)
    {
        case 0:
            if (fg_sms_arrived == 1)
            {
// PRINTLINE;
                fg_sms_telno_get_ok = 0;
                fg_system_reset_req = 0;
                idx = 10;
            }
            break;

        case 10:
            if (mutex_cdma == 0)
            {
// PRINTLINE;
                mutex_cdma = 1;
PRINTVAR(mutex_cdma);
                idx = 14;
            }
            break;

//20120510111627,01033533825,4098,16,0,7265736574
        case 14:
// PRINTLINE;
            modem_rxbuf_clear();
            modem_printf("AT$SMSMT=1?\r");
            tick_sms = TM_2SEC;
            buf_idx = 0;
            idx = 25;
            break;
        case 25:
            if ( (tick_sms==0) || (modem_rxbuf_get_length() > 40) )
            {
// PRINTLINE;
                tick_sms = TM_1SEC;
                idx = 30;
            }
            break;
        case 30:
            //wait 'OK'
            if (tick_sms==0)
            {
// PRINTLINE;
                modem_rxbuf_print();

                if (modem_rxbuf_substr("7265736574") > 0)   // "7265736574" == 'reset'
                {
                    int ii = 0;
                    int jj = 0;

                    //OK
                    tick_sms = 1000/10;
                    fg_system_reset_req = 1;


                    while (1)
                    {
                        if (modem_rxbuf[ii++]==',')
                        {
                            jj++;
                            break;
                        }
                    }
                    //전화번호
                    {
                        int i = 0;
                        while (1)
                        {
                            ch = modem_rxbuf[ii++];
                            if (ch == ',')
                            {
                                jj++;
                                break;
                            }
                            else
                            {
                                if (i < 12)
                                {
                                    if ( (ch >= '0') && (ch <= '9') )
                                    {
                                        sms_tel_no[i++]=ch;
                                        sms_tel_no[i]='\0';
                                        fg_sms_telno_get_ok = 1;
                                    }
                                }
                                else
                                {

                                    break;
                                }
                            }
                        }
                        debugprintf("##TEL_NO(sms):%s\r\n",sms_tel_no);
                    }
                }
                else if (modem_rxbuf_substr("ERROR") > 0)
                {
                    idx = 50;
                    break;
                }
                idx = 40;
            }
            break;

        case 40:
// PRINTLINE;
//             idx = 14;
//             break;

        case 45:
        case 50:
            // SMS 확인 end..
            if (fg_system_reset_req == 1)
            {
PRINTLINE;
                char buf[35];
                modem_rxbuf_clear();
                modem_printf("AT$SMSMO=");

                sprintf(buf, "%s,",sms_tel_no);  //01033533825,01041988498,4098,,,,7265736574\r");
                modem_printf(buf);  //01033533825,01041988498,4098,,,,7265736574\r");

                sprintf(buf, "%s,4098,,,,7265736574\r",cdma_tel_no);  //01033533825,01041988498,4098,,,,7265736574\r");
                modem_printf(buf);  //01033533825,01041988498,4098,,,,7265736574\r");

                tick_sms = TM_5SEC;
                idx = 52;
                break;
            }
            else
            {
                fg_sms_arrived = 0;

                mutex_cdma = 0;
PRINTVAR(mutex_cdma);
                idx = 0;
            }
            break;


        case 52:
            if (tick_sms==0)
            {
                // reset
PRINTLINE;
                ctrl_sensor('R', '0');
            }
            break;

    }
    return(ret_val);
}





#endif  //ifdef USE_IRIDIUM

