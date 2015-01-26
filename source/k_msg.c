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
    char tmp_buf[15];

    // 38 byte + '\0'

    // 0.........1.........2.........3.......
    // 01234567890123456789012345678901234567
    // ||||||||||||||||||||||||||||||||||||||
    // K0AAABBCCDDEEFFGGGGGGGHHHHHHHHIIIIIIII
    // K0S01yymmddhhmmLat---Longi----Sensor--

    memset(s_msg,0,MSG_LENGTH);

    //debugstring("\r\n---make msg K0 (start)\r\n");
    debugstring(    "-------------------------------\r\n");
    debugstring(    "     make event message \r\n");
    debugstring(    "-------------------------------\r\n");

    rtc_isSecUpdate();
    sprintf(tmp_buf,"%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min);

    // BUOY_SPEC + 구분코드
    s_msg[0] = BUOY_SPEC;           // BUOY_SPEC
    s_msg[1] = a_code;              // 구분코드
    s_msg[2] = '\0';

    // BUOY_SPEC + BUOY_ID
    strncat(s_msg, BUOY_ID, 3);     // 부이ID (Sxx Exx)
    debugprintf("%s\r\n", s_msg);

    // DATE & TIME
    strncat(s_msg, tmp_buf, 10);
    debugprintf("time: %s\r\n", tmp_buf);

    // position
    switch (a_code)
    {
        case '0':   // 시작알림
        case '7':   // 충돌
        case '8':   // 이탈
            debugprintf("gps: ");

            if (get_mose_pos_valid())
            {
                set_mose_pos_valid(0);

                sprintf(tmp_buf,"%07d",(int)(get_mose_lat()*100000));
                strncat(s_msg, tmp_buf, 7);
                debugprintf("mose pos: %s ", tmp_buf);

                sprintf(tmp_buf,"%08d",(int)(get_mose_lon()*100000));
                strncat(s_msg, tmp_buf, 8);
                debugprintf("%s \r\n", tmp_buf);
            }
            else if (is_gps_valid())
            {
                clr_gps_valid();

                strncat(s_msg, gps.s_lat, 7);
                strncat(s_msg, gps.s_lon, 8);
                debugprintf("gps pos: %s %s\r\n", gps.s_lat, gps.s_lon);
            }
            else
            {
                strncat(s_msg, "FFFFFFFFFFFFFFF", 8);
                debugprintf("FFFFFFF FFFFFFFF\r\n");
            }
            break;
    }

    switch (a_code)
    {
        case '0':   // 시작알림
            debugprintf("sensor: ");
            sprintf(tmp_buf,"%08X", update_sensor_status(0xFFFFFFFF)); //sensor_status.w);
            strcat(s_msg, tmp_buf);
            debugprintf("%s\r\n", tmp_buf);

            sensor_status.w = DFLT_SENSOR;

            // debugprintf("sensor: ");
            // update_sensor_status
            // strcat(s_msg, "FFFFFFFF");
            // debugprintf("FFFFFFFF\r\n");
            break;
        case '7':   // 충돌 - 충격량
            sprintf(tmp_buf,"%03d",get_shock_shk());
            strncat(s_msg, tmp_buf,3);
            debugprintf("%s \r\n", tmp_buf);
            break;
        case '9':   // 저전압
            sprintf(tmp_buf,"%03d", C_LOW_BAT_LIMIT);
            strncat(s_msg, tmp_buf,3);
            debugprintf("ref_bat: %s \r\n", tmp_buf);
            sprintf(tmp_buf,"%03d",(int)get_battery_level());
            strncat(s_msg, tmp_buf,3);
            debugprintf("bat : %s \r\n", tmp_buf);
            break;
    }

PRINTVAR(strlen(s_msg));

    if (is_q_full())
    {
        q_pop();
    }
    q_putData();

    //saveDeferredMsgFile();

    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());
    debugprintf("%s\r\n",s_msg);


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



//------------------------------------------------------------------------
//      char * make_msg_ky(void)
//------------------------------------------------------------------------
char * make_msg_ky(char t_id, int a_result)
{
    char tmp_buf[15];

    // 38 byte + '\0'

    // 0.........1.........2.........3.......
    // 01234567890123456789012345678901234567
    // ||||||||||||||||||||||||||||||||||||||
    // KYAABBBCCDDEEFFGGHHI
    // KYI0S25yymmddhhmmssR

    memset(s_msg,0,MSG_LENGTH);

    debugstring(    "-------------------------------\r\n");
    debugstring(    "     make msg KY (ack)\r\n");
    debugstring(    "-------------------------------\r\n");

    rtc_isSecUpdate();
    sprintf(tmp_buf,"%02d%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min,rtc_time.sec);

    // BUOY_SPEC + 구분코드
    s_msg[0] = BUOY_SPEC;                 // 구분코드
    s_msg[1] = 'Y';                 // 구분코드
    s_msg[2] = '\0';

    // Transaction Code
    strncat(s_msg, "A0", 2);        // temporary id : A0
    s_msg[2] = t_id;                // transaction id

    // BUOY_SPEC + BUOY_ID
    strncat(s_msg, BUOY_ID, 3);     // 부이ID (Sxx Exx)
    debugprintf("%s\r\n", s_msg);

    // DATE & TIME
    strncat(s_msg, tmp_buf, 12);
    debugprintf("time: %s\r\n", tmp_buf);

    if (a_result==1)
    {
        strncat(s_msg, "1", 1);
    }
    else
    {
        strncat(s_msg, "0", 1);
    }
    debugprintf("result: %d\r\n", a_result);


PRINTVAR(strlen(s_msg));

    if (is_q_full())
    {
        q_pop();
    }
    q_putData();

    //saveDeferredMsgFile();

    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());
    debugprintf("%s\r\n",s_msg);


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

    debugstring(    "-------------------------------\r\n");
    debugstring(    "     make msg K1\r\n");
    debugstring(    "-------------------------------\r\n");

    memset(s_msg,0,MSG_LENGTH);


    rtc_isSecUpdate();
    strncat(s_msg, "EB999", 5);

    sprintf(tmp_buf,"%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min);
    strncat(s_msg, tmp_buf, 10);

    strncat(s_msg, "FFFFFFFFFF", 10);

        // (-)131017   --->>>
    if (is_q_full())
    {
        q_pop();
    }
    q_putData();
    // <<<------


    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());
    debugstring(s_msg);
    // debugstring(s_msg_c);
    debugstring("\r\n");


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




    rtc_isSecUpdate();
    sprintf(tmp_buf,"%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min);

    // BUOY_SPEC + 구분코드
    s_msg[0] = BUOY_SPEC;                 // 구분코드
    s_msg[1] = '1';                 // 구분코드
    s_msg[2] = '\0';

    // BUOY_SPEC + BUOY_ID
    strncat(s_msg, BUOY_ID, 3);     // 부이ID (Sxx Exx)
    s_msg[5] = '0';                 // 관측ID (0: 부이, 1: 지진해일, 2자동승강식)
    s_msg[6] = '\0';

#if (BUOY_SPEC == 'D')
    s_msg[6] = '0';                 // 0: Normal, 1: 지진 모드
    if (env.mode == 1)
    {
        s_msg[6] = '1';                 // 0: Normal, 1: 지진 모드
    }
    s_msg[7] = '\0';
#endif
    debugprintf("%s\r\n", s_msg);

    // date time
    strncat(s_msg, tmp_buf, 10);
    debugprintf("time: %s\r\n", tmp_buf);

    // position
    debugprintf("buoy position: ");

    if (get_mose_pos_valid())
    {
        set_mose_pos_valid(0);

        sprintf(tmp_buf,"%07d",(int)(get_mose_lat()*100000));
        strncat(s_msg, tmp_buf, 7);
        debugprintf("mose pos: %s ", tmp_buf);

        sprintf(tmp_buf,"%08d",(int)(get_mose_lon()*100000));
        strncat(s_msg, tmp_buf, 8);
        debugprintf("%s \r\n", tmp_buf);
    }
    else if (is_gps_valid())
    {
        clr_gps_valid();

        strncat(s_msg, gps.s_lat, 7);
        strncat(s_msg, gps.s_lon, 8);
        debugprintf("gps pos: %s %s\r\n", gps.s_lat, gps.s_lon);
    }
    else
    {
        strncat(s_msg, "FFFFFFFFFFFFFFF", 7+8);
        debugprintf("FFFFFFF FFFFFFFF\r\n");
    }

    // 온습도
    debugprintf("internal: ");
    if (1)   //(get_shock_valid() == 1)
    {
        //sprintf(tmp_buf,"%04d",(int)get_hmp155_temp()+3);
        // sprintf(tmp_buf,"%04d",(int)(get_shock_temp()*10));
        sprintf(tmp_buf,"%04d",(int)(getShtTemp()*10));

        strncat(s_msg, tmp_buf,4);
        debugprintf("%s ", tmp_buf);

        //sprintf(tmp_buf,"%03d",(int)(get_shock_humi()));
        sprintf(tmp_buf,"%03d",(int)(getShtHumi()));
        strncat(s_msg, tmp_buf,3);
        debugprintf("%s \r\n", tmp_buf);
    }
    else
    {
        strcat(s_msg, "FFFFFFF");
        debugprintf("%s %s\r\n", "FFFF", "FFF");
    }


    // sensor status
    debugprintf("sensor: ");
    sprintf(tmp_buf,"%08X", update_sensor_status(sensor_status.w)); //sensor_status.w);
    strcat(s_msg, tmp_buf);
    debugprintf("%s\r\n", tmp_buf);

    // sensor_status.w = DFLT_SENSOR;


    //AIO
    debugprintf("aio: ");
    switch (env.s0)
    {
        case '2':
            debugstring("OFF\r\n");

            strcat(s_msg, "FFFFFFFFFFFFFFFFFF");    //FFF(ws) FFF(wd) FFFF(T) FFFFF(P) FFF(H)
            debugprintf("FFF FFF FFFF FFFFF FFF\r\n");
            break;

        case '1':
            if (is_aio_valid() == 1)
            {
                sprintf(tmp_buf,"%03d",(int)get_aio_ws());
                strncat(s_msg, tmp_buf,3);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%03d",(int)get_aio_wd());
                strncat(s_msg, tmp_buf,3);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_aio_temp());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);
            }
            else
            {
                strcat(s_msg, "FFFFFFFFFF");    //FFF(ws) FFF(wd) FFFF(T) FFFFF(P) FFF(H)
                debugprintf("FFF FFF FFFF ");
            }

            if (is_aio_valid_press() == 1)
            {
                sprintf(tmp_buf,"%05d",(int)get_aio_press());
                strncat(s_msg, tmp_buf,5);
                debugprintf("%s ", tmp_buf);
            }
            else
            {
                strcat(s_msg, "FFFFF");    //FFF(ws) FFF(wd) FFFF(T) FFFFF(P) FFF(H)
                debugprintf("FFFFF");
            }

            if (is_aio_valid() == 1)
            {
                if ( ((int)get_aio_humi()) > 100 )    //(+)131015
                {
                    strcat(s_msg, "FFF");    //FFF(ws) FFF(wd) FFFF(T) FFFFF(P) FFF(H)
                    debugprintf("FFF(humi>100)\r\n");
                }
                else
                {
                    sprintf(tmp_buf,"%03d",((int)get_aio_humi()));
                    strncat(s_msg, tmp_buf,3);
                    debugprintf("%s \r\n", tmp_buf);
                }
            }
            else
            {
                strcat(s_msg, "FFF");    //FFF(ws) FFF(wd) FFFF(T) FFFFF(P) FFF(H)
                debugprintf("FFF\r\n");
            }
            break;
    }
    set_aio_valid(0);   //clear
    init_aio_data();


    //HMP155
    debugprintf("hmp155: ");
    switch (env.sb)
    {
        case '2':
            debugstring("OFF\r\n");
            strcat(s_msg, "FFFF");    //"0167"
            debugprintf("FFFF\r\n");
            break;
        case '1':
            if (is_hmp155_valid() == 1)
            {
                sprintf(tmp_buf,"%04d",(int)get_hmp155_temp());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s \r\n", tmp_buf);

                set_hmp155_valid(0);   //clear
                init_hmp155_data();
            }
            else
            {
                strcat(s_msg, "FFFF");    //"0167"
                debugprintf("FFFF\r\n");
            }
            break;
    }



    //PTB210
    debugprintf("ptb210: ");
    switch (env.sc)
    {
        case '2':
            debugstring("OFF\r\n");
            strcat(s_msg, "FFFFF");    //"10040"
            debugprintf("FFFFF\r\n");
            break;
        case '1':
            if (is_ptb210_valid() == 1)
            {
                sprintf(tmp_buf,"%05d",(int)get_ptb210_press());
                strncat(s_msg, tmp_buf,5);
                debugprintf("%s \r\n", tmp_buf);

                set_ptb210_valid(0);   //clear
                init_ptb210_data();
            }
            else
            {
                strcat(s_msg, "FFFFF");    //"10040"
                debugprintf("FFFFF\r\n");
            }
            break;
    }

    //CT
    debugprintf("ct: ");
    switch (env.s3)
    {
        case '2':
            debugstring("OFF\r\n");
            strcat(s_msg, "FFFFFFFF");     //3624 0142
            debugprintf("FFFF FFFF\r\n");
            break;
        case '1':
            if (is_ct_valid()==1)
            {
                sprintf(tmp_buf,"%04d",(int)get_ct_cond());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_ct_temp());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s \r\n", tmp_buf);

                set_ct_valid(0);
                init_ct_data();
            }
            else
            {
                strcat(s_msg, "FFFFFFFF");     //3624 0142
                debugprintf("FFFF FFFF\r\n");
            }
            break;
    }




    //DCS
    debugprintf("dcs: ");
    switch (env.s5)
    {
        case '2':
            debugstring("OFF\r\n");
            strcat(s_msg, "FFFFFFF");       //0896 141
            debugprintf("FFF FFFF\r\n");
            break;
        case '1':

            if (is_dcs_valid()==1)
            {
                sprintf(tmp_buf,"%04d",(int)get_dcs_speed());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%03d",(int)get_dcs_dir());
                strncat(s_msg, tmp_buf,3);
                debugprintf("%s \r\n", tmp_buf);

                set_dcs_valid(0);
                init_dcs_data();
            }
            else
            {
                strcat(s_msg, "FFFFFFF");       //0896 141
                debugprintf("FFF FFFF\r\n");
            }
            break;
    }

// float get_mose_Hmax(void);
// float get_mose_Tmax(void);
// float get_mose_H3(void);
// float get_mose_t3(void);
// float get_mose_pwd(void);
// int get_mose_data_valid(void);
// void set_mose_data_valid(int a_val);

    //MOSE
    debugprintf("MOSE: ");
    switch (env.s1)
    {
        case '2':
            debugstring("OFF\r\n");
            debugprintf("HF: ");
            strcat(s_msg, "FFFFFFFFFFFFFFFFFFF");   //0038 9200 0033 8125 162
            debugprintf("FFFF FFFF FFFF FFFF FFF\r\n");

            debugprintf("LF: ");
            strcat(s_msg, "FFFFFFFFFFFFFFFFFFF");   //0038 9200 0033 8125 162
            debugprintf("FFFF FFFF FFFF FFFF FFF\r\n");
            break;
        case '1':

            if (get_mose_data_valid())
            {
                debugprintf("HF: ");
                sprintf(tmp_buf,"%04d",(int)get_mose_Hmax());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_mose_Tmax());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_mose_H3());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_mose_t3());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%03d",(int)get_mose_pwd());
                strncat(s_msg, tmp_buf,3);
                debugprintf("%s ", tmp_buf);

                debugprintf("LF: ");
                sprintf(tmp_buf,"%04d",(int)get_moseL_Hmax());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_moseL_Tmax());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_moseL_H3());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%04d",(int)get_moseL_t3());
                strncat(s_msg, tmp_buf,4);
                debugprintf("%s ", tmp_buf);

                sprintf(tmp_buf,"%03d",(int)get_moseL_pwd());
                strncat(s_msg, tmp_buf,3);
                debugprintf("%s ", tmp_buf);

                set_mose_data_valid(0);
            }
            else
            {
                debugprintf("HF: ");
                strcat(s_msg, "FFFFFFFFFFFFFFFFFFF");   //0038 9200 0033 8125 162
                debugprintf("FFFF FFFF FFFF FFFF FFF\r\n");

                debugprintf("LF: ");
                strcat(s_msg, "FFFFFFFFFFFFFFFFFFF");   //0038 9200 0033 8125 162
                debugprintf("FFFF FFFF FFFF FFFF FFF\r\n");
            }
            break;
    }


    //BAT
    debugprintf("bat: ");
    sprintf(tmp_buf,"%03d", get_battery_level());
    strncat(s_msg, tmp_buf,3);
    debugprintf("%s \r\n", tmp_buf);


#if (BUOY_SPEC == 'K')

#ifdef BUOY_WEST
    strcat(s_msg, "FFFFFFFFFF");
    strcat(s_msg, "FFFFFFFFFF");
    strcat(s_msg, "FFFFFFFFFF");
    strcat(s_msg, "FFFFFFFFFF");
    strcat(s_msg, "FFFFFFFFF");
#else
    // 동해부이는 제외함.
    strcat(s_msg, "<USN>");
    {   // ATM
        int use = 0;
        if (is_atm1_valid())
        {
            if (is_atm2_valid())
            {
                if (get_atm1_len() < get_atm2_len())
                {
                    // use atm2
                    use = 2;
                }
                else
                {
                    // use atm1
                    use = 1;
                }
            }
            else
            {
                // use atm1
                use = 1;
            }
        }
        else
        {
            if (is_atm2_valid())
            {
                // use atm2
                use = 2;
            }
        }

        if (use==1)
        {
            strcat(s_msg, atm1_data);
        }
        else if  (use==2)
        {
            strcat(s_msg, atm2_data);
        }
        clr_atm1_valid();
        clr_atm2_valid();
    }
#endif
#endif

PRINTVAR(strlen(s_msg));


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

    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());
    debugstring(s_msg);
    // debugstring(s_msg_c);
    debugstring("\r\n");


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



#if (BUOY_SPEC == 'D')

//------------------------------------------------------------
void make_msg_press_5(void)
{
    // 지진 데이타 (지진모드 : 5분 전송==수신즉시 전송한다)

    // char tmp_buf[50];
    // deepPressure_T t;
    int i;

    // 202 byte + '\0'
    // 0...................................................................................................1...................................................................................................2.
    // 0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........0.
    // 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // D1E8811yymmddhhmmssHeaPitcRollTemp-CondBat1-------2-------3-------4-------5-------6-------7-------8-------9-------10------11------12------13------14------15------16------17------18------19------20------

    debugstring(    "-------------------------------\r\n");
    debugstring(    "     make msg D1Exx11\r\n");
    debugstring(    "-------------------------------\r\n");

    // memset(s_msg,0,MSG_LENGTH);

    // dp_stk_pop(&t);
    // dp_stk_print();

    // rtc_isSecUpdate();
    // sprintf(tmp_buf,"%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min);

    // BUOY_SPEC + 구분코드
    s_msg_e05[0] = BUOY_SPEC;           // 구분코드
    s_msg_e05[1] = '1';                 // 구분코드
    // s_msg_e05[2] = '\0';

    // BUOY_SPEC + BUOY_ID
    // strncat(s_msg, BUOY_ID, 3);     // 부이ID (Sxx Exx)
    strncpy(s_msg_e05+2, BUOY_ID, 3);     // 부이ID (Sxx Exx)
    s_msg_e05[5] = '1';                 // 관측ID (0: 부이, 1: 지진해일, 2자동승강식)
    s_msg_e05[6] = '1';                 // 0: Normal, 1: 지진 모드
    // s_msg_e05[7] = '\0';
    // debugprintf("%s\r\n", s_msg);

    // date time
    strncpy(s_msg_e05+7, time_buf, 12);
    // debugprintf("time: %s\r\n", t.date);

    s_msg_e05[202] = '\0';

    for (i = 0; i < MSG_LENGTH; ++i)
    {
        s_msg[i] = s_msg_e05[i];
        s_msg_e05[i] = '9';
        // sprintf(tmp_buf,"%08d",t.press[i]);
        // strncat(s_msg, tmp_buf, 8);
    }

PRINTVAR(strlen(s_msg));

    if (is_q_full())
    {
        q_pop();
    }
    q_putData();
    //saveDeferredMsgFile();

    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());
    debugstring(s_msg);
    debugstring("\r\n");


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
    // return (s_msg);
}



//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
void make_msg_press_30(void)
{
    // 지진 데이타 (Normal 모드 : 30분 전송)

    char tmp_buf[50];
    deepPressure_T t;
    int i,j,k;
    int sno;
    char pressure[6][170];
    char header[45];

    // 202 byte + '\0'
    // 0...................................................................................................1...................................................................................................2.
    // 0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........0.
    // 0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901
    // ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    // D1E8811yymmddhhmmssHeaPitcRollTemp-CondBat1-------2-------3-------4-------5-------6-------7-------8-------9-------10------11------12------13------14------15------16------17------18------19------20------

    debugstring(    "-------------------------------\r\n");
    debugstring(    "     make msg D1Exx11\r\n");
    debugstring(    "-------------------------------\r\n");

    // memset(s_msg,0,MSG_LENGTH);

    // dp_stk_pop(&t);
    // dp_stk_print();

    // rtc_isSecUpdate();
    // sprintf(tmp_buf,"%02d%02d%02d%02d%02d",rtc_time.year%100,rtc_time.mon,rtc_time.day,rtc_time.hour,rtc_time.min);

    // BUOY_SPEC + 구분코드
    s_msg_e30[0] = BUOY_SPEC;           // 구분코드
    s_msg_e30[1] = '1';                 // 구분코드
    // s_msg_e30[2] = '\0';

    // BUOY_SPEC + BUOY_ID
    // strncat(s_msg, BUOY_ID, 3);     // 부이ID (Sxx Exx)
    strncpy(s_msg_e30+2, BUOY_ID, 3);     // 부이ID (Sxx Exx)
    s_msg_e30[5] = '1';                 // 관측ID (0: 부이, 1: 지진해일, 2자동승강식)
    s_msg_e30[6] = '0';                 // 0: Normal, 1: 지진 모드
    // s_msg_e30[7] = '\0';
    // debugprintf("%s\r\n", s_msg);

    // date time
    strncpy(s_msg_e30+7, time_buf, 12);
    // debugprintf("time: %s\r\n", t.date);

    s_msg_e30[202+(20*8*5)] = '\0';

    for (i = 0; i < MSG_LENGTH; ++i)
    {
        s_msg[i] = s_msg_e30[i];
        s_msg_e30[i] = '9';
        // sprintf(tmp_buf,"%08d",t.press[i]);
        // strncat(s_msg, tmp_buf, 8);
    }


PRINTVAR(strlen(s_msg));

    if (is_q_full())
    {
        q_pop();
    }
    q_putData();
    //saveDeferredMsgFile();

    debugprintf("q_put --> Q[%d]\r\n",is_q_dataNum());
    debugstring(s_msg);
    debugstring("\r\n");


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
    // return (s_msg);

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




