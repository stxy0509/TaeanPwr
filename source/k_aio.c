#define __K_AIO_C__

#include "k_includes.h"


aio_data_T aio;
t_SENSOR_Q aio_q;

ALIGN4 char aio_q_buf[Q_AIO_MAX];
ALIGN4 char aio_line[300];


extern SensorBakSize_Type SensorBakSize;

char is_special_ch_aio(char a_ch)
{
  	switch (a_ch)
  	{
    		case 0x09:	//tab
    		case 0x0A:	//LF
    		case 0x0D:	//CR
      			return 0;
      			break;
  	}

  	if ((a_ch < 0x20)||(a_ch > 0x7E)) return 1;
  	return 0;
}



//------------------------------------------------------------
// AIO :
// 		interval : ? sec
//
//000.9, 193, +027.2, 068, 1000.7, 12.1, *1798                  //old
//000.9(ws), 193(wd), +027.2(temp), 068(humi), 1000.7(press), 12.1, *1798                  //old
//
//000.9, 193, +027.2, 068, 1000.7, 12.1, *1798                  //old
//000.2, 125, +015.9, 049, 1016.1, 00.0, 00.0, 11.9, U0, *2250  //new
//000.2,125,+015.9,049,1016.1,00.0,00.0,11.9,U0,*2250
//
//---141107 @ 부산청학부두
//M0;+019.7;+014.4;+019.4;+071.4;+1021.8;+009.1;+026.8;+00000.00;+000;+000.0;
//------------------------------------------------------------

void aio_q_put(char ch)
{
    aio_q_buf[aio_q.rear] = ch;
    ++aio_q.rear;
    aio_q.rear %= Q_AIO_MAX;
    aio_q.len++;
}

char aio_q_get(void)
{
    char ch;
    if (aio_q.front == aio_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = aio_q_buf[aio_q.front];
        ++aio_q.front;
        aio_q.front %= Q_AIO_MAX;
        aio_q.len--;
    }
    return ch;
}

void aio_q_init(void)
{
    aio_q.rear = 0;
    aio_q.front = 0;
    aio_q.len = 0;
}



void init_aio_data(void)
{
    aio.valid  = 0;
    aio.n = 0;
    aio.ws = 0.0f;
    aio.ws_sum = 0.0f;
    aio.ws_a = 0.0f;
    aio.w_x = 0.0f;
    aio.w_y = 0.0f;
    aio.w_sx = 0.0f;    //sum
    aio.w_sy = 0.0f;
    aio.w_ax = 0.0f;    //average
    aio.w_ay = 0.0f;
    aio.w_vd = 0.0f;    //vector
    //aio.w_vs = 0.0f;

    aio.wd = 0.0f;
    aio.temp = 0.0f;
    aio.temp_sum = 0.0f;
    aio.temp_a = 0.0f;
    aio.humi = 0.0f;
    aio.humi_sum = 0.0f;
    aio.humi_a = 0.0f;

    aio.n_press = 0;;         //131012
    aio.valid_press = 0;     //131012
    aio.press = 0.0f;
    aio.press_sum = 0.0f;
    aio.press_a = 0.0f;
}

void set_aio_valid(u8 a_val)    { aio.valid = a_val;    aio.valid_press = a_val;   }
u8 is_aio_valid(void)           { return aio.valid;   }
u8 is_aio_valid_press(void)     { return aio.valid_press;   }
double get_aio_ws(void)         { return(aio.ws); }
double get_aio_wd(void)         { return(aio.wd); }
double get_aio_temp(void)       { return(aio.temp_a); }
double get_aio_humi(void)       { return(aio.humi_a); }
double get_aio_press(void)      { return(aio.press_a); }




/* --------------------------------------------------
 *  WS500-UMB
 * --------------------------------------------------
 * &_28673_M_00100_34785
 M0;+026.6;+015.7;+028.7;+051.3;+1002.2;+000.5;+129.5;+00000.00;+000;+000.0;
 *
 */

void parsing_aio(char *a_str)
{
    char aio_data[15][15];
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;

    char ch;
    while (1)
    {
        ch = a_str[k++];

        if ((ch== 0x0D) || (ch== 0x0A) || ch=='\0')
        {
            aio_data[i][j] = '\0';
            break;
        }
        else if (ch==';')               // WS500-UMB    // 140922
        {
            aio_data[i][j] = '\0';
            i++;
            j=0;
        }
        else
        {
            if (ch != ' ')
            {
                aio_data[i][j++] = ch;
            }
        }
    }


#if 0
   for (j=0; j<=i; j++)
   {
       debugprintf("%s\r\n",aio_data[j]);
   }
#endif


    if ( (aio_data[0][0]=='M') && (aio_data[0][1]=='0') )
    {
        aio.n++;
        {
            aio.ws = atof(aio_data[6])*10.0f;
            // if (aio.ws < 3.0) aio.ws = 3.0f;        // 풍속 센서최소값 = 0.3m/s
            aio.wd = atof(aio_data[7]);
            aio.temp = atof(aio_data[1])*10.0f;
            aio.humi = atof(aio_data[4]);
            aio.press = atof(aio_data[5])*10.0f;

            aio.temp_sum += aio.temp;
            aio.temp_a = aio.temp_sum / (float)aio.n;

            aio.humi_sum += aio.humi;
            aio.humi_a = aio.humi_sum / (float)aio.n;

            aio.press_sum += aio.press;
            aio.press_a = aio.press_sum / (float)aio.n;

            aio.valid_press  = 1;
            aio.valid  = 1;

            debugprintf(" **** AIO :  %f(ws), %f(wd), %f(t), %f(h)  %f(p)\r\n", aio.ws, aio.wd, aio.temp, aio.humi, aio.press);
        }
    }
}

void task_aio(void)
{
    static u16 line_ptr = 0;
    char ch;

    //PRINTLINE;
    TM_watchAIO = 0;

    ch = aio_q_get();
    if (fg_start_sensing == 0)  return;

    if (ch != -1)
    {
        sensor_status.w |= 3;

        if (0 == is_special_ch_aio(ch))
        {
            switch(ch)
            {
            case 0x0A:
                aio_line[line_ptr++] = 0x0D;
                aio_line[line_ptr++] = 0x0A;
                aio_line[line_ptr] = '\0';

                debugstring("\r\n");
                debugstring(aio_line);
                //debugstring("\r\n");

                if ( (aio_line[0]=='M') && (aio_line[1]=='0') )
                {
                    if (sdc_read_detectPin()==SDC_INSERTED)
                    {
                        u32 fsz;
                        //PRINTLINE;
                        sdc_saveDataToFile(FN_AIO, aio_line, &fsz);
                        //PRINTVAR(fsz);
                        if (fsz > FSZ_MAX)
                        {
                            SensorBakSize.b.aio = 1;
                        }
                    }

                    // parcing
                    parsing_aio(aio_line);
                }

                line_ptr = 0;
                break;

            case 0x0D:
            case '\0':
                //skip
                //PRINTLINE;
                break;

            default:
                aio_line[line_ptr++] = ch;
                break;
          }
        }
    }
    else
    {
        // Query
        if (tmr_AIO_data_display >= 10)
        {
            tmr_AIO_data_display = 0;
            sb_printstring(SB_S_AIO, "M0\r");
        }
    }

}


