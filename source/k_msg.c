#define __K_MSG_C__

#include "k_includes.h"

#define DFLT_SENSOR     0x7D757775



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
    if (env.s0 == '2')  {   status &= ~(u32)(3);   }   //aio
    if (env.s1 == '2')  {   status &= ~(u32)(3<<(1*2));   }   //mose
    // if (env.s2 == '2')  {   status &= ~(u32)(3);   }   //--
    if (env.s3 == '2')  {   status &= ~(u32)(3<<(3*2));   }   //ct
    //if (env.s4 == '2')  {   status &= ~(u32)(3);   }   //--
    if (env.s5 == '2')  {   status &= ~(u32)(3<<(5*2));   }   //dcs
    //if (env.s6 == '2')  {   status &= ~(u32)(3);   }   //--
    if (env.s7 == '2')  {   status &= ~(u32)(3<<(7*2));   }   //gps
    if (env.s8 == '2')  {   status &= ~(u32)(3<<(8*2));   }   //atm1
    if (env.s9 == '2')  {   status &= ~(u32)(3<<(9*2));   }   //atm2
    //if (env.sa == '2')  {   status &= ~(u32)(3);   }   //--
    if (env.sb == '2')  {   status &= ~(u32)(3<<(11*2));   }   //hmp
    if (env.sc == '2')  {   status &= ~(u32)(3<<(12*2));   }   //ptb
    //if (env.sd == '2')  {   status &= ~(u32)(3);   }   //--
    //if (env.se == '2')  {   status &= ~(u32)(3);   }   //--
    if (env.sf == '2')  {   status &= ~(u32)(3<<(15*2));   }   //shock

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

int dp[][2]= {
    {   6,  3}, // 0
    {  15,  1}, // 1
    {  25,  7}, // 2
    {  34,  5}, // 3
    {  43,  3}, // 4
    {  52,  1}, // 5
    {  62,  7}, // 6
    {  71,  5}, // 7
    {  80,  3}, // 8
    {  89,  1}, // 9

    {  99,  7}, // 10
    { 108,  5}, // 11
    { 117,  3}, // 12
    { 126,  1}, // 13
    { 136,  7}, // 14
    { 145,  5}, // 15
    { 154,  3}, // 16
    { 163,  1}, // 17
    { 173,  7}, // 18
    { 182,  5}, // 19

    { 191,  3}, // 20
    { 200,  1}, // 1
    { 210,  7}, // 2
    { 219,  5}, // 3
    { 228,  3}, // 4
    { 237,  1}, // 5
    { 247,  7}, // 6
    { 256,  5}, // 7
    { 265,  3}, // 8
    { 274,  1}, // 9

    { 284,  7}, // 30
    { 293,  5}, // 1
    { 302,  3}, // 2
    { 311,  1}, // 3
    { 321,  7}, // 4
    { 330,  5}, // 5
    { 339,  3}, // 6
    { 348,  1}, // 7
    { 358,  7}, // 8
    { 367,  5}, // 9

    { 376,  3}, // 40
    { 385,  1}, // 1
    { 395,  7}, // 2
    { 404,  5}, // 3
    { 413,  3}, // 4
    { 422,  1}, // 5
    { 432,  7}, // 6
    { 441,  5}, // 7
    { 450,  3}, // 8
    { 459,  1}, // 9

    { 469,  7}, // 50
    { 478,  5}, // 1
    { 487,  3}, // 2
    { 496,  1}, // 3
    { 506,  7}, // 4
    { 515,  5}, // 5
    { 524,  3}, // 6
    { 533,  1}, // 7
    { 543,  7}, // 8
    { 552,  5} // 9
};





// struct _echoData echoData[60];
// echoData_bin_T echoData[60];
echoData_T echoData[60];

// echo1_T data1;
// echoData_bin_T bb;

void init_echoData(void)
{
    int i;

    debugstring("init echoData buffer...\r\n");

    for (i=0; i<60; i++)
    {
        echoData[i].gps_valid = 0;
        echoData[i].depth_valid = 0;
        echoData[i].temp_valid = 0;
    }
}


void make_msg_second(void)
{
    int i;

    // static int fg_valid_before = 1;
    // static int sec_before = 99;

    i = rtc_time.sec;

    echoData[i].lat_d  = get_gps_lat_d();
    echoData[i].lat_md = get_gps_lat_m();
    echoData[i].lat_mf = get_gps_lat_mf();

    echoData[i].lon_d  = get_gps_lon_d();
    echoData[i].lon_md = get_gps_lon_m();
    echoData[i].lon_mf = get_gps_lon_mf();

    echoData[i].depth = get_ct_cond();
    echoData[i].temp  = get_ct_temp();

#if 1
    echoData[i].gps_valid     = is_gps_valid();
    echoData[i].depth_valid   = is_ct_valid();
    echoData[i].temp_valid    = is_ct_valid();
#else
    // test only
    echoData[i].gps_valid     = 0;//is_gps_valid();
    echoData[i].depth_valid   = 0;//is_ct_valid();
    echoData[i].temp_valid    = 0;//is_ct_valid();
#endif

    // debugprintf("echoData[%d] saved...\r\n", i);
}


// char bin_header[7];

//------------------------------------------------------------------------
//      char * make_msg_k1(void)
//------------------------------------------------------------------------
char * make_msg_k1(void)
{
    // unsigned char tmp_buf[50];
    unsigned char t_buf[15];

    unsigned char s_msg2[575];

    // 38 byte + '\0'

    // 0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........
    // 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // K0S010yymmddhhmmLat---Longi----1234123Sensor--ws-wd-at--ap---hu-2at-2ap--sc--st--dcs-dc-

    debugstring(    "-------------------------------\r\n");
    debugstring(    "     make msg K1\r\n");
    debugstring(    "-------------------------------\r\n");

    memset(s_msg,0,MSG_LENGTH);
    memset(s_msg2,0,575);
    memset(t_buf,0,15);

    rtc_isSecUpdate();

    // Header
    // memset(s_msg,0,7);


    s_msg2[0] = 'E';
    s_msg2[1] = 'B';

#if 0
    {
        int xx;
        for (xx=0; xx<=1; xx++)
        {
            debugprintf("%02X ", s_msg2[xx]);
        }
        debugprintf("\r\n");
    }
#endif

    // Buoy ID
    {
        U16 id = 001 << 6;

        s_msg2[2] = (char)( id>>8 );
        s_msg2[3] = (char)( id & 0x00FF );
    }

#if 0
    {
        int xx;
        for (xx=0; xx<=3; xx++)
        {
            debugprintf("%02X ", s_msg2[xx]);
        }
        debugprintf("\r\n");
    }
#endif
    // Date & Time
    {
        u8 i;
        debugprintf("%d.%d.%d %d:%d\r\n", rtc_time.year, rtc_time.mon, rtc_time.day, rtc_time.hour, rtc_time.min);
        i= rtc_time.year%100;
        // debugprintf("%02X ", i);
        s_msg2[3] |= ( i >> 1 );
        s_msg2[4] = ( i << 7 );

        i= rtc_time.mon;
        // debugprintf("%02X ", i);
        s_msg2[4] |= ( i << 3 );

        i= rtc_time.day;
        // debugprintf("%02X ", i);
        s_msg2[4] |= ( i >> 2 );
        s_msg2[5] = ( i << 6 );

        i= rtc_time.hour;
        // debugprintf("%02X ", i);
        s_msg2[5] |= ( i << 1 );

        i= rtc_time.min;
        // debugprintf("%02X\r\n", i);
        s_msg2[5] |= ( i >> 5 );
        s_msg2[6] = ( i << 3 );
    }
#if 0
    {
        int xx;
        for (xx=0; xx<=6; xx++)
        {
            debugprintf("%02X ", s_msg2[xx]);
        }
        debugprintf("\r\n");
    }
#endif
    // 1초 데이타 연결

    {
        int i,j,k, i1;
        u16 d16bit;
        // unsigned char tbuf[5];

        for (i=0; i<60; i++)
        {
            if (echoData[i].gps_valid ==0)
            {
                if ( (i==0) & (echoData[i+1].gps_valid ==1) )
                {
                    echoData[i].lat_d  = echoData[i+1].lat_d;
                    echoData[i].lat_md = echoData[i+1].lat_md;
                    echoData[i].lat_mf = echoData[i+1].lat_mf;

                    echoData[i].lon_d  = echoData[i+1].lon_d;
                    echoData[i].lon_md = echoData[i+1].lon_md;
                    echoData[i].lon_mf = echoData[i+1].lon_mf;

                    debugprintf("adjust-insert....\r\n");
                }
                else if (echoData[i-1].gps_valid ==1)
                {
                    echoData[i].lat_d  = echoData[i-1].lat_d;
                    echoData[i].lat_md = echoData[i-1].lat_md;
                    echoData[i].lat_mf = echoData[i-1].lat_mf;

                    echoData[i].lon_d  = echoData[i-1].lon_d;
                    echoData[i].lon_md = echoData[i-1].lon_md;
                    echoData[i].lon_mf = echoData[i-1].lon_mf;

                    debugprintf("adjust-insert....\r\n");
                }
                else
                {
                    echoData[i].lat_d  = 90;//32;
                    echoData[i].lat_md = 59;//12;
                    echoData[i].lat_mf = 9999;//3456;

                    echoData[i].lon_d  = 180;//126;
                    echoData[i].lon_md = 59;//23;
                    echoData[i].lon_mf = 9999;//4567;

                }
            }

            if (echoData[i].depth_valid ==0)
            {
                if ( (i==0) &  (echoData[i+1].depth_valid ==1)  )
                {
                    echoData[i].depth  = echoData[i+1].depth;
                    echoData[i].temp = echoData[i+1].temp;
                }
                else if (echoData[i-1].depth_valid ==1)
                {
                    echoData[i].depth  = echoData[i-1].depth;
                    echoData[i].temp = echoData[i-1].temp;
                }
                else
                {
                    echoData[i].depth = 999;//200+i;  //get_ct_cond();
                    echoData[i].temp  = 511;//511;//get_ct_temp();
                }
            }


            {
                debugprintf("%02d: %d%02d.%04d - %d%02d.%04d - %03d %03d\r\n", i, echoData[i].lat_d, echoData[i].lat_md, echoData[i].lat_mf, echoData[i].lon_d, echoData[i].lon_md, echoData[i].lon_mf, echoData[i].depth, echoData[i].temp);
                {
                    t_buf[0]  = echoData[i].lat_d  << 1;
                    t_buf[0] |= echoData[i].lat_md >> 5;

                    t_buf[1] |= echoData[i].lat_md << 3;

                    d16bit = echoData[i].lat_mf;
                    t_buf[1] |= (char)(d16bit >> 11);
                    t_buf[2]  = (char)((d16bit >> 3) & 0x00FF);
                    t_buf[3]  = (char)((d16bit << 5) & 0x00FF);

                    t_buf[3] |= (char)(echoData[i].lon_d >> 3);
                    t_buf[4]  = (char)(echoData[i].lon_d << 5);
                    t_buf[4] |= (char)(echoData[i].lon_md >> 1);

                    t_buf[5]  = (char)(echoData[i].lon_md << 7);
                    t_buf[5] |= (char)(echoData[i].lon_mf >> 7);
                    t_buf[6]  = (char)( (echoData[i].lon_mf << 1) & 0x00FF);
                    t_buf[6] |= (char)( (echoData[i].depth >> 11) & 0x00FF);
                    t_buf[7]  = (char)( (echoData[i].depth >> 1) & 0x00FF);
                    t_buf[8]  = (char)( (echoData[i].depth << 7) & 0x00FF);
                    t_buf[8] |= (char)( (echoData[i].temp >> 2) & 0x00FF);
                    t_buf[9]  = (char)( (echoData[i].temp << 6) & 0x00FF);
                }
                // debugprintf("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\r\n", t_buf[0], t_buf[1], t_buf[2], t_buf[3], t_buf[4], t_buf[5], t_buf[6], t_buf[7], t_buf[8], t_buf[9]);
            }
            // else
            // {
            //     debugprintf("%02d: \r\n", i);
            // }


            j = dp[i][0];
            k = dp[i][1];
            // debugprintf("pointer: %d %d\r\n", j,k);

            for (i1=0; i1<10; i1++)
            {
                s_msg2[j] |= (t_buf[i1] >> (8-k));
                j++;
                s_msg2[j]  = (t_buf[i1] << k);
            }

#if 0
            if (i==1)
            {
                int xx;
                for (xx=0; xx<=j; xx++)
                {
                    debugprintf("%02X ", s_msg2[xx]);
                }
                debugprintf("\r\n");
            }
#endif
        }

        // BAT

        measure_BAT_leval();

        i = get_battery_level();

        debugprintf("bat : %d\r\n", i);

        s_msg2[561] |= (i >> 5);
        j++;
        s_msg2[562]  = (i << 3);
        s_msg2[563]  = 0xCC;
        s_msg2[564]  = 0xCC;
        s_msg2[565]  = 0xCC;
        s_msg2[566]  = 0xCC;


    }

    {
        int i,j;
        unsigned char ch;

        j = 0;

        for (i=0; i<567; i++)
        {
            ch = s_msg2[i];
            // if ( (ch==0) || (ch==0x0A) || (ch==0x0D) )
            // {
            //     break;
            // }
            // else
            {
                unsigned char tbuf[5];
                // debugprintf("%X",ch);
                sprintf(tbuf,"%02X", ch);
                // modem_printf(tbuf);
                s_msg[i*2+0] = tbuf[0];
                s_msg[i*2+1] = tbuf[1];
            }

        }
        s_msg[567*2+0] = '\r';
        s_msg[567*2+1] = '\n';
        s_msg[568*2+0] = '\0';

    }






    // strncat(s_msg, "EB999", 5);

    // sprintf(tmp_buf,"%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min);
    // strncat(s_msg, tmp_buf, 10);

    // strncat(s_msg, "FFFFFFFFFF", 10);

        // (-)131017   --->>>
    if (is_q_full())
    {
        q_pop();
    }
    q_putData();
    // <<<------


    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());
    // debugstring(s_msg);
    // debugstring(s_msg_c);
    debugstring("\r\n");


    strcat(s_msg, "\r\n");

#if 0
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
#endif

    return (s_msg);

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------



}



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
    debugprintf("Q-put: f(%d) r(%d)",q_front,q_rear);
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




