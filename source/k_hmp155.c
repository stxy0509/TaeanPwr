#define __K_HMP155_C__

#include "k_includes.h"


hmp155_data_T hmp155;
t_SENSOR_Q hmp155_q;

ALIGN4 char hmp155_q_buf[Q_HMP155_MAX];
ALIGN4 char hmp155_line[100];


extern SensorBakSize_Type SensorBakSize;

// char is_special_ch_hmp155(char a_ch)
// {
//   	switch (a_ch)
//   	{
//     		case 0x09:	//tab
//     		case 0x0A:	//LF
//     		case 0x0D:	//CR
//       			return 0;
//       			break;
//   	}

//   	if ((a_ch < 0x20)||(a_ch > 0x7E)) return 1;
//   	return 0;
// }





void hmp155_q_put(char ch)
{
    hmp155_q_buf[hmp155_q.rear] = ch;
    ++hmp155_q.rear;
    hmp155_q.rear %= Q_HMP155_MAX;
    hmp155_q.len++;
}

char hmp155_q_get(void)
{
    char ch;
    if (hmp155_q.front == hmp155_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = hmp155_q_buf[hmp155_q.front];
        ++hmp155_q.front;
        hmp155_q.front %= Q_HMP155_MAX;
        hmp155_q.len--;
    }
    return ch;
}

void hmp155_q_init(void)
{
    hmp155_q.rear = 0;
    hmp155_q.front = 0;
    hmp155_q.len = 0;
}



void init_hmp155_data(void)
{
    hmp155.valid  = 0;
    hmp155.n  = 0;
    hmp155.temp = 0.0f;
    hmp155.temp_sum = 0.0f;
    hmp155.temp_a = 0.0f;
    // hmp155.humi = 0.0f;
    // hmp155.humi_sum = 0.0f;
    // hmp155.humi_a = 0.0f;
}

void set_hmp155_valid(u8 a_val)    { hmp155.valid = a_val;   }
u8 is_hmp155_valid(void)           { return hmp155.valid;   }
double get_hmp155_temp(void)       { return(hmp155.temp_a); }
//double get_hmp155_humi(void)       { return(hmp155.humi_a); }


/*------------------------------------------------------------
 hmp155 :
    interval : ? sec
RH= 32.5 %RH T= 29.9 'C
------------------------------------------------------------*/



void parsing_hmp155(char *a_str)
{
    char hmp155_data[6][10];
    //int idx = 0;
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;

    char ch;

    while (1)
    {
        ch = a_str[k++];

        if ((ch== 0x0D) || (ch== 0x0A) || ch=='\0')
        {
            hmp155_data[i][j] = '\0';
            break;
        }
        else if ( (ch=='=') || (ch=='%') || (ch== 0x27) )
        {
           hmp155_data[i][j] = '\0';
           i++;
           j=0;
        }
        else
        {
           if (ch != ' ')
           {
              hmp155_data[i][j++] = ch;
           }
        }
    }

#if 0
   for (j=0; j<=3; j++)
   {
       debugprintf("%s\r\n",hmp155_data[j]);
   }
#endif
   //PRINTLINE;



    //if (hmp155.n <= 60)
    {
        hmp155.n++;
        hmp155.temp = atof(hmp155_data[3])*10.0f;
        //hmp155.humi = atof(hmp155_data[1]);

        hmp155.temp_sum += hmp155.temp;
        hmp155.temp_a = hmp155.temp_sum / (float)hmp155.n;

        //hmp155.humi_sum += hmp155.humi;
        //hmp155.humi_a = hmp155.humi_sum / (float)hmp155.n;

        hmp155.valid  = 1;

        //PRINT_TIME;
        debugprintf(" **** hmp155 : %f(t)\r\n", hmp155.temp);
        //debugprintf(" **** hmp155*: %f(t)\r\n\r\n", hmp155.temp_a);
    }
}



void task_hmp155(void)
{
    static u16 line_ptr = 0;
    static int ref_char = 0;
    char ch;
    //int i;

    //PRINTLINE;
    TM_watchHMP155 = 0;

    ch = hmp155_q_get();
    if (fg_start_sensing == 0)  return;

    if (ch != -1)
    {
        if (0 == is_special_ch(ch))
        {
            switch(ch)
            {
                case 0x0A:
                    hmp155_line[line_ptr++] = 0x0D;
                    hmp155_line[line_ptr++] = 0x0A;
                    hmp155_line[line_ptr] = '\0';

                    sensor_status.w |= (3<<(11*2));

                    //문자열의 길이를 확인 (최소 20이상이어야 함.)
                    if (ref_char == 10)
                    {
                      //RH= 32.5 %RH T= 29.9 'C   --->>> "RH=%RHT='C"
                      //debugstring("HMP155 string format OK ------------------------------\r\n");



                        if (tmr_HMP_data_display >= 10)
                        {
                            tmr_HMP_data_display = 0;

                            debugstring("\r\n");
                            debugstring(hmp155_line);
                            //debugstring("\r\n");


                            if (sdc_read_detectPin()==SDC_INSERTED)
                            {
                                u32 fsz;
                                //PRINTLINE;
                                sdc_saveDataToFile(FN_HMP155, hmp155_line, &fsz);
                                //PRINTVAR(fsz);
                                if (fsz > FSZ_MAX)
                                {
                                    SensorBakSize.b.hmp155 = 1;
                                }
                            }
                            // parsing
                            parsing_hmp155(hmp155_line);
                        }
                    }

                    line_ptr = 0;
                    ref_char = 0;
                    break;

                case 0x0D:
                case '\0':
                    //skip
                    //PRINTLINE;
                    break;

                case 'R':
                case 'H':
                case 'T':
                case 'C':
                case '=':
                case 0x25:  // %
                case 0x27:  // '
                    ref_char++;
                    hmp155_line[line_ptr++] = ch;
                    break;

                default:
                    hmp155_line[line_ptr++] = ch;
                    break;
            }
        }
    }
}


