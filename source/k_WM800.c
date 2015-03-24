// #include "k_config.h"

#define __K_IRIDIUM_C__

#include "k_includes.h"
#if MODULE_WM800

#define poPwrIridiumON()
#define poPwrIridiumOFF()


#define Q_IRI1_MAX  100
#define Q_IRI2_MAX  100

void iri1_rcv_q_put(char ch);
char iri1_rcv_q_get(void);

void iri2_rcv_q_put(char ch);
char iri2_rcv_q_get(void);
void iri2_rcv_q_init(void);

// int iridium_init(int a_option);
int sbdreg_check(int a_option);
int sbdixa_check(int a_option);
int sbdrb_check(int a_option);




// enum WM800_RETCODE  
// {
#define WM_OK           '0'       
#define WM_CONNECT      '1'    
#define WM_RING         '2'       
#define WM_NO_CARRIER   '3'
#define WM_ERROR        '4'      
#define WM_NO_DIATONE   '6'
#define WM_BUSY         '7'
#define WM_NO_ANSWER    '8'
// } wm800_retCode;

//static int result_no = 999;
//static int sbdring = 0;
//char result_s[30];
// char result_sbd[50];

#define MODEM_LINE_MAX  (4*40)          // 160 Byte
ALIGN4 char modem_line[MODEM_LINE_MAX];


typedef struct  {
    int resultCode;    // OK, CONNECT, RING, NO CARRIER, ERROR, NO DIAL TONE, BUSY, NO ANSWER
                       //  0,       1,    2,          3,     4,            6,    7,         8
    int sms_tx_success;
} wmResponse_T;


wmResponse_T wm800rcv;
u32 wmCmdType = 0;





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
        uart_putch( 0, ch);
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
            // Modem 초기화(전원투입 후)


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
// PRINTVAR(mutex_cdma);

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
                // debugstring("--## MODEM POWER_OFF for initialize ##--\r\n"); //(+)130905
//                cmdSensorControl(SYS_CMD_SENSOR_OFF, SYS_SENSOR_IRI1);
                // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_OFF, '1');                 //(+)130905
                // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_ON, '1');                 //(+)130905
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
            // debugstring("--## IRIDIUM-1 POWER_ON ##--\r\n");                 //(+)130905
            // cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_IRI1);
            // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_ON, '1');              //(+)130905
            // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_OFF, '1');              //(+)130905
            // tick_iri0 = 10000/10;    //10000/10;   //10sec
            tick_iri0 = 1000/10;    //10000/10;   //10sec
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
                        // debugprintf("\r\nSBD status :%d - send error\r\n",iri_retval);

                        mutex_cdma = 0;
// PRINTVAR(mutex_cdma);

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
                        // debugprintf("\r\nSBD status :%d - send success\r\n",iri_retval);

                        mutex_cdma = 0;
// PRINTVAR(mutex_cdma);

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
                        // debugprintf("\r\nSBD status :%d - Timeout...\r\n",iri_retval);

                        mutex_cdma = 0;
// PRINTVAR(mutex_cdma);
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


char cdma_tel_no[15];                   // 송신기 번호
char sms_tel_no[15]="01033533825";
// char sms_tel_no[15]="01020143428";      // 수신기 번호




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
            // debugstring("----- ATZ\r\n");
            // retry_cnt = 0;
            // idx = 12;
            idx = 80;
            break;
        case 12:
            PRINT_TIME;
            modem_rxbuf_clear();
            // modem_printf("ATZ\r");
            modem_printf("ATV0\r");
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
// PRINTLINE;
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
// PRINTLINE;
            debugstring("----- ATE0\r\n");
            modem_rxbuf_clear();
            modem_printf("ATE0\r");
            tick_iri0 = 1500/10;
            idx = 15;
            // break;
        case 15:
// PRINTLINE;
            // if (tick_iri0==0)
            // {
            //     debugstring("----- ATV1\r\n");
            //     modem_rxbuf_clear();
            //     modem_printf("ATV1\r");
            //     tick_iri0 = TM_1SEC;
            //     idx = 16;
            // }
            // break;
        case 16:

        case 75:
        case 80:
// AT$TCPWRITE=

// PRINTLINE;
            debugprintf("%s\r\n",s_msg);
// PRINTLINE;

            modem_rxbuf_clear();
            wm800rcv.sms_tx_success = 99;
            // modem_printf("AT$SMSMO=");
// PRINTLINE;


            {
                char buf[35];
                modem_rxbuf_clear();
                modem_printf("AT$SMSMO=");
                // debugprintf("AT$SMSMO=");
// PRINTLINE;

                sprintf(buf, "%s,",env.dest_no1);  //01033533825,01041988498,4098,,,,7265736574\r");
                modem_printf(buf);  
// PRINTLINE;

                sprintf(buf, "%s,4098,,,,",cdma_tel_no);  //01033533825,01041988498,4098,,,,7265736574\r");
                modem_printf(buf);  
// PRINTLINE;

            }


#if 1
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
            modem_printf("\r");
            debugprintf("\r\n");
#endif


            tick_iri0 = TM_10SEC;
            //PRINTLINE;

// PRINTLINE;
            idx = 85;
            break;
        case 85:
            if ( (tick_iri0==0) )
            {
                // Timeout !!
                idx = 900;
            }
            else
            {
                if ( 0 == wm800rcv.sms_tx_success)
                {
                    // fail
                    idx = 900;
                }
                else if ( 1== wm800rcv.sms_tx_success)
                {
                    // success
                    ret_val = 1;
                    idx = 100;
                }
            }
            break;

        case 90:
        case 92:
        case 95:
            if (tick_iri0==0)
            {
                // if (is_q_empty())
                {
                    // debugstring("q: empty --> stop sending...\r\n");
// PRINTLINE;
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
        case 105:
        case 110:
        case 112:
        case 113:
        case 114:
        case 115:
        case 120:
            ret_val = 1;
            break;
        case 900:
            ret_val = 2;
            break;
    }
    return(ret_val);
}


void wm800rcv_init(void)
{
    wm800rcv.resultCode = 0;
}

void parsing_wm800(char *a_str)
{
    ALIGN4 char wm_data[20][20];
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;
    char ch;





    while (1)
    {
        ch = a_str[k++];

        if ((ch== 0x0D) || (ch== 0x0A) || (ch=='\0') )
        {
            wm_data[i][j] = '\0';
            // debugprintf("[%d]%s\r\n",i,wm_data[i]);
            break;
        }
        else if ((ch=='\t') || (ch==':') || (ch==','))
        {
            wm_data[i][j] = '\0';
            // debugprintf("[%d]%s\r\n",i,wm_data[i]);
            i++;
            j=0;
        }
        else
        {
            if (ch != ' ')
            {
                wm_data[i][j++] = ch;
            }
        }
    }



#if 0
    for (j=0; j<=i; j++)
    {
        debugprintf("[%d]%s\r\n",j,wm_data[j]);
    }
#endif
    //PRINTLINE;

    // $XXX : Noti parsing
    if (wm_data[0][0]=='$')
    {
        // debugstring("NOTI $$$\r\n");
        u32 noti = atoi( &(wm_data[0][1]) );
        switch (noti)
        {
            case 0:
                debugprintf("CDMA power ON.\r\n");
                break;
            case 6:
                wm800rcv.sms_tx_success = 1;
                debugprintf("SMS send - OK..\r\n");
                break;
            case 7:
                wm800rcv.sms_tx_success = 0;
                debugprintf("SMS send - FAIL.\r\n");
                break;
            case 15:        // RSSI
                {
                    u32 rssi_idx = atoi( &(wm_data[1][0]) );
                    switch(rssi_idx)
                    {
                        case 0:    lblRSSI = -128; break;
                        case 1:    lblRSSI = -108; break;
                        case 2:    lblRSSI = -102; break;
                        case 3:    lblRSSI = -96;  break;
                        case 4:    lblRSSI = -90;  break;
                        case 5:    lblRSSI = -84;  break;
                        case 6:    lblRSSI = -78;  break;
                        default:   lblRSSI = -70;  break;
                    }
                    debugprintf("RSSI : %d\r\n", lblRSSI);
                }
                break;
        }
    }

    // if (strlen(&((wm_data[0][0]))) == 1)
    switch (wmCmdType)
    {
        case 0:         // 'OK'를 기다린다.
            if (strlen(&((wm_data[0][0]))) == 1)
            {
                wm800rcv.resultCode = '0';
                // PRINTVAR(wm800rcv.resultCode);
            }
            break;
        case 1:         // at$phonenum? --> 01020149049,1020149049
            if (i==1)
            {
                wm800rcv.resultCode = '0';
                strcpy(cdma_tel_no, &(wm_data[0][0]));
            }
            break;
        case 2:         // at$bwmodem? --> 010-4173-8495,1041738495,3.9,0,20150323184402MON,000001,
            if (i>3)
            {
                if (strlen( &(wm_data[4][0]) )==17)
                {
                    wm800rcv.resultCode = '0';
                    {
                        rtcTime t1;

                        t1.sec = (wm_data[4][12]-'0')*10 + (wm_data[4][13]-'0');
                        t1.min =  (wm_data[4][10]-'0')*10 + (wm_data[4][11]-'0');
                        t1.hour =  (wm_data[4][8]-'0')*10 + (wm_data[4][9]-'0');

                        t1.day =  (wm_data[4][6]-'0')*10 + (wm_data[4][7]-'0');
                        t1.week = 0;
                        t1.mon =  (wm_data[4][4]-'0')*10 + (wm_data[4][5]-'0');
                        t1.year = (wm_data[4][2]-'0')*10 + (wm_data[4][3]-'0') + 2000;

                        if (t1.year >= 2015)
                        {
                            rtc_settime(&t1);
                            set_rtc_sysclk_sync_req(0);     // request set & sync to clk source( GPS / IRIDIUM / MOSE )
                        }
                    }
                }
            }
            break;

        default:
            break;
    }
}


void task_wm800_rcv(void)
{
    static int line_ptr = 0;
    char ch;

    // static char noti_buf[30];
    // static int noti_buf_idx = 0;


    if (uart_getch(COMM_CHANNEL, &ch))
    {

        if (0 == is_special_ch(ch))
        {
            // uart_putch(0,ch);
            // debugprintf("<%02X:%c>", ch, ch);

            switch(ch)
            {
                // case 0x0A:
                case 0x0D:
                    modem_line[line_ptr++] = 0x0D;
                    modem_line[line_ptr++] = 0x0A;
                    modem_line[line_ptr] = '\0';

                    // debugstring("\r\n");
                    // debugprintf("\r\n<-%s->\r\n",modem_line);

                    // parsing
                    parsing_wm800(modem_line);
                    line_ptr = 0;
                    break;

                // case 0x0D:
                case 0x0A:
                case '\0':
                    //skip
                    break;

                default:
                    modem_line[line_ptr++] = ch;
                    break;
            }
        }
    }
}


void task_wm800_rcvold_old(void)
{
    char ch;
    int saveToBuf = 1;
    static char noti_buf[30];
    static int noti_buf_idx = 0;


    if (uart_getch(COMM_CHANNEL, &ch))
    {

#if 0
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
                case 0:             // $
                    break;
                case 1:             // $0xx, $1x, $2x, $03x, $04x, $05x,
                    switch(ch)
                    {
                        case '0':
                            noti_buf[noti_buf_idx++] = ch;
                            noti_buf[noti_buf_idx] = '\0';
                            saveToBuf = 0;
                            break;
                        default:
                            modem_rxbuf[modem_rxbuf_idx++] = '$';
                            modem_rxbuf[modem_rxbuf_idx++] = ch;
                            modem_rxbuf[modem_rxbuf_idx] = '\0';
                            noti_buf_idx = 0;
                            saveToBuf = 0;
                            break;
                    }
                    break;
                case 2:             // #000, $001, $002, $006, $ 007, $008, $013, $015, $024
                    switch(ch)
                    {
                        case '0':
                        case '1':
                        case '2':
                            noti_buf[noti_buf_idx++] = ch;
                            noti_buf[noti_buf_idx] = '\0';
                            saveToBuf = 0;
                            break;
                        default:
                            modem_rxbuf[modem_rxbuf_idx++] = noti_buf[0];
                            modem_rxbuf[modem_rxbuf_idx++] = noti_buf[1];
                            modem_rxbuf[modem_rxbuf_idx++] = ch;
                            modem_rxbuf[modem_rxbuf_idx] = '\0';
                            noti_buf_idx = 0;
                            saveToBuf = 0;
                            break;
                    }

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

    // TM_watchSMS = 0;
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
// PRINTLINE;
            // debugstring("----- init cdma : (1) atz\r\n");
            retry_cnt = 0;
            idx = 5;
            break;

        case 5:
// PRINTVAR(idx);
// PRINTLINE;

            modem_rxbuf_clear();
            // modem_printf("ATZ\r");
            // modem_printf("ATE0\r");

            wm800rcv_init();
            wmCmdType = 0;

            modem_printf("ATV0\r");
            // debugprintf("ATZ-->");
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
// PRINTLINE;
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
            else
            {
                if (wm800rcv.resultCode == WM_OK)
                {
                    wm800rcv.resultCode = 99;
                    idx = 12;
// PRINTLINE;
                }
            }
            break;

        case 12:
            modem_rxbuf_clear();

            wm800rcv_init();
            wmCmdType = 0;

            modem_printf("ATE0\r");
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
            else
            {
                if (wm800rcv.resultCode == WM_OK)
                {
                    wm800rcv.resultCode = 99;
                    idx = 16;
// PRINTLINE;
                }
            }
            break;

        case 16:
            modem_rxbuf_clear();

            wmCmdType = 0;
            modem_printf("AT$NOTIFY=11101000\r");
            // debugprintf("AT$NOTIFY=11101000\r\n");
                                                    // 0 ON  Ready
                                                    // 1 ON  Out of service
                                                    // 2 ON  Battery Voltage
                                                    // 3 OFF Protocol revision
                                                    // 4 ON  RSSI
                                                    // 5 OFF Reserved
                                                    // 6 OFF Reserved
                                                    // 7 OFF Power save mode start/exit
            tick_iri1 = 1500/10;
            idx = 17;
            break;
        case 17:
            if (tick_iri1==0)
            {
                modem_rxbuf_print();
                idx = 18;
            }
            else
            {
                if (wm800rcv.resultCode == WM_OK)
                {
                    wm800rcv.resultCode = 99;
                    idx = 18;
// PRINTLINE;
                }
            }
            break;

        case 18:
            modem_rxbuf_clear();

            wmCmdType = 0;
            modem_printf("AT$ALERT=0,0,0,0,0,0,0,0,0,0,0\r");   // 경고음 OFF
            // debugprintf("AT$ALERT=0,0,0,0,0,0,0,0,0,0,0\r");   // 경고음 OFF
            tick_iri1 = 1500/10;
            idx = 19;
            break;
        case 19:
            if (tick_iri1==0)
            {
                modem_rxbuf_print();
                idx = 20;
            }
            else
            {
                if (wm800rcv.resultCode == WM_OK)
                {
                    wm800rcv.resultCode = 99;
                    idx = 20;
// PRINTLINE;
                }
            }
            break;

        case 20:
        case 30:
            //AT$SIOFLOW=1   // 흐름제어 없음
            //debugstring("----- init cdma : (2) AT$SIOFLOW=1\r\n");
            modem_rxbuf_clear();

            wmCmdType = 0;
            modem_printf("AT$SIOFLOW=1\r");
            // debugprintf("AT$SIOFLOW=1\r");
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
            else
            {
                if (wm800rcv.resultCode == WM_OK)
                {
                    wm800rcv.resultCode = 99;
                    idx = 42;
// PRINTLINE;
                }
            }
            break;
        case 42:
            // phone number
            modem_rxbuf_clear();

            wmCmdType = 1;
            modem_printf("AT$PHONENUM?\r");
            // debugprintf("AT$PHONENUM?\r");
            // tick_iri1 = 5000/10;    // 2sec
            tick_iri1 = 10000/10;    // 2sec
            idx = 44;
            break;

        case 44:
            if ( (tick_iri1==0)  )
            {
// PRINTLINE;
               idx = 46;
               tick_iri1 = 500/10;
// PRINTLINE;
            }
            else
            {
                if (wm800rcv.resultCode == WM_OK)
                {
                    debugprintf("phone_No: %s\r\n", cdma_tel_no);
                    wm800rcv.resultCode = 99;
                    idx = 46;
                }
            }
            break;

        case 46:
            // phone number
            modem_rxbuf_clear();

            wmCmdType = 2;
            modem_printf("AT$BWMODEM?\r");
            // debugprintf("AT$BWMODEM?\r");
            // tick_iri1 = 5000/10;    // 2sec
            tick_iri1 = 10000/10;    // 2sec
            idx = 48;
            break;

        case 48:
            if ( (tick_iri1==0)  )
            {
// PRINTLINE;
               idx = 50;
               tick_iri1 = 500/10;
// PRINTLINE;
            }
            else
            {
                if (wm800rcv.resultCode == WM_OK)
                {
                    //debugprintf("phone_No: %s\r\n", cdma_tel_no);
                    PRINT_TIME;
                    wm800rcv.resultCode = 99;
                    idx = 50;
                }
            }
            break;




        case 50:
            if (tick_iri1==0)
            {
                //AT$KEYE  // 전화 끊기
                // debugstring("----- init cdma : (3) AT$KEYE\r\n");
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
            // //AT$TCPCLOSE   //TCP socket close
            // //debugstring("----- init cdma : (4) AT$TCPCLOSE\r\n");
            // modem_rxbuf_clear();
            // modem_printf("AT$TCPCLOSE\r");
            // tick_iri1 = TM_10SEC;
            // idx = 75;
            // break;
        case 75:
            // if ( (tick_iri1==0) || (modem_rxbuf_get_length() > 11) )
            // {
            //     tick_iri1 = TM_1SEC;
            //     idx = 80;
            // }
            // break;
        case 80:
            // if (tick_iri1==0)
            // {
            //     /*
            //     if (modem_rxbuf_get_length() > 0)
            //     {
            //         modem_rxbuf_print();
            //         modem_rxbuf_printHex();
            //     }
            //     */
            //     idx = 90;
            // }
            // break;

        case 90:
            // // AT$TCPEXIT   //호 단절
            // //debugstring("----- init cdma : (5) AT$TCPEXIT\r\n");
            // modem_rxbuf_clear();
            // modem_printf("AT$TCPEXIT\r");
            // tick_iri1 = TM_10SEC;
            // idx = 95;
            // break;
        case 95:
            // if ( (tick_iri1==0) || (modem_rxbuf_get_length() > 5) )
            // {
            //     tick_iri1 = TM_1SEC;
            //     idx = 100;
            // }
            // break;
        case 100:
            // if (tick_iri1==0)
            // {
            //     /*
            //     if (modem_rxbuf_get_length() > 0)
            //     {
            //         modem_rxbuf_print();
            //         modem_rxbuf_printHex();
            //     }
            //     */
            //     idx = 110;
            // }
            // break;
        case 110:
            ret_val = 1;
            break;
        case 900:
            ret_val = 2;
            break;
    }
    return(ret_val);
}



// int iridium_init(int a_option)
// {}


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

    // switch(a_id)
    // {
    //     case '0': env.s0 = a_code;  break;
    //     case '1': env.s1 = a_code;  break;
    //     case '3': env.s3 = a_code;  break;
    //     case '5': env.s5 = a_code;  break;
    //     case '7': env.s7 = a_code;  break;
    //     case '8': env.s8 = a_code;  break;
    //     case '9': env.s9 = a_code;  break;
    //     case 'a':
    //     case 'A': env.sa = a_code;  break;
    //     case 'b':
    //     case 'B': env.sb = a_code;  break;
    //     case 'c':
    //     case 'C': env.sc = a_code;  break;
    //     case 'e':
    //     case 'E': env.se = a_code;  break;
    //     case 'f':
    //     case 'F': env.sf = a_code;  break;
    // }
    env_save();
}

void ctrl_sensor(char a_id, char a_code)
{
    // char cmd = '1';
    // //'0' : Reset, '1' : On, '2' : Off

    // switch (a_code)
    // {
    //     case '0':   cmd = '4';  break;  // reset
    //     case '1':   cmd = '1';  break;  // on
    //     case '2':   cmd = '0';  break;  // off
    // }

    // debugstring("+++ control : ");
    // switch(a_id)
    // {
    //     case '0': debugstring("AIO");    break;
    //     case '1': debugstring("MOSE");   break;
    //     case '3': debugstring("CT3919"); break;
    //     case '5': debugstring("DCS");    break;
    //     case '7': debugstring("GPS");    break;
    //     case '8': debugstring("ATM1");   break;
    //     case '9': debugstring("ATM2");   break;
    //     case 'b':
    //     case 'B': debugstring("HMP155"); break;
    //     case 'c':
    //     case 'C': debugstring("PTB210"); break;
    //     case 'f':
    //     case 'F': debugstring("SHOCK"); break;
    //     case 'r':
    //     case 'R': debugstring("SYSTEM"); break;
    // }
    // if      (a_code == '0')      debugstring(" RESET");
    // else if (a_code == '1')      debugstring(" ON");
    // else if (a_code == '2')      debugstring(" OFF");
    // debugstring("\r\n");

    // switch(a_id)
    // {
    //     case '0': cmdSensorControl(cmd, SYS_SENSOR_AIO);    break;
    //     case '1': cmdSensorControl(cmd, SYS_SENSOR_MOSE);   break;
    //     case '3': cmdSensorControl(cmd, SYS_SENSOR_CT3919); break;
    //     case '5': cmdSensorControl(cmd, SYS_SENSOR_DCS);    break;
    //     case '7': cmdSensorControl(cmd, SYS_SENSOR_GPS);    break;
    //     case '8': cmdSensorControl(cmd, SYS_SENSOR_ATM1);   break;
    //     case '9': cmdSensorControl(cmd, SYS_SENSOR_ATM2);   break;
    //     case 'b':
    //     case 'B': cmdSensorControl(cmd, SYS_SENSOR_HMP155); break;
    //     case 'c':
    //     case 'C': cmdSensorControl(cmd, SYS_SENSOR_PTB210); break;
    //     case 'f':
    //     case 'F': cmdSensorControl(cmd, SYS_SENSOR_SHOCK); break;
    //     case 'r':
    //     case 'R': cmdSensorControl('8', '0');               break;
    // }
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
                debugprintf("AT$SMSMO=");

                sprintf(buf, "%s,",sms_tel_no);  //01033533825,01041988498,4098,,,,7265736574\r");
                modem_printf(buf);  //01033533825,01041988498,4098,,,,7265736574\r");
                debugprintf(buf);  //01033533825,01041988498,4098,,,,7265736574\r");

                sprintf(buf, "%s,4098,,,,7265736574\r",cdma_tel_no);  //01033533825,01041988498,4098,,,,7265736574\r");
                modem_printf(buf);  //01033533825,01041988498,4098,,,,7265736574\r");
                debugprintf(buf);  //01033533825,01041988498,4098,,,,7265736574\r");

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
                // ctrl_sensor('R', '0');

                mutex_cdma = 0;
                idx = 0;

            }
            break;

    }
    return(ret_val);
}





#endif  //if MODULE_WM800

