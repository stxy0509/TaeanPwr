#define __K_MSG_C__

#include "k_includes.h"

#define DFLT_SENSOR     0x7D757775


int fg_sec_data_display = 0;
int fg_min_data_display = 0;
void set_secData_display(void)
{
    if (fg_sec_data_display==0)
    {
        debugprintf("ON\r\n");
        fg_sec_data_display = 1;
    }
    else
    {
        debugprintf("OFF\r\n");
        fg_sec_data_display = 0;
    }
}
void set_minData_display(void)
{
    if (fg_min_data_display==0)
    {
        debugprintf("ON\r\n");
        fg_min_data_display = 1;
    }
    else
    {
        debugprintf("OFF\r\n");
        fg_min_data_display = 0;
    }
}



void strxch(char *dst, int st, int len, char *val)
{
	int i;

	dst += st;
	for ( i=0; i<len; i++ )
	{
		*dst++ = *val++;
	}

}


void xstrcpy(char *dst, const char *val, int st, int len )
{
    int i;

    dst += st;
    for ( i=0; i<len; i++ )
    {
        *dst++ = *val++;
    }
}






u32 update_sensor_status(u32 a_status)
{
    u32 status = a_status;

    // debugprintf("status=%08X ", status);

    //'0'=Reset, '1'=On, '2'=Off
    // if (env.s0 == '2')  {   status &= ~(u32)(3);   }   //aio
    // if (env.s1 == '2')  {   status &= ~(u32)(3<<(1*2));   }   //mose
    // // if (env.s2 == '2')  {   status &= ~(u32)(3);   }   //--
    // if (env.s3 == '2')  {   status &= ~(u32)(3<<(3*2));   }   //ct
    // //if (env.s4 == '2')  {   status &= ~(u32)(3);   }   //--
    // if (env.s5 == '2')  {   status &= ~(u32)(3<<(5*2));   }   //dcs
    // //if (env.s6 == '2')  {   status &= ~(u32)(3);   }   //--
    // if (env.s7 == '2')  {   status &= ~(u32)(3<<(7*2));   }   //gps
    // if (env.s8 == '2')  {   status &= ~(u32)(3<<(8*2));   }   //atm1
    // if (env.s9 == '2')  {   status &= ~(u32)(3<<(9*2));   }   //atm2
    // //if (env.sa == '2')  {   status &= ~(u32)(3);   }   //--
    // if (env.sb == '2')  {   status &= ~(u32)(3<<(11*2));   }   //hmp
    // if (env.sc == '2')  {   status &= ~(u32)(3<<(12*2));   }   //ptb
    // //if (env.sd == '2')  {   status &= ~(u32)(3);   }   //--
    // //if (env.se == '2')  {   status &= ~(u32)(3);   }   //--
    // if (env.sf == '2')  {   status &= ~(u32)(3<<(15*2));   }   //shock

    // debugprintf("%08X\r\n", status);
    return (status);
}


//------------------------------------------------------------------------
//      char * make_msg_kx(void)
//              event 알림 msg
//------------------------------------------------------------------------
char * make_msg_kx(char a_code)
{
    return (s_msg);
}



//------------------------------------------------------------------------
//      char * make_msg_ky(void)
//------------------------------------------------------------------------
char * make_msg_ky(char t_id, int a_result)
{

    return (s_msg);
}


//==========================================================================================================================
#if 1   //BUOY_ECHO



// char bin_header[7];

//------------------------------------------------------------------------
//      char * make_msg_k1(void)
//------------------------------------------------------------------------
char * make_msg_k1(void)
{

    char tmp_buf[50];
    //char t_buf[15];
    // 38 byte + '\0'

    // 0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........
    // 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // K0S010yymmddhhmmLat---Longi----1234123Sensor--ws-wd-at--ap---hu-2at-2ap--sc--st--dcs-dc-
    // #H11,150310160005,-84,11.9,0000.0000,00000.0000,,,1.348E+01


    debugstring(    "-------------------------------\r\n");
    // debugstring(    "     make msg K1\r\n");
    // debugstring(    "-------------------------------\r\n");


    memset(s_msg,0,MSG_LENGTH);

    // Header
    strcpy(s_msg, "#");
    strcat(s_msg, env.id);

    // date time
    rtc_isSecUpdate();
    sprintf(tmp_buf,"11,%02d%02d%02d%02d%02d00,",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min);
    strcat(s_msg, tmp_buf);

    // debugprintf("%s\r\n", s_msg);

    // CDMA RSSI
    sprintf(tmp_buf,"%d,",lblRSSI);
    strcat(s_msg, tmp_buf);
    
    // BAT
    sprintf(tmp_buf,"%.1f,", (float)(get_battery_level()/10.0f));
    strcat(s_msg, tmp_buf);
    // debugprintf("%s \r\n", s_msg);

    // Position
    strcat(s_msg, "0000.0000,00000.0000,,,");
    // debugprintf("%s \r\n", s_msg);


    if (is_dcs_valid()==1)
    {
        sprintf(tmp_buf,"%.3f",get_dcs_speed());    // dcs.pitch == temperature
        // strcat(s_msg, tmp_buf);                  // (-)150518
        strcat(s_msg, get_4050_temp());             // (+)150518
        set_dcs_valid(0);
        init_dcs_data();
    }
    // debugprintf("%s \r\n", s_msg);





// PRINTVAR(strlen(s_msg));


    sensor_status.w = DFLT_SENSOR;
    // sensor_status.w = 0xFFFFFFFF;
    // 5    5    5    5    5    5    5    5
    // 0101 0101 0101 0101 0101 0101 0101 0101
    // 0111 1101 0101 0101 0111 0111 0111 0101
    // 7    D    5    5    7    7    7    5
    // 7D557775 : DEFAULT VALUE


// F7 77 55 D5

    // (-)131017   --->>>
    if (is_q_full())
    {
        q_pop();
    }
    q_putData();
    // <<<------

    // (+)131017 ---->>>
    // fgMsgC_ready = 1;
    // {
    //     int i;
    //     for (i=0;i<MSG_LENGTH; i++)
    //     {
    //         s_msg_c[i] = s_msg[i];
    //     }
    // }
    // <<<------


    //saveDeferredMsgFile();

    debugstring(s_msg);
    // debugstring(s_msg_c);
    debugstring("\r\n");
    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());


    strcat(s_msg, "\r\n");

    if (sdc_read_detectPin()==SDC_INSERTED)
    {
        u32 fsz;
        //PRINTLINE;
        sdc_saveDataToFile(FN_SEND, s_msg, &fsz);
        //PRINTVAR(fsz);
        if (fsz > FSZ_MAX)
        {
            SensorBakSize.b.send = 1;
        }
    }


    return (s_msg);
}
#endif


#if (BUOY_SPEC == 'D')

//------------------------------------------------------------
void make_msg_press_5(void)
{
    // 지진 데이타 (지진모드 : 5분 전송==수신즉시 전송한다)
}



//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
void make_msg_press_30(void)
{

}
#endif



//------------------------------------------------------------
// Queue for defered Data
//
//
void init_q(void)
{
    q_front = 0;
    q_rear = 0;
    debugstring("Q: init\r\n");
}

char q_putData(void)
{
    int i;
    for (i=0;i<MSG_LENGTH; i++)
    {
        msg_queue[q_rear][i] = s_msg[i];
    }
    q_rear++;
    q_rear %= DEFERD_Q_MAX;
    // debugprintf("Q-put: f(%d) r(%d)",q_front,q_rear);
    return 1;
}

char q_getData_only(void)
{
    int i;
    for (i=0;i<MSG_LENGTH; i++)
    {
        s_msg[i] = msg_queue[q_front][i];
    }
    //debugprintf("Q-getData: f(%d) r(%d)\r\n",q_front,q_rear);
    return 1;
}

// char q_getData_only_R(void)
// {
//     int i;
//     int tmp_rear;

//     if (rear==0)
//     {
//         tmp_rear = DEFERD_Q_MAX;
//     }
//     else
//     {
//         tmp_rear = rear-1;
//     }

//     for (i=0;i<MSG_LENGTH; i++)
//     {
//         s_msg[i] = msg_queue[tmp_rear][i];
//     }
//     //debugprintf("Q-getData: f(%d) r(%d)\r\n",q_front,q_rear);
//     return 1;
// }

char q_getData(int num)
{
    int i;

    num += q_front;
    num %= DEFERD_Q_MAX;

    for (i=0;i<MSG_LENGTH; i++)
    {
        s_msg[i] = msg_queue[num][i];
    }
    //debugprintf("Q-getData: f(%d) r(%d)\r\n",q_front,q_rear);
    return 1;
}

// char q_getData_R(int num)
// {
//     // int i;

//     // num += q_front;
//     // num %= DEFERD_Q_MAX;

//     // for (i=0;i<MSG_LENGTH; i++)
//     // {
//     //     s_msg[i] = msg_queue[num][i];
//     // }
//     // //debugprintf("Q-getData: f(%d) r(%d)\r\n",q_front,q_rear);
//     return 1;
// }

char q_pop(void)
{
    q_front++;
    q_front %= DEFERD_Q_MAX;

    debugprintf("Q-pop: f(%d) r(%d)\r\n",q_front,q_rear);
    return 1;
}

// char q_pop_R(void)
// {
//     if (rear==0)
//     {
//         rear = DEFERD_Q_MAX;
//     }
//     else
//     {
//         rear--;
//     }
//     // q_front++;
//     // q_front %= DEFERD_Q_MAX;

//     debugprintf("Q-pop: f(%d) r(%d)\r\n",q_front,q_rear);
//     return 1;
// }

char is_q_empty(void)
{
    if (q_front == q_rear)    //empty???
        return 1;
    else
        return 0;
}
char is_q_full(void)
{
    if (( (q_rear + 1) % DEFERD_Q_MAX ) == q_front )        // full ??
        return 1;
    else
        return 0;
}
int is_q_dataNum(void)
{
    int ret_val=0;
    if (q_front == q_rear)    //empty???
        ret_val= 0;
    else if (( (q_rear + 1) % DEFERD_Q_MAX ) == q_front )       // full ??
        ret_val= (DEFERD_Q_MAX-1);
    else if (q_front < q_rear)
    {
        ret_val= (q_rear - q_front);
    }
    else if (q_front > q_rear)
    {
        ret_val= ((DEFERD_Q_MAX + q_rear) - q_front);
    }
    return(ret_val);
}




