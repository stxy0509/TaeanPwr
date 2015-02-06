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




// static int port_no = PORT_WM215;
void change_iridium(void)
{
}
void restore_iri_port(void)
{
    iri_port = PORT_WM215;
}



#define IRIDIUM_CMD_POWER_ON	1
#define IRIDIUM_CMD_POWER_OFF	2
#define IRIDIUM_CMD_SEND_NEW	3
#define IRIDIUM_CMD_SEND_OLD	4


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

static int mt1_status = 0;
static int mt2_status = 0;
int  get_mt1_status(void)       {   return mt1_status;   }
int  get_mt2_status(void)       {   return mt2_status;   }
int  get_mt_status(void)
{
    if (iri_port == C_IRIDIUM_1)
    {
        return mt1_status;
    }
    else
    {
        return mt2_status;
    }
}
void set_mt1_status(int a_val)   {   mt1_status = a_val; }
void set_mt2_status(int a_val)   {   mt2_status = a_val; }
void set_mt_status(int a_val)
{
    if (iri_port == C_IRIDIUM_1)
    {
        mt1_status = a_val;
    }
    else
    {
        mt2_status = a_val;
    }
}

static int ring1 = 0;
static int ring2 = 0;
void set_sbdring1(int a_val)   {   ring1 = a_val; }
void set_sbdring2(int a_val)   {   ring2 = a_val; }
int  get_sbdring1(void)       {   return ring1;   }
int  get_sbdring2(void)       {   return ring2;   }


char ctrl_msg[50];

// static int iri_init_time_10sec = 1;     //맨 처음에는 10초만 전원 OFF 한다.

//----------------------------------------
// task_iridium()
// 1    : OK
//
// 2    : Timeout error
// 0    : NG
// 999  : Waiting...
//----------------------------------------
void task_iridium(void)
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

            if ((get_iri1_init() == 1) )
            {
                idx = 500;      break;
            }

            #if 0
            // read mail
            if (get_sbdring1() > 0)
            {
                iri_port=C_IRIDIUM_1; idx = 200;  break;
            }
            if (get_sbdring2() > 0)
            {
                iri_port=C_IRIDIUM_2; idx = 200;  break;
            }

            // if (get_mt1_status() > 0)    //(-)130904
            if (get_mt1_status() == 1)      //(+)130904
            {
                iri_port=C_IRIDIUM_1; idx = 300;  break;
            }
            // if (get_mt2_status() > 0)    //(-)130904
            if (get_mt2_status() == 1)      //(+)130904
            {
                iri_port=C_IRIDIUM_2; idx = 300;  break;
            }
            #endif

            // send message
            // if ( (0 == is_q_empty()) || (1==fgMsgC_ready) )    // not empty...
            if ( (0 == is_q_empty()) )    // not empty...
            {
                if (get_fgFW_updating())
                    return;

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
                break;
            }

            #if 0
            // SBDREG chk
            if(get_tm_sbdreg()==1)
            //if(get_tm_sbdreg() > (1*60))
            {
                set_tm_sbdreg(0);
                idx = 700;
                break;
            }
            #endif
            break;


            //---------------------------------------------------
        case 400:
            iridium_Process_1(1);
            idx = 900;
            break;

            //---------------------------------------------------// iridium-1 init
        case 500:
            // power OFF
            if ((get_iri1_init() == 1))
            {
                debugstring("--## MODEM POWER_OFF for initialize ##--\r\n"); //(+)130905
                // cmdSensorControl(SYS_CMD_SENSOR_OFF, SYS_SENSOR_IRI1);
                // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_ON, '1');                 //(+)130905
            }

            // if (iri_init_time_10sec == 1)
                tick_iri0 = 3000/10;    //10000/10;   //5 sec
            // else
                // tick_iri0 = 60000/10;    //60000/10;   //60sec

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
            }
            break;
        case 520:
            debugstring("--## MODEM POWER_ON ##--\r\n");                 //(+)130905
            // cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_IRI1);
            // cmdSensorControl(SYS_CMD_IRIDIUM_EXT_OFF, '1');              //(+)130905
            tick_iri0 = 10000/10;    //10000/10;   //10sec
            idx = 525;
            break;
        case 525:
            if (tick_iri0 == 0)
            {
                // iri_port = C_IRIDIUM_1;
                iridium_init(1);
                idx = 530;
            }
            break;
        case 530:
            if (iridium_init(0)==1)
            {
                // init IR1 is ended.
                set_iri1_init(0);

                {
                    idx = 8000;
                }
            }
            break;

        case 800:
            idx = 8000;
            break;

            //---------------------------------------------------// wait Result
        case 900:
            {
                //int i;
                int iri_retval = iridium_Process_1(0);  //0: 상태체크용...
                switch (iri_retval)
                {
                    case 0: //err
                        debugprintf("\r\nSBD status :%d - send error\r\n",iri_retval);

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
    debugprintf("####### init MODEM [");
    // if ( iri_port==C_IRIDIUM_1)
    {
        debugstring("1]\r\n");
        set_iri1_init(1);
    }
    // else if ( iri_port==C_IRIDIUM_2)
    {
        // debugstring("2]\r\n");
        // set_iri2_init(1);
    }
}


char iri_response[50];
int mo_status;
//int mt_status;
int csq;
//----------------------------------------
// iridium_Process_1()
// 1 	: OK
//
// 2	: Timeout error
// 0 	: NG
// 999  : Waiting...
//----------------------------------------
int MODEM_Process(void)
{
    //static u32 idx = 0;
    static u32 m_stage = 7000;  //1000;
    static u32 s_stage = 0;

    static int resp_idx = 0;
    
    // static int retry_cnt = 0;
    //static char csq_value;
    //static char sbdi_value;
    // static int sbdi_error;

    // static int at_cnt = 0;;

    // static int chg_n_retry=0;   // 이리듐을 교체하여  ATV 재시도 했는가??
    // static int chg_atv_retry=0;   // 이리듐을 교체하여  ATV 재시도 했는가??
    // static int chg_csq_retry=0;   // 이리듐을 교체하여  CSQ 재시도 했는가??
    // static int chg_sbdi_retry=0;  // 이리듐을 교체하여 SBDI 재시도 했는가??
    char ch;
    // int i;
    // int ret_val = 999;
    //char s_msg[100];


    // return ret_val;


/*
    switch (a_option)
    {
        case 1:
            // 새로 시작
            idx = 0;
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

*/

    // ----- READ UART BUFFER ----------------------------------------

    while(1)
    {
        if (uart_getch(PORT_WM215,&ch))
        {
            iri_response[resp_idx++] = ch;
#if 1            
            uart_putch(0, ch);
#else            
            debugprintf("%02X ", ch);
            if (ch == 0x0A)
            {
                debugprintf("\r\n");
            }
#endif
        }
        else
        {
            break;
        }
    }




    switch (m_stage)
    {
        // ----- +ZREADY ---------------------------------------------
        case 1000:






        // ----- INIT ------------------------------------------------
        case 2000:
            s_stage = 0;
            m_stage = 2010;
            break;

        case 2010:
            resp_idx = 0;

            if (s_stage == 0)
            {
                iridium_printf("ATE0\r\n");
                debugprintf("ATE0 ---> ");
            }
            else
            {
                iridium_printf("ATV1\r\n");
                debugprintf("ATV1 ---> ");
            }

            tick_iri0 = TM_10SEC;
            m_stage = 2020;
            break;
        
        case 2020:
            //wait 'OK'
            if (resp_idx > 5)
            {
                if (iri_response[3] == 'K')
                {
                    // debugprintf("-OK-\r\n");
                    s_stage++;
                }
                tick_iri0 = TM_1SEC;
                m_stage = 2030;
            }
            if (tick_iri0==0)
            {
                // over 10 sec
                m_stage = 2000;
            }
            break;

        case 2030:
            if (tick_iri0==0)
            {
                switch(s_stage)
                {
                    case 0:
                        m_stage = 2000;
                        break;
                    case 1:
                        m_stage = 2010;
                        break;
                    default:
                        m_stage = 3000;
                        break;
                        break;
                }
            }
            break;


        // ----- +ZIPCALL? (chk Data Call)---------------------------
        case 3000:
            s_stage = 0;
            m_stage = 3010;
            break;

        case 3010:
            resp_idx = 0;

            iridium_printf("AT+ZIPCALL?\r\n");
            debugprintf("AT+ZIPCALL? ---> ");

            tick_iri0 = TM_20SEC;
            m_stage = 3020;
            break;
        
        case 3020:
            //wait 'OK'
            if (resp_idx > 14)
            {
                if (iri_response[12] == '1')
                {
                    //debugprintf("-OK-\r\n");
                    s_stage = 0;
                }
                else
                {
                    s_stage = 1;
                }
                tick_iri0 = TM_1SEC;
                m_stage = 3030;
            }
            if (tick_iri0==0)
            {
                // over 20 sec
                m_stage = 2000;
            }
            break;

        case 3030:
            if (tick_iri0==0)
            {
                switch(s_stage)
                {
                    case 0:
                        // +zipcall = 1
                        m_stage = 4000; 
                        break;
                    default:
                        m_stage = 3040;
                        break;
                }
            }
            break;


        // ----- +ZIPCALL=1 (setup Data Call)-------------------------
        case 3040:
            resp_idx = 0;

            iridium_printf("AT+ZIPCALL=1\r\n");
            debugprintf("AT+ZIPCALL=1 ---> ");

            tick_iri0 = TM_20SEC;
            m_stage = 3020;
            break;
        




        // ----- +ZIPSTAT=1 (chk socket opened)------------------------
        case 4000:
            s_stage = 0;
            m_stage = 4010;
            break;

        case 4010:
            resp_idx = 0;

            iridium_printf("AT+ZIPSTAT=1\r\n");
            debugprintf("AT+ZIPSTAT=1 ---> ");

            tick_iri0 = TM_20SEC;
            m_stage = 4020;
            break;
        
        case 4020:
            //wait 'OK'
            if (resp_idx > 20)
            {
                debugprintf("socket is ");
                if (iri_response[14] == '1')
                {
                    debugprintf("opened.\r\n");
                    s_stage = 0;
                }
                else
                {
                    debugprintf("closed.\r\n");
                    s_stage = 1;
                }
                tick_iri0 = TM_1SEC;
                m_stage = 4030;
            }
            if (tick_iri0==0)
            {
                // over 20 sec
                m_stage = 2000;
            }
            break;

        case 4030:
            if (tick_iri0==0)
            {
                switch(s_stage)
                {
                    case 0:
                        m_stage = 5000;     // opened
                        break;
                    default:
                        m_stage = 6000;     // closed
                        break;
                }
            }
            break;





        // ----- +ZIPSEND ----------------------------

        case 5000:      //send
            s_stage = 0;
            m_stage = 5010;
            break;

        case 5010:
            resp_idx = 0;

            debugprintf("len= %d\r\n", strlen(newMsg));

            iridium_printf("AT+ZIPSEND=1,");  //34
            debugprintf("AT+ZIPSEND=1,data ---> ");  //34
            iridium_printf(newMsg);

            tick_iri0 = TM_20SEC;
            m_stage = 5020;
            break;
        
        case 5020:
            //wait 'OK'
            if (resp_idx > 20)
            {
                if (iri_response[3] == 'K')
                {
                    // send ok
                    s_stage = 0;
                }
                else
                {
                    // dens fail
                    s_stage = 1;
                }
                tick_iri0 = TM_1SEC;
                m_stage = 5030;
            }
            if (tick_iri0==0)
            {
                // over 20 sec
                m_stage = 2000;
            }
            break;

        case 5030:
            if (tick_iri0==0)
            {
                switch(s_stage)
                {
                    case 0:
                        q_pop();
                        debugprintf("q_pop --> Q[%d]\r\n",is_q_dataNum());

                        if ( (0 == is_q_empty()) )    
                        {
                            // not empty...
                            m_stage = 7000;
                        }
                        else
                        {
                            // empty...
                            m_stage = 8000;     // ---> ZIPCLOSE
                        }
                        break;
                    default:
                        m_stage = 2000;     
                        break;
                }
            }
            break;







        // ----- +ZIPOPEN=1,0,1.214.193.188,2222----------------------
        case 6000:      // open
            s_stage = 0;
            m_stage = 6010;
            break;

        case 6010:
            resp_idx = 0;

#if 0
            //ezTerm
            iridium_printf("AT+ZIPOPEN=1,0,1.214.193.188,2222\r\n");
            debugprintf("AT+ZIPOPEN=1,0,1.214.193.188,2222 ---> ");
#else
            iridium_printf("AT+ZIPOPEN=1,0,124.243.127.180,5555\r\n");
            debugprintf("AT+ZIPOPEN=1,0,124.243.127.180,5555 ---> ");
#endif
            tick_iri0 = TM_20SEC;
            m_stage = 6020;
            break;
        
        case 6020:
            //wait 'OK'
            if (resp_idx > 20)
            {
                debugprintf("socket is ");
                if (iri_response[14+6] == '1')
                {
                    debugprintf("opened.\r\n");
                    s_stage = 0;
                }
                else
                {
                    debugprintf("closed.\r\n");
                    s_stage = 1;
                }
                tick_iri0 = TM_1SEC;
                m_stage = 6030;
            }
            if (tick_iri0==0)
            {
                // over 20 sec
                m_stage = 2000;
            }
            break;

        case 6030:
            if (tick_iri0==0)
            {
                switch(s_stage)
                {
                    case 0:
                        m_stage = 5000; //7000;     // opened
                        break;
                    default:
                        m_stage = 2000;     // closed
                        break;
                }
            }
            break;



        // -------------------------------------------------------------------------------
        case 7000:
            if ( (0 == is_q_empty()) )    // not empty...
            {
                // if (get_fgFW_updating())
                    // return;

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
                m_stage = 2000;
            }
            break;


        // -------------------------------------------------------------------------------
        case 8000:
            s_stage = 0;
            m_stage = 8010;
            break;

        case 8010:
            resp_idx = 0;

            switch (s_stage)
            {
                case 0:
                    iridium_printf("AT+ZIPCLOSE=1\r\n");
                    debugprintf("AT+ZIPCLOSE=1 ---> ");
                    break;
                default:
                    iridium_printf("AT+ZIPCALL=0\r\n");
                    debugprintf("AT+ZIPCALL=0 ---> ");
                    break;
            }

            tick_iri0 = TM_20SEC;
            m_stage = 8020;
            break;

        case 8020:
            //wait 'OK'
            if (resp_idx > 8)
            {
                tick_iri0 = TM_2SEC;
                m_stage = 8030;
            }
            if (tick_iri0==0)
            {
                // over 20 sec
                m_stage = 8030;
            }
            break;

        case 8030:

            if (tick_iri0==0)
            {
                switch(s_stage)
                {
                    case 0:
                        s_stage = 1;
                        m_stage = 8010;     // opened
                        break;
                    default:
                        m_stage = 7000;     // closed
                        break;
                }
            }

            break;
    }
    return(1);
}


int iridium_Process_1(int a_option)
{
    static int idx = 0;
    static int resp_idx = 0;
    static int retry_cnt = 0;
    //static char csq_value;
    //static char sbdi_value;
    // static int sbdi_error;

    static int at_cnt = 0;;

    static int chg_n_retry=0;   // 이리듐을 교체하여  ATV 재시도 했는가??
    static int chg_atv_retry=0;   // 이리듐을 교체하여  ATV 재시도 했는가??
    static int chg_csq_retry=0;   // 이리듐을 교체하여  CSQ 재시도 했는가??
    static int chg_sbdi_retry=0;  // 이리듐을 교체하여 SBDI 재시도 했는가??
    char ch;
    int i;
    int ret_val = 999;
    //char s_msg[100];


    // return ret_val;



    switch (a_option)
    {
        case 1:
            // 새로 시작
            idx = 0;
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
            // restore_iri_port();

            // debugprintf("\r\nIridium-[%d] is active.\r\n", ((iri_port==C_IRIDIUM_1) ? 1:2) );
            idx = 450;
            break;

        case 450:
// PRINTVAR(idx);            
            resp_idx = 0;
            // while(1) {  if (uart_getch(3, &ch) == 0)   break; }
            debugprintf("ATE0 ---> ");
            iridium_printf("ATE0\r\n");
            tick_iri0 = 1000/10;    //1000/10;   //1sec
            idx = 500;
            break;
        case 500:
            //wait 'OK'
            if (wm_rcv_get(&ch))
            {
                iri_response[resp_idx++] = ch;
                // debugprintf("%c",ch);
                if ( (ch=='K') || (ch=='0') )
                {
                    debugprintf("-OK-\r\n");
                    idx = 550;
                }
            }
            if (tick_iri0==0)
            {
                idx = 505;
            }
            break;

        case 505:
// PRINTVAR(idx);            
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
        case 20:
        case 30:
        case 56:
            wm_rcv_q_init();
            resp_idx = 0;

            debugstring("AT+ZIPCALL=1 ---> ");

            //PRINT_TIME;
            iridium_printf("AT+ZIPCALL=1\r\n");
            tick_iri0 = 20000/10;    //5sec
            idx = 57;
            break;
        case 57:
            //wait 'OK'
            if (wm_rcv_get(&ch))
            // if (uart_getch(3, &ch))
            {
                uart_putch(0, ch);
            // if (iri_rcv_get(iri_port,&ch))
            // {
                // debugprintf("%c",ch);
                if ((ch=='K') || (ch=='0') )
                {
                    // debugprintf("[OK]\r\n");
                    idx = 60;

                    //initial RETRY_CNT:  in case of 'CSQ=0'
                    retry_cnt = 0;
                }
                else if ((ch == 'O') || (ch == '4') ) // ERROR
                {
                    // idx = 114;
                    idx = 60;

                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                //debugstring("----AT+SBDMTA=1 NG\r\n");
                idx = 999;
            }
            break;


            // AT+CSQ? ------------------------------------------------AT+CSQ?
        case 60:
            wm_rcv_q_init();
            // debugprintf("AT+ZIPOPEN=1,0,1.214.193.188,2222 ---> ");
            debugprintf("AT+ZIPOPEN=1,0,1.214.193.188,5005 ---> ");

            //PRINT_TIME;
            resp_idx = 0;
            iridium_printf("AT+ZIPOPEN=1,0,1.214.193.188,5005\r\n");
            // iridium_printf("AT+ZIPOPEN=1,0,1.214.193.188,2222\r\n");
            tick_iri0 = 20000/10;  //10sec
            idx = 62;
            break;
        case 62:
            //wait 'OK'
            if (wm_rcv_get(&ch))
            {
                uart_putch(0, ch);

                if ((ch==',') )         // +ZIPSTAT: 1,1
                {
                    idx = 100;
                }
                else if ((ch=='R') || (ch == '4')  )         // ERROR
                {
                    // idx = 999;
                    idx = 100;
                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                idx = 999;
            }
            break;



            // AT+SBDWB : data transfer to ISU  ------------------------------------------------AT+SBDWB
        case 100:
            wm_rcv_q_init();
            debugstring("\r\n-----AT+ZIPSEND=1,data\r\n");
            //PRINT_TIME;

            resp_idx = 0;
            // iridium_printf("AT+ZIPSEND=1,4542E10790D045AEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCF6BB9C3D6BB9C3DE7F3DAEE70F5AEE70F79FCE81FB601F97475C0000207ED807E5D1D70603081FB601F97475C180C207ED807E5D1D70542A81FB601F97475C150AA07ED807E5D1D70542A81FB601F97475C0000207ED807E5D1D70000081FB601F97475C0000207ED807E5D1D70000081FB601F97475C0000207ED807E5D1D70542A81FB601F97475C180C207ED807E5D1D70603081FB601F97475C0000207ED807E5D1D70000081FB601F97475C0000207ED807E5D1D706C3681FB601F97475C1B0DA07ED807E5D1D706C3681FB601F97475C1B0DA07ED807E5D1D70000081FB601F97475C00003D8CCCCCCCC\r\n");  //34

            {
                // u32 chksum=0;
                char ch;
                
                int len = strlen(newMsg);

                debugprintf("len= %d\r\n", len);

                iridium_printf("AT+ZIPSEND=1,");  //34

                for (i=0; i<len+1; i++)
                {
                    ch = newMsg[i];
                    uart_putch(PORT_WM215, ch);
                    uart_putch(0, ch);
                }
                iridium_printf("\r\n");

                tick_iri0 = 20000/10;  //120sec
                idx = 108;
            }
            break;

        case 108:
            //wait 'OK'
            if (wm_rcv_get(&ch))
            {
                uart_putch(0, ch);

                if ((ch==',') )         // +ZIPSEND: 1,1
                {
                    idx = 109;
                }
                else if ((ch=='O') || (ch == '4')  )         // ERROR
                {
                    idx = 999;
                }
            }
            else if (tick_iri0==0)
            {
                debugprintf("[NG]\r\n");
                idx = 999;
            }
            break;
   
       case 109:
            if (tick_iri0==0)
            {
                // debugstring("-----AT+SBDWB timeout!!\r\n");
                idx = 999;
                // idx = 112;
            }
            break;

        case 110:
        case 112:
            for (i=0;i<10;i++) iri_rcv_get(iri_port,&ch);
            debugprintf("\r\n-AT+ZIPCLOSE=1\r\n");
            //PRINT_TIME;

            resp_idx=0;
            iridium_printf("AT+ZIPCLOSE=1\r\n");
            // iridium_printf("AT+SBDIXA\r\n");
            tick_iri0 = 5000/10;  //120sec
            idx = 113;
            break;
        case 113:
            for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            if (tick_iri0==0)
            {
                debugprintf("\r\n-AT+ZIPCALL=0\r\n");
                //PRINT_TIME;

                resp_idx=0;
                iridium_printf("AT+ZIPCALL=0\r\n");
                // iridium_printf("AT+SBDIXA\r\n");
                tick_iri0 = 5000/10;  //120sec
                idx = 114;

            }
            break;

        case 114:
            for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            if (tick_iri0==0)
            {
                debugprintf("\r\nAT+CFUN=1,1\r\n");
                //PRINT_TIME;

                resp_idx=0;
                iridium_printf("AT+CFUN=1,1\r\n");
                // iridium_printf("AT+SBDIXA\r\n");
                tick_iri0 = 5000/10;  //120sec
                idx = 999;

            }
            break;



        case 999:
PRINTVAR(idx);            
            //timeout error
            ret_val = 2;

            change_iridium();
            break;
    }
    return(ret_val);
}




//static int result_no = 999;
//static int sbdring = 0;
char result_s[30];
char result_sbd[50];

void task_iri_rcv(void)
{
    // static int idx1=0;
    // static int idx2=0;
    char ch;

    //-----1-----------------------------------------
    if (uart_getch(PORT_WM215,&ch))
    {
        // uart_putch(PORT_WM215,ch);
        
        iri1_rcv_q_put(ch);

        // switch(idx1)
        // {
        //     case 0:
        //         if (ch=='1')
        //             idx1 = 10;
        //         break;
        //     case 10:
        //         if (ch=='2')
        //             idx1 = 20;
        //         else
        //             idx1 = 0;
        //         break;
        //     case 20:
        //         if (ch=='6')
        //         {
        //             debugprintf("\r\n---------------------->>>> SBDRING(1) >>>>\r\n");
        //             set_sbdring1(1);

        //             idx1 = 70;
        //         }
        //         else
        //             idx1 = 0;
        //         break;
        //     case 70:
        //     default:
        //         idx1 = 0;
        //         break;
        // }
    }





    // static int idx = 0;

    // switch (idx)
    // {
    //     case 1000:
    //         break;

    // }
}





//-------------------------------------------------------------------
t_SENSOR_Q iri1_q;
t_SENSOR_Q iri2_q;

ALIGN4 char iri1_q_buf[Q_IRI1_MAX];
ALIGN4 char iri2_q_buf[Q_IRI2_MAX];


int wm_rcv_get(char *ch)
{
    char tmp;
    // if (no == C_IRIDIUM_1)
    {
        tmp = iri1_rcv_q_get();
    }
    // else
    // {
    //     tmp = iri2_rcv_q_get();
    // }

    if (tmp != -1)
    {
        *ch = tmp;
        return(1);
    }
    else
    {
        return(0);
    }
}



int iri_rcv_get(int no, char *ch)
{
    char tmp;
    // if (no == C_IRIDIUM_1)
    {
        tmp = iri1_rcv_q_get();
    }
    // else
    // {
    //     tmp = iri2_rcv_q_get();
    // }

    if (tmp != -1)
    {
        *ch = tmp;
        return(1);
    }
    else
    {
        return(0);
    }
}

void iri1_rcv_q_put(char ch)
{
    iri1_q_buf[iri1_q.rear] = ch;
    ++iri1_q.rear;
    iri1_q.rear %= Q_IRI1_MAX;
    iri1_q.len++;
}

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

void wm_rcv_q_init(void)
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

void iri2_rcv_q_put(char ch)
{
    iri2_q_buf[iri2_q.rear] = ch;
    ++iri2_q.rear;
    iri2_q.rear %= Q_IRI2_MAX;
    iri2_q.len++;
}

char iri2_rcv_q_get(void)
{
    char ch;
    if (iri2_q.front == iri2_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = iri2_q_buf[iri2_q.front];
        ++iri2_q.front;
        iri2_q.front %= Q_IRI2_MAX;
        iri2_q.len--;
    }
    return ch;
}

void iri2_rcv_q_init(void)
{
    iri2_q.rear = 0;
    iri2_q.front = 0;
    iri2_q.len = 0;
}

u32 get_iri2_rcv_len(void)
{
    return iri2_q.len;
}






int iridium_init(int a_option)
{
    static int idx = 0;
    static int resp_idx = 0;
    // static int retry_cnt = 0;
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
            debugprintf("\r\nmodem init start.\r\n");
            idx = 450;
            break;

        case 450:
            resp_idx = 0;
            wm_rcv_q_init();

            debugprintf("ATE0 ---> ");
            iridium_printf("ATE0\r\n");
            tick_iri0 = 3000/10;    //1000/10;   //1sec
            idx = 500;
            break;

        case 500:
            //wait 'OK'
            if (wm_rcv_get(&ch))
            {
                // uart_putch(0,ch);

                iri_response[resp_idx++] = ch;
                if ((ch=='K') || (ch=='0'))
                {
                    debugprintf("-OK-\r\n");
                    idx = 550;
                }
            }
            else if (tick_iri0==0)
            {
                idx = 505;
            }
            break;

        case 505:
            at_cnt++;
            if (at_cnt >5)
            {
                idx = 20;   //550;
            }
            else
            {
                idx = 450;
            }
            break;

        case 550:
        //     resp_idx = 0;
        //     wm_rcv_q_init();

        //     debugprintf("ATV0 ---> ");
        //     iridium_printf("ATV0\r\n");
        //     tick_iri0 = 5000/10;    //10000/10;   //10sec
        //     idx = 2;
        //     break;

        // case 2:
        //     //wait 'OK'
        //     if (wm_rcv_get(&ch))
        //     {
        //         iri_response[resp_idx++] = ch;
        //         if ( (ch==0x0A) )
        //         {
        //             debugprintf(iri_response);
        //             idx = 20;
        //         }
        //     }
        //     else if (tick_iri0==0)
        //     {
        //         debugprintf("[NG]\r\n");
        //         idx = 999;
        //     }
        //     break;

            //ATE0 ------------------------------------------------ATE0
        case 20:
        //     // while(1) {  if (uart_getch(3, &ch) == 0)   break; }
        //     wm_rcv_q_init();

        //     debugstring("ATE0 ---> ");

        //     //PRINT_TIME;
        //     iridium_printf("ATE0\r");
        //     tick_iri0 = 5000/10;    //5sec
        //     idx = 25;
        //     break;
        // case 25:
        //     //wait 'OK'
        //     if (uart_getch(3, &ch))
        //     {
        //         uart_putch(0, ch);
        //     // if (UartGetCh(iri_port,&ch))
        //     // {
        //         // debugprintf("%c",ch);
        //         if (( ch=='K') || (ch=='0') )
        //         {
        //             // debugprintf("[OK]\r\n");
        //             idx = 30;
        //         }
        //     }
        //     else if (tick_iri0==0)
        //     {
        //         debugprintf("[NG]\r\n");
        //         //debugstring("ATE0 NG\r\n");
        //         idx = 999;
        //     }
        //     break;

            //AT&K0 ------------------------------------------------AT&K0
        case 30:
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
    static int idx = 0;
    static int resp_idx = 0;
    static int at_cnt = 0;;

    char ch;
    int i;
    int ret_val = 999;

    switch (a_option)
    {
        case 1:
            // 새로 시작
            idx = 0;
            break;
        case 0:
            // idx는 현재값을 유지하고...---> 상태체크용.
        default:
            break;
    }


    switch (idx)
    {
        case 0:
            at_cnt = 0;
            debugprintf("\r\nIridium-[%d] is active.\r\n", ((iri_port==C_IRIDIUM_1) ? 1:2) );
            idx = 450;
            break;

        case 450:
            resp_idx = 0;
            //debugprintf("AT ---> ");
            iridium_printf("AT\r");
            tick_iri0 = 1000/10;    //1000/10;   //1sec
            idx = 500;
            break;
        case 500:
            //wait 'OK'
            if (UartGetCh(iri_port,&ch))
            {
                iri_response[resp_idx++] = ch;
                //debugprintf("%c",ch);
                if ( (ch=='K') || (ch=='0') )
                {
                    //debugprintf("[OK]\r\n");
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

            // AT+SBDREG  ------------------------------------------------AT+SBDREG
        case 80:
            for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            debugstring("AT+SBDREG? ---> ");

            resp_idx = 0;
            iridium_printf("AT+SBDREG?\r\n");
            tick_iri0 = 60000/10;  //60sec
            idx = 85;
            break;
        case 85:
            //wait 'OK'
            if (UartGetCh(iri_port,&ch))
            {
                if (ch=='+')
                {
                    resp_idx=0;
                }

                // debugprintf("%c",ch);
                iri_response[resp_idx++] = ch;
                //if (ch=='K')
                if ( (ch== 0x0D) || (ch== 0x0A))
                {
                    iri_response[resp_idx++] = '\0';
                    if (iri_response[0]=='+')
                    {
                        debugstring("\r\n");
                        debugstring(iri_response);
                        {
                            int i=0;
                            int j=0;
                            int k=0;
                            char ch1;
                            char parsed_block[5][10];
                            while (1)
                            {
                                ch1 = iri_response[k++];
                                if ((ch1==0x0D) || (ch1==0x0A) || ch1=='\0')
                                {
                                    parsed_block[i][j] = '\0';
                                    break;
                                }
                                else if ( (ch1==',') || (ch1==':') )
                                {
                                    parsed_block[i][j] = '\0';
                                    i++;
                                    j=0;
                                }
                                else
                                {
                                    parsed_block[i][j++] = ch1;
                                }
                            }
                            // debugprintf("%s\r\n",parsed_block[0]);//sbdi
                            // debugprintf("%s\r\n",parsed_block[1]);//mo-status
                            // debugprintf("%s\r\n",parsed_block[2]);//mo-msn
                            // debugprintf("%s\r\n",parsed_block[3]);//mt-status
                            // debugprintf("%s\r\n",parsed_block[4]);//mt-msn
                            // debugprintf("%s\r\n",parsed_block[5]);//mt-len
                            // debugprintf("%s\r\n",parsed_block[6]);//mt-queue

                            if (atoi(parsed_block[1]) != 2)
                            {
                                resp_idx = 0;
                                iridium_printf("AT+SBDREG\r\n");
                                tick_iri0 = 120000/10;  //60sec
                                idx = 90;
                            }
                            else
                            {
                                idx = 999;
                            }
                        }
                    }
                    else
                    {
                        resp_idx=0;
                    }
                }
            }
            else if (tick_iri0==0)
            {
                debugstring("\r\n-----AT+SBDREG timeout!!\r\n");
                idx = 999;
            }
            break;

        case 90:
            if (UartGetCh(iri_port,&ch))
            {
                debugprintf("%c", ch);
            }
            if (tick_iri0==0)
            {
                idx = 999;
            }
            break;

        case 999:
        default:
            ret_val = 1;
            break;
    }
    return(ret_val);
}



int sbdixa_check(int a_option)
{
    static int idx = 0;
    static int resp_idx = 0;
    //static int retry_cnt = 0;
    static int at_cnt = 0;;

    char ch;
    int i;
    int ret_val = 999;

    switch (a_option)
    {
        case 1:
            // 새로 시작
            idx = 0;
            break;
        case 0:
            // idx는 현재값을 유지하고...---> 상태체크용.
        default:
            break;
    }


    switch (idx)
    {
        case 0:
            at_cnt = 0;
            debugprintf("\r\nIridium-[%d] is active.\r\n", ((iri_port==C_IRIDIUM_1) ? 1:2) );
            idx = 450;
            break;

        case 450:
            resp_idx = 0;
            //debugprintf("AT ---> ");
            iridium_printf("AT\r");
            tick_iri0 = 1000/10;    //1000/10;   //1sec
            idx = 500;
            break;
        case 500:
            //wait 'OK'
            if (UartGetCh(iri_port,&ch))
            {
                iri_response[resp_idx++] = ch;
                //debugprintf("%c",ch);
                if ( (ch=='K') || (ch=='0') )
                {
                    //debugprintf("[OK]\r\n");
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

            // AT+SBDI ------------------------------------------------AT+SBDI
        case 112:
            for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            debugprintf("AT+SBDIXA ---> ");

            resp_idx=0;
            iridium_printf("AT+SBDIXA\r\n");
            // iridium_printf("AT+SBDIXA\r\n");
            tick_iri0 = 60000/10;  //120sec
            idx = 115;
            break;
        case 115:
            //wait '+SBDI:'
            if (UartGetCh(iri_port,&ch))
            {
                debugprintf("%c",ch);

                if (ch=='+')
                {
                    resp_idx = 0;
                }

                iri_response[resp_idx++] = ch;

                //if ((ch==':'))
                if ((ch==0x0D) || (ch==0x0A))
                {
                    iri_response[resp_idx++] = '\0';
                    if (iri_response[0]=='+')
                    {
                        debugstring("\r\n");
                        debugstring(iri_response);
                        {
                            int i=0;
                            int j=0;
                            int k=0;
                            char ch1;
                            char parsed_block[8][10];
                            while (1)
                            {
                                ch1 = iri_response[k++];
                                if ((ch1==0x0D) || (ch1==0x0A) || ch1=='\0')
                                {
                                    parsed_block[i][j] = '\0';
                                    break;
                                }
                                else if ( (ch1==',') || (ch1==':') )
                                {
                                    parsed_block[i][j] = '\0';

                                    //debugstring("\r\n");
                                    //debugstring(parsed_block[i]);

                                    i++;
                                    j=0;
                                }
                                else
                                {
                                    parsed_block[i][j++] = ch1;
                                }
                            }
                            // debugprintf("%s\r\n",parsed_block[0]);//sbdi
                            // debugprintf("%s\r\n",parsed_block[1]);//mo-status
                            // debugprintf("%s\r\n",parsed_block[2]);//mo-msn
                            // debugprintf("%s\r\n",parsed_block[3]);//mt-status
                            // debugprintf("%s\r\n",parsed_block[4]);//mt-msn
                            // debugprintf("%s\r\n",parsed_block[5]);//mt-len
                            // debugprintf("%s\r\n",parsed_block[6]);//mt-queue

                            mo_status = atoi(parsed_block[1]);
                            // mt_status = atoi(parsed_block[3]);
                            set_mt_status(atoi(parsed_block[3]));

                            debugprintf("mo_status= %d\r\n",mo_status);
                            debugprintf("mt_status= %d\r\n",get_mt_status());

                            if (iri_port == C_IRIDIUM_1)    set_sbdring1(0);
                            if (iri_port == C_IRIDIUM_2)    set_sbdring2(0);
                        }

                        idx = 120;
                        break;
                    }
                    else
                    {
                        resp_idx=0;
                    }
                }
            }
            else if (tick_iri0==0)
            {
                debugstring("\r\nAT+SBDI timeout!!\r\n");
                idx = 999;
            }
            break;
        case 120:
        case 125:
        case 90:
        case 999:
        default:
            ret_val = 1;
            break;
    }
    return(ret_val);
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

}



int sbdrb_check(int a_option)
{
    static int idx = 0;
    static int resp_idx = 0;
    //static int retry_cnt = 0;
    static int at_cnt = 0;;

    char ch;
    int i;
    int ret_val = 999;

    switch (a_option)
    {
        case 1:
            // 새로 시작
            idx = 0;
            break;
        case 0:
            // idx는 현재값을 유지하고...---> 상태체크용.
        default:
            break;
    }


    switch (idx)
    {
        case 0:
            at_cnt = 0;
            debugprintf("\r\nIridium-[%d] is active.\r\n", ((iri_port==C_IRIDIUM_1) ? 1:2) );
            idx = 450;
            break;

        case 450:
            resp_idx = 0;
            //debugprintf("AT ---> ");
            iridium_printf("AT\r");
            tick_iri0 = 1000/10;    //1000/10;   //1sec
            idx = 500;
            break;
        case 500:
            //wait 'OK'
            if (UartGetCh(iri_port,&ch))
            {
                iri_response[resp_idx++] = ch;
                //debugprintf("%c",ch);
                if ( (ch=='K') || (ch=='0') )
                {
                    //debugprintf("[OK]\r\n");
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

            // AT+SBDRB ------------------------------------------------AT+SBDRB
        case 112:

            set_mt_status(0);       // flag clear   //(+)130904


            for (i=0;i<10;i++) UartGetCh(iri_port,&ch);
            debugprintf("AT+SBDRB ---> ");

            resp_idx=0;
            iridium_printf("AT+SBDRB\r\n");
            tick_iri0 = 60000/10;  //120sec
            idx = 115;
            break;
        case 115:
            //wait '+SBDI:'
            if (UartGetCh(iri_port,&ch))
            {
                // debugprintf("%c",ch);
                debugprintf("%02X ",ch);

                iri_response[resp_idx++] = ch;

                if ((ch==0x0D) || (ch==0x0A))
                {
                    iri_response[resp_idx++] = '\0';
                    debugstring("\r\n");
                    debugstring(iri_response);

                    // parsing
                    {
                        int xxx = 0;
                        while(1)
                        {
                            if (iri_response[xxx]=='\0')
                            {
                                debugprintf("\r\n");
                               break;
                            }
                            debugprintf("%x ", iri_response[xxx++]);

                        }

                        switch (iri_response[0+2])
                        {
                            //case 'K':
                            //case 'k':
                            case BUOY_SPEC:
                                // set_mt_status(0);       // flag clear   //(-)130904


                                // 유효시간 체크
                                {
//                                    int yr,mon,day,hr,min;
//                                    u32 cur_t, nsg_t;


                                }

                                // 부이ID 확인
                                {
                                    char term[4];
                                    term[0] = iri_response[4+2];
                                    term[1] = iri_response[5+2];
                                    term[2] = iri_response[6+2];
                                    term[3] = '\0';

                                    if ( strncmp( term, BUOY_ID, 3))
                                    {
                                        // 다르면...
                                        break;
                                    }

                                }


                                switch (iri_response[1+2])
                                {
                                    case 'A':   // command
                                    case 'a':   // command
                                        debugprintf("message type A\r\n");

                                        // 0         1         2
                                        // 0123456789012345678901
                                        // KAA0S9112062621460000
                                        {
                                            char s_id, c_code;
                                            s_id=   iri_response[19+2];
                                            c_code= iri_response[20+2];

                                            switch(c_code)
                                            {
                                                case '0':
                                                case '1':
                                                case '2':
                                                    ctrl_sensor(s_id, c_code);
                                                    ctrl_sensor_set_env(s_id, c_code);
                                                    make_msg_ky('A',1);
                                                    break;
                                                default:
                                                    make_msg_ky('A',0);
                                                    break;
                                            }
                                        }
                                        break;

                                    case 'E':   // 전송주기
                                    case 'e':   // 전송주기
                                        debugprintf("message type E\r\n");
                                        // 0         1         2
                                        // 0123456789012345678901
                                        // KEE0S91120607075000060
                                        {
                                            char term[4];
                                            int i_term =30;
                                            term[0] = iri_response[19+2];
                                            term[1] = iri_response[20+2];
                                            term[2] = iri_response[21+2];
                                            term[3] = '\0';
                                            i_term = atoi(term);

                                            switch(i_term)
                                            {
                                                case 10:
                                                case 20:
                                                case 30:
                                                case 60:
                                                case 120:
                                                    env_set_interval(i_term);
                                                    make_msg_ky('E',1);
                                                    break;
                                                default:
                                                    make_msg_ky('E',0);
                                                    break;
                                            }
                                        }
                                        break;

                                    case 'F':   // 기준위치변경
                                    case 'f':   // 기준위치변경
                                        debugprintf("message type F\r\n");
                                        // 0         1         2         3
                                        // 01234567890123456789012345678901234
                                        // KFF0S91120607075000370000012600000
                                        {
                                            char c_lat[8];
                                            char c_lon[9];
                                            double f_lat,f_lon;
                                            c_lat[0]= iri_response[19+2];
                                            c_lat[1]= iri_response[20+2];
                                            c_lat[2]= iri_response[21+2];
                                            c_lat[3]= iri_response[22+2];
                                            c_lat[4]= iri_response[23+2];
                                            c_lat[5]= iri_response[24+2];
                                            c_lat[6]= iri_response[25+2];
                                            c_lat[7]= '\0';

                                            c_lon[0]= iri_response[26+2];
                                            c_lon[1]= iri_response[27+2];
                                            c_lon[2]= iri_response[28+2];
                                            c_lon[3]= iri_response[29+2];
                                            c_lon[4]= iri_response[30+2];
                                            c_lon[5]= iri_response[31+2];
                                            c_lon[6]= iri_response[32+2];
                                            c_lon[7]= iri_response[33+2];
                                            c_lon[8]= '\0';
                                            f_lat= atof(c_lat)/100000.0f;
                                            f_lon= atof(c_lon)/100000.0f;

                                            {
                                                int valid = 0;
                                                if ( (f_lat >= 0.0f)&&(f_lat <= 89.99999f) )
                                                {
                                                    if ( (f_lon >= 0.0f)&&(f_lon <= 179.00000f) )
                                                    {
                                                        valid = 1;
                                                    }
                                                }
                                                if (valid == 1)
                                                {
                                                    env_set_pos(f_lat, f_lon);
                                                    make_msg_ky('F',1);
                                                }
                                                else
                                                {
                                                    make_msg_ky('F',0);
                                                }
                                            }
                                        }
                                        break;

                                    case 'G':   // 위험반경변경
                                    case 'g':   // 위험반경변경
                                        debugprintf("message type G\r\n");
                                        // 0         1         2
                                        // 01234567890123456789012
                                        // KGG0S91120607075000001
                                        {
                                            char term[4];
                                            int i_term =30;
                                            term[0] = iri_response[19+2];
                                            term[1] = iri_response[20+2];
                                            term[2] = iri_response[21+2];
                                            term[3] = '\0';
                                            i_term = atoi(term);

                                            if ( (i_term >=1) && (i_term <=100))
                                            {
                                                env_set_distance(i_term);
                                                make_msg_ky('G',1);
                                            }
                                            else
                                            {
                                                make_msg_ky('G',0);
                                            }
                                        }
                                        break;

                                    case 'I':   // 충격랑변경
                                    case 'i':   // 충격랑변경
                                        debugprintf("message type I\r\n");
                                        // 0         1         2
                                        // 01234567890123456789012
                                        // KGG0S91120607075000001
                                        {
                                            char term[4];
                                            int i_term =30;
                                            term[0] = iri_response[19+2];
                                            term[1] = iri_response[20+2];
                                            term[2] = iri_response[21+2];
                                            term[3] = '\0';
                                            i_term = atoi(term);

                                            if ( (i_term >=0) && (i_term <=999))
                                            {
                                                env_set_shock(i_term);
                                                make_msg_ky('I', 1);
                                            }
                                            else
                                            {
                                                make_msg_ky('I', 0);
                                            }
                                        }
                                        break;

                                    case 'J':   // TRBM RESET
                                    case 'j':   // TRBM RESET
                                        debugprintf("message type J\r\n");
                                        //DEBUGPRINTF("message type J\r\n");
                                        // 0         1         2
                                        // 01234567890123456789012
                                        // KGG0S91120607075000001
                                        {
                                            char term[4];
                                            int i_term =30;
                                            term[0] = iri_response[19+2];
                                            term[1] = iri_response[20+2];
                                            term[2] = iri_response[21+2];
                                            term[3] = '\0';
                                            i_term = atoi(term);

                                            switch (i_term)
                                            {
                                                case 1:
                                                    // sb_printstring(SB_S_ATM1, "%+RTRBM\r");
                                                    // sb_printstring(SB_S_ATM2, "%+RTRBM\r");
                                                    make_msg_ky('J',1);
                                                    break;
#if (BUOY_SPEC == 'D')
                                                case 10:
                                                    // 지진모드해제
                                                    env_set_mode(0);
                                                    make_msg_ky('J',1);
                                                    break;
                                                case 11:
                                                    // 지진모드설정
                                                    env_set_mode(1);
                                                    make_msg_ky('J',1);
                                                    break;
#endif
                                                default:
                                                    make_msg_ky('J',0);
                                                    break;
                                            }
                                            // if (i_term == 1)
                                            // {
                                            //     sb_printstring(SB_S_ATM2, "%+RTRBM\r");
                                            //     make_msg_ky('J',1);
                                            // }
                                            // else
                                            // {
                                            //     make_msg_ky('J',0);
                                            // }
                                        }
                                        break;
                                    default:
                                        break;

                                }   //switch (iri_response[1+2])
                                break;
                            default:
                                break;

                        }   //switch (iri_response[0+2])

                    }
                    idx = 120;
                }   //if ((ch==0x0D) || (ch==0x0A))
            }   //if (UartGetCh(iri_port,&ch))

            else if (tick_iri0==0)
            {
                debugstring("\r\nAT+SBDI timeout!!\r\n");
                idx = 999;
            }
            break;
        case 120:
        case 125:
        case 90:
        case 999:
        default:
            ret_val = 1;
            break;
    }   //switch (idx)
    return(ret_val);
}



