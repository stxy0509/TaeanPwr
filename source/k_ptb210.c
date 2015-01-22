#define __K_PTB210_C__

#include "k_includes.h"

//#if (SENSOR_PTB210 == 1)

ptb_data_T ptb;

t_SENSOR_Q ptb_q;
ALIGN4 char ptb_q_buf[Q_PTB_MAX];
ALIGN4 char ptb_line[100];


extern SensorBakSize_Type SensorBakSize;

void ptb_q_put(char ch)
{
    ptb_q_buf[ptb_q.rear] = ch;
    ++ptb_q.rear;
    ptb_q.rear %= Q_PTB_MAX;
    ptb_q.len++;
}

char ptb_q_get(void)
{
    char ch;
    if (ptb_q.front == ptb_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = ptb_q_buf[ptb_q.front];
        ++ptb_q.front;
        ptb_q.front %= Q_PTB_MAX;
        ptb_q.len--;
    }
    return ch;
}

void ptb_q_init(void)
{
    ptb_q.rear = 0;
    ptb_q.front = 0;
    ptb_q.len = 0;
}

void set_ptb210_valid(u8 a_val)    { ptb.valid = a_val;   }
u8 is_ptb210_valid(void)           { return ptb.valid;   }
double get_ptb210_press(void)      { return(ptb.pressure_a); }


void init_ptb210_data(void)
{
    ptb.valid = 0;
    ptb.n = 0;
    ptb.pressure = 0.0f;
    ptb.pressure_sum = 0.0f;
    ptb.pressure_a = 0.0f;
}

void task_ptb210(void )
{
    static u16 line_ptr = 0;
    char ch;
    //static double t_pressure;
    double f_pressure;

    TM_watchPTB210 = 0;

    if (fg_start_sensing == 0)  return;

    if (tmr_PTB_data_display >= 10)
    {
        //debugstring("PTB querying-----------------\r\n");
        sb_printstring(SB_S_PTB210, ".p\r\n");

        tmr_PTB_data_display = 0;
        return;
    }

    ch = ptb_q_get();
    if (ch != -1)
    {
        //sensor_status.b.ptb210 = 3;
        sensor_status.w |= (3<<(12*2));

        if (0 == is_special_ch(ch))
        {
            switch (ch)
            {
                case 0x0A:
                    ptb_line[line_ptr++] = 0x0D;
                    ptb_line[line_ptr++] = 0x0A;
                    ptb_line[line_ptr] = '\0';

                    debugstring("\r\n");
                    debugstring(ptb_line);

                    if (sdc_read_detectPin()==SDC_INSERTED)
                    {
                        u32 fsz;
                        //PRINTLINE;
                        sdc_saveDataToFile(FN_PTB210, ptb_line, &fsz);
                        //PRINTVAR(fsz);
                        if (fsz > FSZ_MAX)
                        {
                            SensorBakSize.b.ptb210 = 1;
                        }
                    }


                    // parcing
                    //PRINTLINE;

                    f_pressure = atof(ptb_line)*10.0;
                    if ( (f_pressure >= 9200) && (f_pressure <= 10800) )
                    {
                        ptb.n++;

                        ptb.pressure_sum += f_pressure;
                        ptb.pressure_a = ptb.pressure_sum / (float)ptb.n;
                        ptb.valid = 1;
                    }
                    //PRINT_TIME;
                    debugprintf(" **** ptb210 : %f\r\n", f_pressure);
                    //debugprintf(" **** ptb210*: %f\r\n\r\n", ptb.pressure_a);

                    line_ptr = 0;
                    break;

                case 0x0D:
                case ' ':
                case '\0':
                    break;

                default:
                    ptb_line[line_ptr++] = ch;
                    break;
            }
        }
    }
}



//#endif



