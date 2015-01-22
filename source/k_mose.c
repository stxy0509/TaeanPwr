#define __K_MOSE_C__

#include "k_includes.h"
#include "buoy_waves2.h"

#define CHAR_CR     0x0D
#define CHAR_LF     0x0A

#define MOSE_DEBUG_ENABLE
#ifdef MOSE_DEBUG_ENABLE
    #define mose_printf debugprintf
#else
    #define mose_printf(...)
#endif


///-----------------------------------------

//mose_data_T mose;

t_SENSOR_Q mose_q;
ALIGN4 char mose_q_buf[Q_MOSE_MAX];


extern SensorBakSize_Type SensorBakSize;

void mose_q_put(char ch)
{
    mose_q_buf[mose_q.rear] = ch;
    ++mose_q.rear;
    mose_q.rear %= Q_MOSE_MAX;
    mose_q.len++;
}
char mose_q_get(void)
{
    char ch;
    if (mose_q.front == mose_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = mose_q_buf[mose_q.front];
        ++mose_q.front;
        mose_q.front %= Q_MOSE_MAX;
        mose_q.len--;
    }
    return ch;
}

void mose_q_init(void)
{
    mose_q.rear = 0;
    mose_q.front = 0;
    mose_q.len = 0;
}

u32 get_mose_q_len(void)
{
    return mose_q.len;
}

void init_mose_data(void)
{
    // mose.valid = 0;
    // mose.dir = 0.0f;
    // mose.speed = 0.0f;
    // mose.pitch = 0.0f;
    //mose.roll = 0.0f;
}

///-----------------------------------------



#if 0
0      1   2  3  4  5  6  7  8     9       10        11       12
$PDTWL,MOT,LF,10,09,30,23,25,09.25,   0.003,  -1.889,  -0.482,0*11
0      1   2  3  4  5  6  7  8     9       10        11       12
$PDTWL,MOT,HF,10,10,01,00,05,44.25,   0.089,   0.676,   0.095,0*1A
0      1   2  3  4  5  6  7     8  9      10     11      12 13    14  15
$PDTWL,POS,10,10,01,00,10,00.00,32,04.2797,N,125,09.5323,E,  23.3,0.9,1.5*15

HF,
10,10,01,00,05,44.25,   0.089,   0.676,   0.095
#endif

float test_datH[2050][3];
//float test_datL[1024][3];


//======================== HF ===============================
#define Q_MAX_HF   (2048+1)

mose_data_T mbuf[2048+1];   //HF
//static
int front;
//static
int rear;

void q_init_HF_mbuf(void)
{
    int i=0;

    for (i=0; i<Q_MAX_HF; i++)
    {
        mbuf[i].x = mbuf[i].y = mbuf[i].z = 0.0f;
    }
}

void q_init_HF(void)
{
    front = 0;
    rear = 0;
}

void q_clear_HF(void)
{
    front = rear;
}

void q_put_HF(mose_data_T k)
{
    if (((rear+1)%Q_MAX_HF) == front)
    {
        //full --> pop 1개
        ++front;
        front %= Q_MAX_HF;
    }
    mbuf[rear] = k;
    ++rear;
    rear %= Q_MAX_HF;
}

int q_is_empty_HF(void)
{
    if (front == rear)  //empty???
    {
        return 1;
    }
    return 0;
}

mose_data_T q_get_HF(void)
{
    mose_data_T k;
    k = mbuf[front];
    front++;
    front %= Q_MAX_HF;
    return k;
}

mose_data_T q_read_HF(int t_front)
{
    return ( mbuf[t_front]);
}

int q_start_position_HF(void)
{
    return front;
}


//======================== LF ===============================

#define Q_MAX_LF   (1024+1)

mose_data_T mbuf_L[1024+1]; //LF
static int front_L;
static int rear_L;

void q_init_LF_mbuf(void)
{
    int i=0;

    for (i=0; i<Q_MAX_LF; i++)
    {
        mbuf_L[i].x = mbuf_L[i].y = mbuf_L[i].z = 0.0f;
    }
}

void q_init_LF(void)
{
    front_L = 0;
    rear_L = 0;
}

void q_clear_LF(void)
{
    front_L = rear_L;
}

void q_put_LF(mose_data_T k)
{
    if ((rear_L+1)%Q_MAX_LF == front_L)
    {
        //full --> pop 1개
        ++front_L;
        front_L %= Q_MAX_LF;
    }
    mbuf_L[rear_L] = k;
    ++rear_L;
    rear_L %= Q_MAX_LF;
}

int q_is_empty_LF(void)
{
    if (front_L == rear_L)  //empty???
    {
        return 1;
    }
    return 0;
}

mose_data_T q_get_LF(void)
{
    mose_data_T k;
    k = mbuf_L[front_L];
    ++front_L;
    front_L %= Q_MAX_LF;
    return k;
}

mose_data_T q_read_LF(int t_front_L)
{
    return ( mbuf_L[t_front_L]);
}

int q_start_position_LF(void)
{
    return front_L;
}




#define FFT_NUM_HF    2048
#define FFT_NUM_LF    1024
//#define FFT_NUM     1024



_MOSE_OUTDATA_T mose_out_data;



//void myprintf(const char * rsp,	...);

// int timer_tick=0;
// void timer0isr()
// {
// 	timer_tick++;
// 	//PRINTVAR(timer_tick);
// }



void task_calc(void)
{
    if (evt_calc_wave == 1)
    {
        int j=0;
        int k=0;
        mose_data_T mdata;

        if (get_fgFW_updating())
            return;

        evt_calc_wave = 0;

        //debugprintf("[time : %02d%02d%02d %02d:%02d:%s]\r\n", mose_out_data.year, mose_out_data.mon, mose_out_data.day, mose_out_data.hour,mose_out_data.min, mose_out_data.sec );

// HF ------------------------------------------------------------------
        k = q_start_position_HF();

        for (j=0; j<FFT_NUM_HF; j++)
        {

            {
                mdata = q_read_HF(k);
                 ++k;
                k %= Q_MAX_HF;

                // order of mose data : vertical,x,y ---> change order. 120417a
                //   --> need arrange *RESULT VALUE* in main-CPU when make msg.
                //        before: x1000  ---> after: x100
                test_datH[j][0] = mdata.x;
                test_datH[j][1] = mdata.y;
                test_datH[j][2] = mdata.z;
            }
#if 0
            // data buffer contents parsed..
            debugprintf("[%04d,%04d] %f\t\t%f\t\t%f\r\n",j,k,test_datH[j][0],test_datH[j][1],test_datH[j][2]);
#endif
        }

        PRINTVAR(getTick());
        call_mose1000_HF(test_datH, 30.0f, 0.5f, FFT_NUM_HF, &result_hf);
        PRINTVAR(getTick());

// LF ------------------------------------------------------------------
        k = q_start_position_LF();

        for (j=0; j<FFT_NUM_LF; j++)
        {

            {
                mdata = q_read_LF(k);
                 ++k;
                k %= Q_MAX_LF;

                // order of mose data : vertical,x,y ---> change order. 120417a
                //   --> need arrange *RESULT VALUE* in main-CPU when make msg.
                //        before: x1000  ---> after: x100
                test_datH[j][0] = mdata.x;
                test_datH[j][1] = mdata.y;
                test_datH[j][2] = mdata.z;
            }
#if 0
            // data buffer contents parsed..
            debugprintf("[%04d,%04d] %f\t\t%f\t\t%f\r\n",j,k,test_datH[j][0],test_datH[j][1],test_datH[j][2]);
#endif
        }

        PRINTVAR(getTick());
        call_mose1000_LF(test_datH, 30.0f, 5.0f, FFT_NUM_LF, &result_lf);
        //call_mose1000_LF(test_datL, 30.0f, 5.0f, FFT_NUM_LF, &result_lf);
        PRINTVAR(getTick());
    }
}


/*
0      1   2  3  4  5  6  7  8     9       10        11       12 13
$PDTWL,MOT,LF,10,09,30,23,25,09.25,   0.003,  -1.889,  -0.482, 0*11
0      1   2  3  4  5  6  7  8     9       10        11       12 13
$PDTWL,MOT,HF,10,10,01,00,05,44.25,   0.089,   0.676,   0.095, 0*1A
0      1   2  3  4  5  6  7     8  9      10 11  12      13 14    15  16  17
$PDTWL,POS,10,10,01,00,10,00.00,32,04.2797,N,125,09.5323,E,  23.3,0.9,1.5*15
*/


static double mose_lat=0,mose_lon=0;
static int mose_pos_valid = 0;

double get_mose_lat(void)   {return mose_lat;}
double get_mose_lon(void)   {return mose_lon;}
int get_mose_pos_valid(void)   {return mose_pos_valid;}
void set_mose_pos_valid(int a_flag)   { mose_pos_valid = a_flag;}

void parsing_mose(char *a_str)
{
    char mose_data[20][15];
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;
    //u8 retval = 0;
    //U16 a_level;
    char ch;
    while (1)
    {
        ch = a_str[k++];

        if ((ch== 0x0D) || (ch== 0x0A) || ch=='\0')
        {
            mose_data[i][j] = '\0';
            break;
        }
        else if ( (ch==',') || (ch=='*') )
        {
            mose_data[i][j] = '\0';
            i++;
            j=0;
        }
        else
        {
            if (ch != ' ')
            {
                mose_data[i][j++] = ch;
            }
        }
    }

    #if 0
    for (j=0; j<=i; j++)
    {
        debugprintf("[%s] ",mose_data[j]);
    }
    debugprintf("\r\n");
    #endif
    //PRINTLINE;

    if ( (mose_data[1][0]=='P') && (mose_data[1][1]=='O') && (mose_data[1][2]=='S') )
    {
        // pos
        mose_lat = (atof(mose_data[ 8]) + atof(mose_data[ 9])/60.0f);
        mose_lon = (atof(mose_data[11]) + atof(mose_data[12])/60.0f);
        if ( (mose_lon==0) || (mose_lat==0))
        {
            set_mose_pos_valid(0);
        }
        else
        {
            set_mose_pos_valid(1);
        }
    }

    if (  (mose_data[2][1]=='F') )
    {
        if (  (mose_data[2][0]=='H') )
        {
            // HF
            mose_data_T k;

            k.x = (float)atof(mose_data[11])*100.0f;
            k.y = (float)atof(mose_data[10])*100.0f;
            k.z = (float)atof(mose_data[9])*100.0f;

            q_put_HF(k);
        }
        if (  (mose_data[2][0]=='L') )
        {
            // LF
            mose_data_T k;

            k.x = (float)atof(mose_data[11])*100.0f;
            k.y = (float)atof(mose_data[10])*100.0f;
            k.z = (float)atof(mose_data[9])*100.0f;

            q_put_LF(k);
        }
    }



    #if 0
    {
        double d1,d2;
        d1 = atof(mose_data[1]);
        d2 = atof(mose_data[0]);
        mose.cond = d1;
        mose.temp = d2;
        mose.valid  = 1;

        //PRINT_TIME;
        debugprintf(" **** MOSE: %f(c), %f(t) \r\n\r\n", mose.cond, mose.temp);
    }
    #endif
}


void task_send_msg(void)
{
}



//#if 1



/*
0      1       2        3     4       5 6 7  8     9       11
$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B
0      1       2   3        4     5       6   7    8      9
$GPRMC,083559.00,A,4717.11437,N,00833.91522,E,0.004,77.52,091202,,,A*57

0      1   2  3  4  5  6  7  8     9       10        11       12
$PDTWL,MOT,LF,10,09,30,23,25,09.25,   0.003,  -1.889,  -0.482,0*11
0      1   2  3  4  5  6  7  8     9       10        11       12
$PDTWL,MOT,HF,10,10,01,00,05,44.25,   0.089,   0.676,   0.095,0*1A
0      1   2  3  4  5  6  7     8  9      10     11      12 13    14  15
$PDTWL,POS,10,10,01,00,10,00.00,32,04.2797,N,125,09.5323,E,  23.3,0.9,1.5*15

HF,
10,10,01,00,05,44.25,   0.089,   0.676,   0.095
*/


//float mose_data[1024][3];

ALIGN4 char mose_line[100];
char parsed_block[20][20];

void task_mose(void)
{
    char ch;
    //static int cnt = 0;
    static int line_ptr = 0;
    static int ref_cnt = 0;


    if (evt_calc_wave ==1)
    {
        // calculating..
        return;
    }

    ch = mose_q_get();
    //if (uart_getch(4, &ch));   //UART3_MOSE, &ch))  //UartGetCh(UART0_GPS,&ch))
    if (ch != -1)   //(uart_getch(UART3_MOSE, &ch))  //UartGetCh(UART0_GPS,&ch))
    {
        //sensor_status.b.mose = 3;
        sensor_status.w |= (3<<(1*2));

        if (0 == is_special_ch(ch))
        {
            switch(ch)
            {
              case 0x0A:
                mose_line[line_ptr++] = 0x0D;
                mose_line[line_ptr++] = 0x0A;
                mose_line[line_ptr] = '\0';

                if (tmr_MOSE_data_display > 19)
                {
                    tmr_MOSE_data_display = 0;

                    debugstring("\r\n");
                    debugstring(mose_line);
                }

                if (sdc_read_detectPin()==SDC_INSERTED)
                {
                    u32 fsz;
                    //PRINTLINE;
                    sdc_saveDataToFile(FN_MOSE, mose_line, &fsz);
                    //PRINTVAR(fsz);
                    if (fsz > FSZ_MAX_25M)
                    {
                        SensorBakSize.b.mose = 1;
                    }
                }


                // verify chksum
                {
                    //u8 checksum = 0;
                }

                // parsing
                parsing_mose(mose_line);

                #if 0
                if (ref_cnt == 6)
                {
                    parsing_sbe(mose_line);
                }
                else
                {
                    parsing_ct3919(mose_line);
                }
                #endif

                line_ptr = 0;
                ref_cnt = 0;
                break;

              case 0x0D:
              case 0x11:
              case 0x13:
              case '\0':
                //skip
                //PRINTLINE;
                break;

              case '$':
                line_ptr = 0;
                mose_line[line_ptr++] = ch;
                ref_cnt++;
                break;

              case 'P':
              case 'D':
              case 'T':
              case 'W':
              case 'L':
              case '*':
                mose_line[line_ptr++] = ch;
                ref_cnt++;
                break;

              default:
                mose_line[line_ptr++] = ch;
                break;
            }
        }
    } //if~
} //task_mose


/*
$PDTWL,POS,11,11,01,00,01,10.00,34,43.9136,N,128,22.6923,E,  25.1,0.9,1.2*19
$PDTWL,MOT,HF,11,10,31,23,56,57.25,  -0.009,   0.016,  -0.086,0*17
$PDTWL,MOT,LF,11,10,31,23,16,22.75,   0.020,  -0.412,   0.008,0*10
*/

int parse_input_string(void)
{
    //parsed_block[i][j], mose_line[k]
    int ret_val=0;
    int i = 0;
    int j = 0;
    int k = 0;
    char ch;
    while (1)
    {
        ch = mose_line[k++];
        if ((ch==0x0D) || (ch==0x0A) || ch=='\0')
        {
            parsed_block[i][j] = '\0';
            break;
        }
        else if (ch==',')
        {
            ret_val++;  // # of comma(,)
            parsed_block[i][j] = '\0';
            i++;
            j=0;
        }
        else
        {
            parsed_block[i][j++] = ch;
        }
    }
    return (ret_val);
}

//#endif

