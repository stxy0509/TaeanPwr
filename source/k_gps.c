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
int gga_start_gethering = 0;

int idx_wave_data_from_gga=0;

//------------------

gps_data_T gps;

t_SENSOR_Q gps_q;
ALIGN4 char gps_q_buf[Q_GPS_MAX];
ALIGN4 char gps_line[GPS_LINE_MAX];


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


//------------------




void gps_neo6q_init(void)
{
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

            //debugstring("\r\n");
            //debugstring(parsed_block[i]);

            i++;
            j=0;
        }
        else
        {
            parsed_block[i][j++] = ch;
        }
    }
}

static void convert_time(char* a_data)
{
    char tmp_str[4];
    int i,j;
    //hour
    j=0;
    for (i=0;i<2;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    d1_GpsHour = atoi(tmp_str);

    //min
    j=0;
    for (i=2;i<4;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    d1_GpsMin = atoi(tmp_str);

    //sec
    j=0;
    for (i=4;i<6;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    d1_GpsSec = atoi(tmp_str);
}

static void convert_date(char* a_data)
{
    char tmp_str[4];
    int i,j;
    //date
    j=0;
    for (i=0;i<2;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    d1_GpsDay = atoi(tmp_str);

    //month
    j=0;
    for (i=2;i<4;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    d1_GpsMonth = atoi(tmp_str);

    //year
    j=0;
    for (i=4;i<6;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    d1_GpsYear = atoi(tmp_str)+2000;
}

static double convert_latitude(char* a_data)
{
    //latitude
#if 1
    int i;
    int j = 0;
    char tmp_str[15];
    double ret_val;
    for (i=0;i<2;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    ret_val = atof(tmp_str);

    j=0;
    for (i=2;i<9;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    ret_val += atof(tmp_str)/60;
    return(ret_val);
#else
    return(atof(a_data));
#endif
}

static double convert_longitude(char* a_data)
{
    //longitude
#if 1
    int i;
    int j = 0;
    char tmp_str[15];
    double ret_val;

    for (i=0;i<3;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    ret_val = atof(tmp_str);

    j=0;
    for (i=3;i<10;i++)
    {
        tmp_str[j++] = a_data[i];
    }
    tmp_str[i]='\0';
    ret_val += atof(tmp_str)/60;

    return(ret_val);
#else
    return(atof(a_data));
#endif
}




    #if 0	//for test
float adjHeight[20] = {0,1,2,3,4,5,4,3,2,1,0,-1,-2,-3,-4,-5,-4,-3,-2,-1};
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


void task_gps(void)
{
    static int line_ptr = 0;
    static int ref_cnt = 0;
    static int ref_cnt_rmc = 0;
    char ch;
    //int i;

    TM_watchGPS = 0;


    if (get_gps_q_len() > 500)
    {
        //debugstring("gps_q : overflow --> init..\r\n");

        gps_q_init();
        return;
    }

    // ch = gps_q_get();
    if (uart_getch(1,&ch)) //receive
    // if (ch != -1)
    {
        //sensor_status.b.gps = 3;
        sensor_status.w |= (3<<(7*2));

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

                // PRINTVAR(ref_cnt);
                // PRINTVAR(ref_cnt_rmc);
                 // debugstring(gps_line);


                if (ref_cnt_rmc == 7)   //$GPRMC*
                {
                    if ( gps_chksum() )
                    {
                        #if 1
                        if (tmr_GPS_data_display >= 60)  //20)
                            debugstring(gps_line);
                        #endif

                        //PRINTVAR(get_gps_q_len());
                        parsing_gps();

                        if ( 'R' == parsed_block[0][3]) // $GPRMC
                        {
                            if ( 'A' == parsed_block[2][0])
                            {
                                //gps.valid = 1;
                                set_gps_valid();

                                gps_time.hour = (parsed_block[1][0]-'0')*10 + (parsed_block[1][1]-'0');
                                gps_time.min =  (parsed_block[1][2]-'0')*10 + (parsed_block[1][3]-'0');
                                gps_time.sec =  (parsed_block[1][4]-'0')*10 + (parsed_block[1][5]-'0');
                                gps_time.day =  (parsed_block[9][0]-'0')*10 + (parsed_block[9][1]-'0');
                                gps_time.week = 0;
                                gps_time.mon =  (parsed_block[9][2]-'0')*10 + (parsed_block[9][3]-'0');
                                gps_time.year = (parsed_block[9][4]-'0')*10 + (parsed_block[9][5]-'0') + 2000;

                                //strcpy(gps.latitude, parsed_block[3]);
                                //strcpy(gps.longitude, parsed_block[5]);

                                {
                                    // latitude
                                    gps.latitude = (u32)(convert_latitude(parsed_block[3])*100000);
                                    gps.longitude = (u32)(convert_longitude(parsed_block[5])*100000);
                                    sprintf(gps.s_lat, "%7d", gps.latitude);
                                    sprintf(gps.s_lon, "%8d", gps.longitude);
                                    //t1 = atof(parsed_block[3]);
                                }

                                if (tmr_GPS_data_display >= 60)  //20)
                                {
                                    //tmr_GPS_data_display = 0;

                                    // debugstring("\r\n");
                                    //PRINT_TIME;
                                    // debugstring(gps_line);
                                    debugprintf(" **** GPS: lat : %7d,  longi : %8u\r\n", gps.latitude, gps.longitude);

                                    if (sdc_read_detectPin()==SDC_INSERTED)
                                    {
                                        u32 fsz;
                                        //PRINTLINE;
                                        sdc_saveDataToFile(FN_GPS, gps_line, &fsz);
                                        //PRINTVAR(fsz);
                                        if (fsz > FSZ_MAX)
                                        {
                                            SensorBakSize.b.gps = 1;
                                        }
                                    }
                                }





                                if (1 == get_rtc_sysclk_sync_req())
                                {

                                    debugstring("RTC sync with GPS\r\n");
                                    debugprintf("\r\nGPS time: %04d.%02d.%02d %02d:%02d:%02d \r\n",gps_time.year, gps_time.mon, gps_time.day, gps_time.hour,gps_time.min,gps_time.sec);
                                    rtc_settime(&gps_time);
                                    //is_updated_min();
                                    //PRINT_TIME;

                                    set_rtc_sysclk_sync_req(0);  //fg_RTC_sync_req = 0;
                                }

                            }

                        }

                        if (tmr_GPS_data_display >= 60)  //20)
                        {
                            tmr_GPS_data_display = 0;
                        }

                    }
                }
                line_ptr = 0;
                ref_cnt = 0;
                ref_cnt_rmc = 0;

                //PRINTLINE;
                break;


              case 0x0D:
              case '\0':
                //skip
                //PRINTLINE;
                break;

              case '$':
                line_ptr = 0;
                ref_cnt_rmc = 0;
                gps_line[line_ptr++] = ch;
                ref_cnt++;
                ref_cnt_rmc++;
                break;

              case 'G':
              case 'P':
              case 'R':
              case 'M':
              case 'C':
              case '*':
                ref_cnt_rmc++;

              case ',':
                gps_line[line_ptr++] = ch;
                ref_cnt++;
                break;

              default:
                gps_line[line_ptr++] = ch;
                break;
            }
        }
    }
}
//#endif


void get_gps_time( rtcTime *a_time)
{
    *a_time = gps_time;
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


    double f15 = c17 + c17 * c17;
    double f16 = f15 / 2.0f;
    double f17 = c17 * c17 / 2.0f;
    double f18 = c17 * c17 / 8.0f;
    double f19 = c17 * c17 / 16.0f;

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
    double c36 = atan(c35);
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
    double c50 = c17 / 16.0f * cos(asin(c38)) * cos(asin(c38)) * (4 + c17 * (4 - 3.0f * cos(asin(c38)) * cos(asin(c38))));
    double c52 = c18 + (1 - c50) * c17 * c38 * (acos(c33) + c50 * sin(acos(c33)) * (c40 + c50 * c33 * (-1 + 2.0f * c40 * c40)));

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





