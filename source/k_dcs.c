#define __K_DCS_C__

#include "k_includes.h"

dcs_data_T dcs;

t_SENSOR_Q dcs_q;
ALIGN4 char dcs_q_buf[Q_DCS_MAX];
ALIGN4 char dcs_line[512];


extern SensorBakSize_Type SensorBakSize;

void dcs_q_put(char ch)
{
    dcs_q_buf[dcs_q.rear] = ch;
    ++dcs_q.rear;
    dcs_q.rear %= Q_DCS_MAX;
    dcs_q.len++;
}
char dcs_q_get(void)
{
    char ch;
    if (dcs_q.front == dcs_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = dcs_q_buf[dcs_q.front];
        ++dcs_q.front;
        dcs_q.front %= Q_DCS_MAX;
        dcs_q.len--;
    }
    return ch;
}

void dcs_q_init(void)
{
    dcs_q.rear = 0;
    dcs_q.front = 0;
    dcs_q.len = 0;
}



void init_dcs_data(void)
{
    dcs.valid = 0;
    dcs.dir = 0.0f;
    dcs.speed = 0.0f;
    dcs.pitch = 0.0f;
    dcs.roll = 0.0f;
}

int is_dcs_valid(void)        {    return dcs.valid;}
void set_dcs_valid(int a_val)    { dcs.valid = a_val;}
double get_dcs_dir(void)      {return dcs.dir;}
// double get_dcs_speed(void)    {return dcs.speed;}
double get_dcs_speed(void)    {return dcs.pitch;}

/*
409 byte
MEASUREMENT 4420    365 Abs Speed(cm/s) 1.344260E+01    Direction(Deg.M)    5.714996E+01    North(cm/s) 7.291832E+00    East(cm/s)  1.129304E+01    Heading(Deg.M)  2.596979E+02    Tilt X(Deg) 1.480422E+00    Tilt Y(Deg) -1.554279E+00   SP Std(cm/s)    9.355846E+01    Strength(dB)    -3.597727E+01   Ping Count  150 Abs Tilt(Deg)   1.239204E+01    Max Tilt(Deg)   5.400383E+01    Std Tilt(Deg)   1.033722E+01
 */

void parsing_dcs(char *a_str)
{
    char p_data[13][40];
    u8 i = 0;
   u8 j = 0;
   u8 k = 0;
   //u8 retval = 0;
   char ch;
   while (1)
   {
           ch = a_str[k++];

           if ((ch== 0x0D) || (ch== 0x0A) || ch=='\0')
           {
                   p_data[i][j] = '\0';
                   break;
           }
           else if (ch=='\t')
           {
                   p_data[i][j] = '\0';
                   i++;
                   j=0;

                   if (i >12)
                   {
                       i--;
                       break;
                   }

           }
           else
           {
               if (ch != ' ')
               {
                       p_data[i][j++] = ch;
               }
           }
   }

#if 0
   for (j=0; j<=i; j++)
   {
       debugprintf("%s\r\n",p_data[j]);
   }
#endif

#if 1
   //PRINTLINE;

   // if (!strcmp(p_data[0],"MEASUREMENT"))
   if (!strcmp(p_data[1],"4050"))
   {
       dcs.valid = 1;
       // dcs.dir = atof(p_data[4]);
       // dcs.speed = 0.0f;  //atof(p_data[4])*10;
       dcs.pitch = atof(p_data[4]);
       //dcs.roll = atof(p_data[10]);

       //PRINT_TIME;
       debugprintf(" **** 4050: %f(temp)\r\n\r\n",  dcs.pitch);
   }
#endif
}




void task_dcs(void)
{
    static u16 line_ptr = 0;
    char ch;
    //int i;

    TM_watchDCS = 0;

    // ch = dcs_q_get();
    

    if (fg_start_sensing == 0)  return;

    if (uart_getch(2, &ch))
    {
        //sensor_status.b.dcs = 3;
        sensor_status.w |= 3<<(5*2);

        if (0 == is_special_ch(ch))
        {
            // if ((ch<0x20))
            // {
            //     if (!( (ch==0x09) ||(ch==0x0D)||(ch==0x0A) ))
            //     {
            //         return;
            //     }
            // }
            
            // UartPutCh(0, ch);


            if (line_ptr >= 500)
            {
                ch = 0x0A;
            }


            switch(ch)
            {
              case 0x0A:
                dcs_line[line_ptr++] = 0x0D;
                dcs_line[line_ptr++] = 0x0A;
                dcs_line[line_ptr] = '\0';
                debugstring("\r\n");
                debugstring(dcs_line);
                //debugstring("\r\n");

                if (sdc_read_detectPin()==SDC_INSERTED)
                {
                    u32 fsz;
                    //PRINTLINE;
                    sdc_saveDataToFile(FN_DCS, dcs_line, &fsz);
                    //PRINTVAR(fsz);
                    if (fsz > FSZ_MAX)
                    {
                        SensorBakSize.b.dcs = 1;
                    }
                }


                // if (dcs_line[0] == 'M')
                {
                    // parcing
                    parsing_dcs(dcs_line);
                }
                line_ptr = 0;
                break;

              case 0x0D:
              case '\0':
              case '%':
              case '#':
                break;
              default:
                dcs_line[line_ptr++] = ch;
                break;
            }
        }
    }
}


