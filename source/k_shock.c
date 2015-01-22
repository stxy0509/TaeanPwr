#define __K_SHOCK_C__

#include "k_includes.h"


shock_data_T shock;

t_SENSOR_Q shock_q;
ALIGN4 char shock_q_buf[Q_SHOCK_MAX];
ALIGN4 char shock_line[100];


extern SensorBakSize_Type SensorBakSize;

void shock_q_put(char ch)
{
    shock_q_buf[shock_q.rear] = ch;
    ++shock_q.rear;
    shock_q.rear %= Q_SHOCK_MAX;
    shock_q.len++;
}

char shock_q_get(void)
{
    char ch;
    if (shock_q.front == shock_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = shock_q_buf[shock_q.front];
        ++shock_q.front;
        shock_q.front %= Q_SHOCK_MAX;
        shock_q.len--;
    }
    return ch;
}

void shock_q_init(void)
{
    shock_q.rear = 0;
    shock_q.front = 0;
    shock_q.len = 0;
}

void set_shock_valid(u8 a_val)    { shock.valid = a_val;   }
int  get_shock_valid(void)           { return shock.valid;   }
int get_shock_shk(void)      { return(shock.shk); }
//double get_shock_temp(void)      { return(shock.temp); }
//double get_shock_humi(void)      { return(shock.humi); }


void init_shock_data(void)
{
    shock.valid = 0;
    shock.shk = 0;
    // shock.temp = 0.0f;
    // shock.humi = 0.0f;
}


//static double temp = 0.0f;
//static double humi = 0.0f;

//11.9760,  1.87518,  15.2111, 24 Feb 2012, 11:50:12
void parsing_shock(char *a_str)
{
    // static double temp_t=0;
    // static double temp_h=0;

    char shk_data[3][20];
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;
    //u8 retval = 0;
    //U16 a_level;
    char ch;
    while (1)
    {
        ch = a_str[k++];

// PRINTVAR(ch);

        if ((ch== 0x0D) || (ch== 0x0A) || ch=='\0')
        {
            shk_data[i][j] = '\0';
            break;
        }
        else if (ch==',')
        {
            shk_data[i][j] = '\0';
            i++;
            j=0;
        }
        else
        {
            if (ch != ' ')
            {
                shk_data[i][j++] = ch;
            }
        }
    }

#ifdef DEBUG_SENSOR_SHOCK
    for (j=0; j<=i; j++)
    {
    debugprintf("%s\r\n",shk_data[j]);
    }
#endif
// PRINTLINE;

    if (shk_data[0][0]=='\0')
    {
        //error
    }
    else
    {
        // if (is_aio_valid())
        // {
        //     temp_t =  ((get_aio_temp()+3)/10);
        //     temp_h =  get_aio_humi()+2;
        // }
        // else
        // {
        //     temp_t =  getShtTemp();
        //     temp_h =  getShtHumi();
        // }
        // shock.temp = getShtTemp();  //temp;
        // shock.humi = getShtHumi();    //humi;

        shock.shk  = atoi(shk_data[0]);

        //debugprintf("shock= %d\r\n", shock.shk);

        shock.valid  = 1;

        //PRINT_TIME;
    }
}


static int shk_interval = 0;
void dec_shk_interval(void)
{
    if (shk_interval > 0)   shk_interval--;
}

void task_shock(void )
{
    static u16 line_ptr = 0;
    char ch;
    char tbuf[30];

    //static double t_pressure;
    // double f_pressure;

    TM_watchSHOCK = 0;

    if (fg_start_sensing == 0)  return;


    ch = shock_q_get();

#if 0 //def E01_SHOCK

    if (ch != -1)
    {
        u8 t_shock;

        sensor_status.w |= (3<<(15*2));

        t_shock = 0;

        // debugprintf("shock: %d\r\n",t_shock);
        sprintf(shock_line,"%d\r\n",t_shock);




        parsing_shock(shock_line);

        //debugprintf("shock parsed...(%d)\r\n", shock.valid);

        if (shock.valid  == 1)
        {

            sprintf(tbuf,"%3.1f,%d,%d\r\n", getShtTemp(), (int)getShtHumi(),shock.shk);
            // debugstring("\r\n");
            // debugstring(tbuf);

            //if (shock.valid  == 1)
            {
                //debugprintf("tmr_SHOCK_data_display(%d)\r\n", tmr_SHOCK_data_display);

                // shock.temp *= 10.0f;

                //if (tmr_SHOCK_data_display >= 30)
                {
                    tmr_SHOCK_data_display = 0;

                    // sprintf(tbuf,"%3.1f,%d,%d\r\n", getShtTemp(), (int)getShtHumi(),shock.shk);
                    debugstring("\r\n");
                    debugstring(tbuf);
                    debugprintf(" **** shock: %f(t), %d(h), %d(s) \r\n\r\n", getShtTemp()*10, (int)getShtHumi(), shock.shk);
                }

                // 충돌량 체크
                // if (shock.shk >= env.ref_shock)
                if (shock.shk > env.ref_shock)  // 131008
                {
                    if(shk_interval==0)
                    {
                        shk_interval = 60;
                        make_msg_kx('7');
                    }
                }
            }

            if (sdc_read_detectPin()==SDC_INSERTED)
            {
                u32 fsz;
                //PRINTLINE;
                sdc_saveDataToFile(FN_SHOCK, tbuf, &fsz);
                //PRINTVAR(fsz);
                if (fsz > FSZ_MAX)
                {
                    SensorBakSize.b.shock = 1;
                }
            }
            //return;
        }

        // if (shock.valid  == 1)
        // {


        //     if (sdc_read_detectPin()==SDC_INSERTED)
        //     {
        //         u32 fsz;
        //         //PRINTLINE;
        //         sdc_saveDataToFile(FN_SHOCK, tbuf, &fsz);
        //         //PRINTVAR(fsz);
        //         if (fsz > FSZ_MAX)
        //         {
        //             SensorBakSize.b.shock = 1;
        //         }
        //     }
        // }

    }

#else

    if (ch != -1)
    {

#ifdef DEBUG_SENSOR_SHOCK
        // debugprintf("^%x_%d~ ",ch, ch);

        // sprintf(shock_line, "%d",ch);
        // ch = 0x0A;
#endif

#ifdef SHOCK_W02
        if (ch == 0x00)
        {
            line_ptr = 0;
            return;
        }
        else
        {
            sprintf(shock_line, "%d\r\n",(u8)ch);
// debugprintf("%s",shock_line);
            line_ptr = strlen(shock_line);

            if (line_ptr>0)
            {
                line_ptr--;
            }
            else
            {
                shock_line[0] = '0';
            }
            ch = 0x0A;
// debugprintf("%s\n",shock_line);
        }
#endif
        //sensor_status.b.shock = 3;
        sensor_status.w |= (3<<(15*2));
        // uart_putch(0,ch);

        switch (ch)
        {
            case 0x0A:

                shock_line[line_ptr++] = 0x0D;
                shock_line[line_ptr++] = 0x0A;
                shock_line[line_ptr] = '\0';

// PRINTLINE;
#ifdef DEBUG_SENSOR_SHOCK
                // debugprintf("^%x_%d~ ",ch, ch);
                debugprintf("%s\n",shock_line);
#endif

                parsing_shock(shock_line);

                //debugprintf("shock parsed...(%d)\r\n", shock.valid);

                if (shock.valid  == 1)
                {

                    sprintf(tbuf,"%3.1f,%d,%d\r\n", getShtTemp(), (int)getShtHumi(),shock.shk);
                    // debugstring("\r\n");
                    // debugstring(tbuf);

                    //if (shock.valid  == 1)
                    {
                        //debugprintf("tmr_SHOCK_data_display(%d)\r\n", tmr_SHOCK_data_display);

                        // shock.temp *= 10.0f;
#ifdef DEBUG_SENSOR_SHOCK
                        //if (tmr_SHOCK_data_display >= 1)
#else
                        if (tmr_SHOCK_data_display >= 30)
#endif
                        {
                            tmr_SHOCK_data_display = 0;

                            // sprintf(tbuf,"%3.1f,%d,%d\r\n", getShtTemp(), (int)getShtHumi(),shock.shk);
                            debugstring("\r\n");
                            debugstring(tbuf);
                            debugprintf(" **** shock: %f(t), %d(h), %d(s) \r\n\r\n", getShtTemp()*10, (int)getShtHumi(), shock.shk);
                        }

                        // 충돌량 체크
                        // if (shock.shk >= env.ref_shock)
                        if (shock.shk > env.ref_shock)  // 131008
                        {
                            if(shk_interval==0)
                            {
                                shk_interval = 60;
                                make_msg_kx('7');
                            }
                        }
                    }

                    if (sdc_read_detectPin()==SDC_INSERTED)
                    {
                        u32 fsz;
                        sdc_saveDataToFile(FN_SHOCK, tbuf, &fsz);
                        if (fsz > FSZ_MAX)
                        {
                            SensorBakSize.b.shock = 1;
                        }
                    }
                }
                line_ptr = 0;
                break;

            case 0x0D:
            case ' ':
            case '\0':
                break;

            default:
                // 숫자만 받고 나머지는 무시한다   ---->>>
                // KOGA14 20141022

                if ( (ch >= '0') && (ch <= '9') )
                {
                    shock_line[line_ptr++] = ch;
                }
                // <<<<--------------------
                break;
        }
    }
#endif
}



//#endif



