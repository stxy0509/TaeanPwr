#define __K_GPS_C__

#include "k_includes.h"

#define GPS_DEBUG_ENABLE
#ifdef GPS_DEBUG_ENABLE
    #define GPS_DEBUG debugprintf
#else
    #define GPS_DEBUG(...)
#endif


//#if 1
// $GPRMC,hhmmss,status,latitude,N,longitude,E,spd,cog,ddmmyy,mv,mvE,mode*cs<CR><LF>
// $GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57


struct
{
    char time_d[11];
    //int hour;
    //int min;
    //int sec;
    char status;
    //float latitude;
    char latitude_d[12];
    //char latitude_NS;
    //float longitude;
    char longitude_d[13];
    //char longitude_EW;
    char date_d[8];
    //int year;
    //int month;
    //int day;
} gpRMC;

double d1_GpsLatitude;
char d1_GpsLatitude_NS;
double d1_GpsLongitude;
float d1_GpsAltitude;
char d1_GpsLongitude_EW;
int d1_GpsYear;
int d1_GpsMonth;
int d1_GpsDay;
int d1_GpsHour;
int d1_GpsMin;
int d1_GpsSec;



    #if 0
0      1         2        3 4         5 6  7    8    9   10 11    12 13     14
$GPGGA,hhmmss.ss,Latitude,N,Longitude,E,FS,NoSV,HDOP,msl,m,Altref,m,DiffAge,DiffStation*cs<CR><LF>
$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B

$GPGRS,082632.00,1,0.54,0.83,1.00,1.02,-2.12,2.64,-0.71,-1.18,0.25,,,*70
$GPGSA,A,3,23,29,07,08,09,18,26,28,,,,,1.94,1.18,1.54*0D
$GPGSV,3,1,10,23,38,230,44,29,71,156,47,07,29,116,41,08,09,081,36*7F
$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57
    #endif

    #define CHAR_CR		0x0D
    #define CHAR_LF		0x0A

//char gps_line[120];
int is_gprmc_valid = 0;
int fg_gps_hasData = 0;
int fg_gps_hasTime = 0;

int gga_start_gethering = 0;

int idx_wave_data_from_gga=0;

//------------------

gps_data_T gps;

t_SENSOR_Q gps_q;
ALIGN4 char gps_q_buf[Q_GPS_MAX];
ALIGN4 char gps_line[GPS_LINE_MAX];


static int fg_gps_rawdata_display = 0;
int is_gps_rawdata_display(void)
{
    return fg_gps_rawdata_display;
}
void set_gps_rawdata_display(void)
{
    if (fg_gps_rawdata_display==1)
    {
        debugprintf("OFF\r\n");
        fg_gps_rawdata_display = 0;
    }
    else
    {
        debugprintf("ON\r\n");
        fg_gps_rawdata_display = 1;
    }
}



extern SensorBakSize_Type SensorBakSize;

void gps_q_put(char ch)
{
    gps_q_buf[gps_q.rear] = ch;
    ++gps_q.rear;
    gps_q.rear %= Q_GPS_MAX;
    gps_q.len++;
}

char gps_q_get(void)
{
    char ch;
    if (gps_q.front == gps_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = gps_q_buf[gps_q.front];
        ++gps_q.front;
        gps_q.front %= Q_GPS_MAX;
        gps_q.len--;
    }
    return ch;
}

void gps_q_init(void)
{
    gps_q.rear = 0;
    gps_q.front = 0;
    gps_q.len = 0;
}

u32 get_gps_q_len(void)
{
    return gps_q.len;
}

#if 0
//------------------
void gps_neo6q_init(void)
{

/*
1.      $GPGGA Disable HEX 값: B5 62 06 01 08 00 F0 00 00 00 00 00 00 01 00 24
2.      $GPGLL Disable HEX 값: B5 62 06 01 08 00 F0 01 00 00 00 00 00 01 01 2B
3.      $GPGSA Disable HEX 값: B5 62 06 01 08 00 F0 02 00 00 00 00 00 01 02 32
4.      $GPGSA Disable HEX 값: B5 62 06 01 08 00 F0 03 00 00 00 00 00 01 03 39
5.      $GPVTG Disable HEX 값: B5 62 06 01 08 00 F0 05 00 00 00 00 00 01 05 47
6.      $GPRMC Enable HEX 값 : B5 62 06 01 08 00 F0 04 00 01 00 00 00 01 05 45
7.      $PUBX Enable HEX 값  : B5 62 06 01 08 00 F1 00 00 01 01 00 00 00 02 34
*/
    int i;
    char init0[] = {0xB5,0x62,0x06,0x08,0x06,0x00,0xF4,0x01,0x01,0x00,0x01,0x00,0x0B,0x77          };
    char init1[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24};
    char init2[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B};
    char init3[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32};
    char init4[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39};
    char init5[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47};
    char init6[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x01,0x00,0x00,0x00,0x01,0x05,0x45};
    char init7[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF1,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x02,0x34};

    for (i=0;i<14;i++)  UartPutCh(UART0_GPS,init0[i]);    delayms(100);    *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  UartPutCh(UART0_GPS,init1[i]);    delayms(100);    *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  UartPutCh(UART0_GPS,init2[i]);    delayms(100);    *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  UartPutCh(UART0_GPS,init3[i]);    delayms(100);    *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  UartPutCh(UART0_GPS,init4[i]);    delayms(100);    *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  UartPutCh(UART0_GPS,init5[i]);    delayms(100);    *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  UartPutCh(UART0_GPS,init6[i]);    delayms(100);    *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  UartPutCh(UART0_GPS,init7[i]);

#if 0
    // B5 62 06 08 06 00 F4 01 01 00 01 00 0B 77
    debugstring("\r\n----> GPS init : NMEA, 2Hz <----\r\n");
    UartPutCh(UART0_GPS,0xB5);
    UartPutCh(UART0_GPS,0x62);
    UartPutCh(UART0_GPS,0x06);
    UartPutCh(UART0_GPS,0x08);
    UartPutCh(UART0_GPS,0x06);
    UartPutCh(UART0_GPS,0x00);
    UartPutCh(UART0_GPS,0xF4);
    UartPutCh(UART0_GPS,0x01);
    UartPutCh(UART0_GPS,0x01);
    UartPutCh(UART0_GPS,0x00);
    UartPutCh(UART0_GPS,0x01);
    UartPutCh(UART0_GPS,0x00);
    UartPutCh(UART0_GPS,0x0B);
    UartPutCh(UART0_GPS,0x77);
#endif
}
void gps_neo6q_init_pubx(void)
{
    // B5 62 06 01 08 00 F1 00 00 01 01 00 00 00 02 34
    debugstring("\r\n----> GPS init : NMEA, 2Hz <----\r\n");
    UartPutCh(UART0_GPS,0xB5);
    UartPutCh(UART0_GPS,0x62);
    UartPutCh(UART0_GPS,0x06);
    UartPutCh(UART0_GPS,0x01);
    UartPutCh(UART0_GPS,0x08);
    UartPutCh(UART0_GPS,0x00);

    UartPutCh(UART0_GPS,0xF1);
    UartPutCh(UART0_GPS,0x00);
    UartPutCh(UART0_GPS,0x00);

    UartPutCh(UART0_GPS,0x01);
    UartPutCh(UART0_GPS,0x01);
    UartPutCh(UART0_GPS,0x00);
    UartPutCh(UART0_GPS,0x00);
    UartPutCh(UART0_GPS,0x00);

    UartPutCh(UART0_GPS,0x02);
    UartPutCh(UART0_GPS,0x34);
}
#endif


void gps_neo6q_init(void)
{
/*
1.      $GPGGA Disable HEX 값: B5 62 06 01 08 00 F0 00 00 00 00 00 00 01 00 24
2.      $GPGLL Disable HEX 값: B5 62 06 01 08 00 F0 01 00 00 00 00 00 01 01 2B
3.      $GPGSA Disable HEX 값: B5 62 06 01 08 00 F0 02 00 00 00 00 00 01 02 32
4.      $GPGSA Disable HEX 값: B5 62 06 01 08 00 F0 03 00 00 00 00 00 01 03 39
5.      $GPVTG Disable HEX 값: B5 62 06 01 08 00 F0 05 00 00 00 00 00 01 05 47
6.      $GPRMC Enable HEX 값 : B5 62 06 01 08 00 F0 04 00 01 00 00 00 01 05 45
7.      $PUBX Enable HEX 값  : B5 62 06 01 08 00 F1 00 00 01 01 00 00 00 02 34
*/
    int i;
    char init0[] = {0xB5,0x62,0x06,0x08,0x06,0x00,0xF4,0x01,0x01,0x00,0x01,0x00,0x0B,0x77          };
    char init1[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24};
    char init2[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B};
    char init3[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32};
    char init4[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39};
    char init5[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47};
    char init6[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x01,0x00,0x00,0x00,0x01,0x05,0x45};
    char init7[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF1,0x00,0x00,0x01,0x01,0x00,0x00,0x00,0x02,0x34};

    // for (i=0;i<14;i++)  uart_putch(1,init0[i]);    delayms(100);   // *R_WDCNT=WATCHDOG_V;/
    for (i=0;i<16;i++)  uart_putch(1,init1[i]);    delayms(100);   // *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  uart_putch(1,init2[i]);    delayms(100);   // *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  uart_putch(1,init3[i]);    delayms(100);   // *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  uart_putch(1,init4[i]);    delayms(100);   // *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  uart_putch(1,init5[i]);    delayms(100);   // *R_WDCNT=WATCHDOG_V;
    for (i=0;i<16;i++)  uart_putch(1,init6[i]);    delayms(100);   // *R_WDCNT=WATCHDOG_V;
    // for (i=0;i<16;i++)  uart_putch(1,init7[i]);    delayms(100);
}
void gps_neo6q_init_pubx(void)
{
}

void gga_gethering_start(void)
{
}
int is_gga_gethering_completed(void)
{
	return 0;
}


static char parsed_block[17][15];
void parsing_gps(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    char ch;

#if 0
    if (gga_start_gethering==1)
    {
        //debugprintf("%s",gps_line);
    }
#endif

    while (1)
    {
        ch = gps_line[k++];
        if ((ch==0x0D) || (ch==0x0A) || ch=='\0')
        {
            parsed_block[i][j] = '\0';
            break;
        }
        else if ( (ch==',') || (ch=='*') )
        {
            parsed_block[i][j] = '\0';
#if 0
            debugstring("\r\n");
            // debugstring(parsed_block[i]);
            debugprintf("%02d: %s",i,parsed_block[i]);
#endif
            i++;
            j=0;
        }
        else
        {
            parsed_block[i][j++] = ch;
        }
    }
}




    #if 0	//for test
// float adjHeight[20] = {0,1,2,3,4,5,4,3,2,1,0,-1,-2,-3,-4,-5,-4,-3,-2,-1};
int adj_idx = 0;
float adj_lat = 0;
    #endif
int push_gga_data_for_wave()
{
    return 1;
}

int is_gps_valid()
{
    return is_gprmc_valid;
}
void set_gps_valid()
{
    is_gprmc_valid = 1;
}
void clr_gps_valid()
{
    is_gprmc_valid = 0;
}

/*
새로운 GPS 데이타가 파싱되었다. VALID 여부와는 무관...
 */
void set_gps_hasData(int a_value)
{
    fg_gps_hasData = a_value;
}
int is_gps_hasData(void)
{
    return fg_gps_hasData;
}

/*
새로운 GPS 시간을 얻었다...
 */
void set_gps_hasTime(int a_value)
{
    fg_gps_hasTime = a_value;
}
int is_gps_hasTime(void)
{
    return fg_gps_hasTime;
}

int lat_d, lat_m, lat_mf;
int lon_d, lon_m, lon_mf;

int get_gps_lat_d(void)
{
    return lat_d;   //(atoi(parsed_block[3][0])*  10 + atoi(parsed_block[3][1]));
}
int get_gps_lat_m(void)
{
    return lat_m;   //( atoi(parsed_block[3][2])*  10 + atoi(parsed_block[3][3]));
}
int get_gps_lat_mf(void)
{
    return lat_mf;  //(atoi(parsed_block[3][5])*1000 + atoi(parsed_block[3][6])*100 + atoi(parsed_block[3][7])*10 + atoi(parsed_block[3][8]));
}
int get_gps_lon_d(void)
{
    return lon_d;   //( atoi(parsed_block[5][0])*100 + atoi(parsed_block[5][1])*10 + atoi(parsed_block[5][2]) );
}
int get_gps_lon_m(void)
{
    return lon_m;   //(atoi(parsed_block[5][3])*  10 + atoi(parsed_block[5][4]));
}
int get_gps_lon_mf(void)
{
    return lon_mf;  //(atoi(parsed_block[5][6])*1000 + atoi(parsed_block[5][7])*100 + atoi(parsed_block[5][8])*10 + atoi(parsed_block[5][9]));
}


/*
    0      1         2          3 4           5 6 7 8     9       11
    $GPGGA,092725.00,4717.11399,N,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B
    0      1         2 3          4 5           6 7     8     9
    $GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57     // ref_cnt : 19
    0     1  2         3          4 5           6 7       8     9
    $PUBX,00,091436.50,3723.17796,N,12656.07025,E,182.055,G3,5.7,8.1,0.141,0.00,-0.051,,1.19,1.68,1.11,7,0,0*4C
*/


char itoc(int i)
{
    char ch;

    if (i<10)   ch = i+'0';
    else if (i<16) ch = i-10+'A';
    else ch = '\0';
    return ch;
}


int gps_chksum(void)
{
    U8 chksum;
    char ch;
    int i;
    int len = strlen(gps_line);
    int k = 0;

    chksum = gps_line[1];
    for (i=2; i<len; i++)
    {
        ch = gps_line[i];
        if ( ch=='*' )
        {
            if ( itoc(chksum >> 4) == gps_line[i+1] )
            {
                if ( itoc(chksum & 0x0f) == gps_line[i+2] )
                {
                    k = 1;
                }
            }
            //debugprintf("chksum of GPRMC : %x, %d\r\n", chksum, k);
            break;
        }
        else
        {
            chksum ^= ch;
        }
    }
    return k;
}

int chk_GPRMC(void)
{
    int retval = 0;
    if ( (gps_line[0]=='$') && (gps_line[3]=='R') &&
         (gps_line[4]=='M') && (gps_line[5]=='C') )
    {
        retval=1;
    }
    return retval;
}

void task_gps(void)
{
    static int line_ptr = 0;
    // static int ref_cnt = 0;
    // static int ref_cnt_rmc = 0;
    char ch;
    static int sec_before=99;

    TM_watchGPS = 0;


    if (get_gps_q_len() > 500)
    {
        debugstring("gps_q : overflow --> init..\r\n");
        gps_q_init();
        return;
    }

    /*
     * GPS 기능을 사용하지 않는다.
     */
    uart_getch(1,&ch);
    return;




    // ch = gps_q_get();
    if (uart_getch(1,&ch)) //receive
    // if (ch != -1)
    {
#if 1
        if (TRUE == fgGpsAll_print)
        {                     
            uart_putch(0,ch);
        }
#endif

        //sensor_status.b.gps = 3;
        // sensor_status.w |= (3<<(7*2));

        if (0 == is_special_ch(ch))
        {
            // if ((ch<0x20))
            // {
            //     if (!( (ch==0x09) ||(ch==0x0D)||(ch==0x0A) ))
            //     {
            //         return;
            //     }
            // }

            switch(ch)
            {
              case 0x0A:
                gps_line[line_ptr++] = 0x0D;
                gps_line[line_ptr++] = 0x0A;
                gps_line[line_ptr] = '\0';

                if (chk_GPRMC() == 1)   //$GPRMC*
                {
                    // if ( gps_chksum() )
                    // {
                        if (is_gps_rawdata_display()==1)
                        {
                            debugstring(gps_line);
                        }

                        //PRINTVAR(get_gps_q_len());
                        parsing_gps();

                        set_gps_hasData(TRUE);

                        // if ( 'R' == parsed_block[0][3]) // $GPRMC
                        {
                            if ( 'A' == parsed_block[2][0])
                            {
                                //gps.valid = 1;
                                set_gps_valid();
#if 1
                                gps_time.hour = (parsed_block[1][0]-'0')*10 + (parsed_block[1][1]-'0');
                                gps_time.min =  (parsed_block[1][2]-'0')*10 + (parsed_block[1][3]-'0');
                                gps_time.sec =  (parsed_block[1][4]-'0')*10 + (parsed_block[1][5]-'0');
                                gps_time.day =  (parsed_block[9][0]-'0')*10 + (parsed_block[9][1]-'0');
                                gps_time.week = 0;
                                gps_time.mon =  (parsed_block[9][2]-'0')*10 + (parsed_block[9][3]-'0');
                                gps_time.year = (parsed_block[9][4]-'0')*10 + (parsed_block[9][5]-'0') + 2000;
#endif
                                {
                                    int i;
                                    char tbuf[6];
                                    for(i=0;i<2;i++)    tbuf[i] = parsed_block[3][i];     tbuf[2]='\0';     lat_d  =  atoi(tbuf);
                                    for(i=0;i<2;i++)    tbuf[i] = parsed_block[3][i+2];   tbuf[2]='\0';     lat_m  =  atoi(tbuf);
                                    for(i=0;i<4;i++)    tbuf[i] = parsed_block[3][i+5];   tbuf[4]='\0';     lat_mf =  atoi(tbuf);

                                    for(i=0;i<3;i++)    tbuf[i] = parsed_block[5][i];     tbuf[3]='\0';     lon_d  =  atoi(tbuf);
                                    for(i=0;i<2;i++)    tbuf[i] = parsed_block[5][i+3];   tbuf[2]='\0';     lon_m  =  atoi(tbuf);
                                    for(i=0;i<4;i++)    tbuf[i] = parsed_block[5][i+6];   tbuf[4]='\0';     lon_mf =  atoi(tbuf); 
                               }

                                if (sec_before == gps_time.sec)
                                {
                                    set_gps_hasData(FALSE);
                                    sec_before = gps_time.sec;
                                }

                                if ( (1 == get_rtc_sysclk_sync_req()) ||
                                     ( (0 == gps_time.min%2) && (30 == gps_time.sec) ))
                                {
                                    rtc_settime(&gps_time);
                                    set_rtc_sysclk_sync_req(0);  //fg_RTC_sync_req = 0;
                                }
                                set_gps_hasTime(TRUE);

#if 0 
            if (sdc_read_detectPin()==SDC_INSERTED)
            {
                u32 fsz;
                char fname[20];
                char wdata[100];

                sprintf(fname, "%04d%02d%02d_EB.txt", rtc_time.year, rtc_time.mon, rtc_time.day);
                sprintf(wdata, "%04d/%02d/%02d %02d:%02d:%02d %f,%f\r\n", gps_time.year, gps_time.mon, gps_time.day, gps_time.hour, gps_time.min, gps_time.sec, atof(&parsed_block[3][0]), atof(&parsed_block[5][0]));

                sdc_saveDataToFile(fname, wdata, &fsz);
            }
#endif


                            }
                            else
                            {
                                /*
                                    GPS가 Active가 아닐지라도 시간정보를 사용한다..
                                 */
                                if ( (9 == strlen(parsed_block[1])) && (6 == strlen(parsed_block[9]))) 
                                {
                                    gps_time.hour = (parsed_block[1][0]-'0')*10 + (parsed_block[1][1]-'0');
                                    gps_time.min =  (parsed_block[1][2]-'0')*10 + (parsed_block[1][3]-'0');
                                    gps_time.sec =  (parsed_block[1][4]-'0')*10 + (parsed_block[1][5]-'0');

                                    gps_time.day =  (parsed_block[9][0]-'0')*10 + (parsed_block[9][1]-'0');
                                    gps_time.week = 0;
                                    gps_time.mon =  (parsed_block[9][2]-'0')*10 + (parsed_block[9][3]-'0');
                                    gps_time.year = (parsed_block[9][4]-'0')*10 + (parsed_block[9][5]-'0') + 2000;
                                
                                    if (sec_before == gps_time.sec)
                                    {
                                        set_gps_hasData(FALSE);
                                        sec_before = gps_time.sec;
                                    }


                                    if ( (1 == get_rtc_sysclk_sync_req()) ||
                                         ( (0 == gps_time.min%2) && (30 == gps_time.sec) ))
                                    {
                                        rtc_settime(&gps_time);
                                        set_rtc_sysclk_sync_req(0);  //fg_RTC_sync_req = 0;
                                    }
                                    set_gps_hasTime(TRUE);
                                }
                            }
                        }

                    // }
                }
                line_ptr = 0;
                // ref_cnt = 0;
                // ref_cnt_rmc = 0;

                //PRINTLINE;
                break;


              case 0x0D:
              case '\0':
                //skip
                //PRINTLINE;
                break;

              case '$':
                line_ptr = 0;
                // ref_cnt_rmc = 0;
                // gps_line[line_ptr++] = ch;
                // ref_cnt++;
                // ref_cnt_rmc++;
                // break;

              // case 'G':
              // case 'P':
              // case 'R':
              // case 'M':
              // case 'C':
              // case '*':
              //   ref_cnt_rmc++;

              // case ',':
              //   gps_line[line_ptr++] = ch;
              //   // ref_cnt++;
              //   break;

              default:
                gps_line[line_ptr++] = ch;
                break;
            }
        }
    }
}
//#endif


void get_gps_time(rtcTime *a_t)
{
    *a_t = gps_time;
}




#define PI  ((float)3.1415926535898) /* (float)(4.0*atan(1.0)); */

double pos_distance(double P1_latitude/*C10*/, double P1_longitude/*C11*/, double P2_latitude/*C12*/, double P2_longitude/*C13*/)
{
    if ((P1_latitude == P2_latitude)&&(P1_longitude == P2_longitude))
    {
        return 0.0f;
    }

    double e10 = P1_latitude * PI / 180.0f;
    double e11 = P1_longitude * PI / 180.0f;
    double e12 = P2_latitude * PI / 180.0f;
    double e13 = P2_longitude * PI / 180.0f;

    /* 타원체 GRS80 */
    double c16 = 6356752.314140910;
    double c15 = 6378137.000000000;
    double c17 = 0.0033528107;


    // double f15 = c17 + c17 * c17;
    // double f16 = f15 / 2.0f;
    // double f17 = c17 * c17 / 2.0f;
    // double f18 = c17 * c17 / 8.0f;
    // double f19 = c17 * c17 / 16.0f;

    double c18 = e13 - e11;
    double c20 = (1 - c17) * tan(e10);
    double c21 = atan(c20);
    double c22 = sin(c21);
    double c23 = cos(c21);
    double c24 = (1 - c17) * tan(e12);
    double c25 = atan(c24);
    double c26 = sin(c25);
    double c27 = cos(c25);

    double c29 = c18;
    double c31 = (c27 * sin(c29) * c27 * sin(c29)) + (c23 * c26 - c22 * c27 * cos(c29)) * (c23 * c26 - c22 * c27 * cos(c29));
    double c33 = (c22 * c26) + (c23 * c27 * cos(c29));
    double c35 = sqrt(c31) / c33;
    // double c36 = atan(c35);
    double c38 = 0;
    if (c31==0)
    {
        c38 = 0;
    }else{
        c38 = c23 * c27 * sin(c29) / sqrt(c31);
    }

    double c40 = 0;
    if ((cos(asin(c38)) * cos(asin(c38))) == 0)
    {
        c40 = 0;
    }else{
        c40 = c33 - 2 * c22 * c26 / (cos(asin(c38)) * cos(asin(c38)));
    }

    double c41 = cos(asin(c38)) * cos(asin(c38)) * (c15 * c15 - c16 * c16) / (c16 * c16);
    double c43 = 1 + c41 / 16384.0f * (4096.0f + c41 * (-768.0f + c41 * (320.0f - 175.0f * c41)));
    double c45 = c41 / 1024.0f * (256.0f + c41 * (-128.0f + c41 * (74.0f - 47.0f * c41)));
    double c47 = c45 * sqrt(c31) * (c40 + c45 / 4.0f * (c33 * (-1 + 2.0f * c40 * c40) - c45 / 6.0f * c40 * (-3 + 4.0f * c31) * (-3 + 4.0f * c40 * c40)));
    // double c50 = c17 / 16.0f * cos(asin(c38)) * cos(asin(c38)) * (4 + c17 * (4 - 3.0f * cos(asin(c38)) * cos(asin(c38))));
    // double c52 = c18 + (1 - c50) * c17 * c38 * (acos(c33) + c50 * sin(acos(c33)) * (c40 + c50 * c33 * (-1 + 2.0f * c40 * c40)));

    double c54 = c16 * c43 * (atan(c35) - c47);

    // return distance in meter
    return c54;
}


#if 0
public double distance(double P1_latitude/*C10*/, double P1_longitude/*C11*/, double P2_latitude/*C12*/, double P2_longitude/*C13*/)
{
    if ((P1_latitude == P2_latitude)&&(P1_longitude == P2_longitude))
    {
        return 0;
    }

    double e10 = P1_latitude * Math.PI / 180;
    double e11 = P1_longitude * Math.PI / 180;
    double e12 = P2_latitude * Math.PI / 180;
    double e13 = P2_longitude * Math.PI / 180;

    /* 타원체 GRS80 */
    double c16 = 6356752.314140910;
    double c15 = 6378137.000000000;
    double c17 = 0.0033528107;


    double f15 = c17 + c17 * c17;
    double f16 = f15 / 2;
    double f17 = c17 * c17 / 2;
    double f18 = c17 * c17 / 8;
    double f19 = c17 * c17 / 16;

    double c18 = e13 - e11;
    double c20 = (1 - c17) * Math.Tan(e10);
    double c21 = Math.Atan(c20);
    double c22 = Math.Sin(c21);
    double c23 = Math.Cos(c21);
    double c24 = (1 - c17) * Math.Tan(e12);
    double c25 = Math.Atan(c24);
    double c26 = Math.Sin(c25);
    double c27 = Math.Cos(c25);

    double c29 = c18;
    double c31 = (c27 * Math.Sin(c29) * c27 * Math.Sin(c29)) + (c23 * c26 - c22 * c27 * Math.Cos(c29)) * (c23 * c26 - c22 * c27 * Math.Cos(c29));
    double c33 = (c22 * c26) + (c23 * c27 * Math.Cos(c29));
    double c35 = Math.Sqrt(c31) / c33;
    double c36 = Math.Atan(c35);
    double c38 = 0;
    if (c31==0)
    {
        c38 = 0;
    }else{
        c38 = c23 * c27 * Math.Sin(c29) / Math.Sqrt(c31);
    }

    double c40 = 0;
    if ((Math.Cos(Math.Asin(c38)) * Math.Cos(Math.Asin(c38))) == 0)
    {
        c40 = 0;
    }else{
        c40 = c33 - 2 * c22 * c26 / (Math.Cos(Math.Asin(c38)) * Math.Cos(Math.Asin(c38)));
    }

    double c41 = Math.Cos(Math.Asin(c38)) * Math.Cos(Math.Asin(c38)) * (c15 * c15 - c16 * c16) / (c16 * c16);
    double c43 = 1 + c41 / 16384 * (4096 + c41 * (-768 + c41 * (320 - 175 * c41)));
    double c45 = c41 / 1024 * (256 + c41 * (-128 + c41 * (74 - 47 * c41)));
    double c47 = c45 * Math.Sqrt(c31) * (c40 + c45 / 4 * (c33 * (-1 + 2 * c40 * c40) - c45 / 6 * c40 * (-3 + 4 * c31) * (-3 + 4 * c40 * c40)));
    double c50 = c17 / 16 * Math.Cos(Math.Asin(c38)) * Math.Cos(Math.Asin(c38)) * (4 + c17 * (4 - 3 * Math.Cos(Math.Asin(c38)) * Math.Cos(Math.Asin(c38))));
    double c52 = c18 + (1 - c50) * c17 * c38 * (Math.Acos(c33) + c50 * Math.Sin(Math.Acos(c33)) * (c40 + c50 * c33 * (-1 + 2 * c40 * c40)));

    double c54 = c16 * c43 * (Math.Atan(c35) - c47);

    // return distance in meter
    return c54;
}
#endif


int get_gpsTimr_hour(void) { return gps_time.hour;}
int get_gpsTimr_min(void)  { return gps_time.min;}
int get_gpsTimr_sec (void) { return gps_time.sec ;}
int get_gpsTimr_year(void) { return gps_time.year;}
int get_gpsTimr_mon(void)  { return gps_time.mon;}
int get_gpsTimr_day (void) { return gps_time.day ;}


