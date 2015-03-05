#define __K_CT3919_C__

#include "k_includes.h"


ct_data_T ct;
t_SENSOR_Q ct_q;

ALIGN4 char ct_q_buf[Q_CT_MAX];
ALIGN4 char ct_line[100];


extern SensorBakSize_Type SensorBakSize;


static int fg_alti_rawdata_display = 0;
int is_alti_rawdata_display(void)
{
    return fg_alti_rawdata_display;
}
void set_alti_rawdata_display(void)
{
    if (fg_alti_rawdata_display==1)
    {
        debugprintf("OFF\r\n");
        fg_alti_rawdata_display = 0;
    }
    else
    {
        debugprintf("ON\r\n");
        fg_alti_rawdata_display = 1;
    }
}


char is_special_ch(char a_ch)
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

char is_0D_0A_ch(char a_ch)
{
	switch (a_ch)
	{
		case 0x0A:	//LF
		case 0x0D:	//CR
			return 1;
			break;
	}
	return 0;
}

char is_space_tab_ch(char a_ch)
{
	switch (a_ch)
	{
		case ' ':  //space
		case 0x09:	//tab
			return 1;
			break;
	}
	return 0;
}

char is_number_ch(char a_ch)
{
	switch (a_ch)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			return 1;
			break;
	}
	return 0;
}



//------------------------------------------------------------
// CT3919 :
// 		interval : 5sec
//%#	  3919	  1259	     0.0345(ct)	    17.636(temp)
//------------------------------------------------------------

void ct_q_put(char ch)
{
    ct_q_buf[ct_q.rear] = ch;
    ++ct_q.rear;
    ct_q.rear %= Q_CT_MAX;
    ct_q.len++;
}

char ct_q_get(void)
{
    char ch;
    if (ct_q.front == ct_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = ct_q_buf[ct_q.front];
        ++ct_q.front;
        ct_q.front %= Q_CT_MAX;
        ct_q.len--;
    }
    return ch;
}

void ct_q_init(void)
{
    ct_q.rear = 0;
    ct_q.front = 0;
    ct_q.len = 0;
}



void init_ct_data(void)
{
    ct.valid  = 0;
    ct.cond = 0.0f;
    ct.temp = 0.0f;
}

int is_ct_valid(void)   {    return ct.valid;}
void set_ct_valid(int a_val)    { ct.valid = a_val;}
int get_ct_cond(void)    {return ct.cond;}
int get_ct_temp(void)    {return ct.temp;}


void parsing_ct3919(char *a_str)
{
    char ct_data[6][20];
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
            ct_data[i][j] = '\0';
            break;
        }
        else if (ch==',')
        {
            ct_data[i][j] = '\0';
            i++;
            j=0;
        }
        else
        {
            if (ch != ' ')
            {
                ct_data[i][j++] = ch;
            }
        }
    }

#if 0
   for (j=0; j<=i; j++)
   {
       debugprintf("%s\r\n",ct_data[j]);
   }
#endif
   //PRINTLINE;



    if (!strcmp(ct_data[0],"$SDDPT"))
    {
        int d1;
        d1 = (int)(atof(ct_data[1])*10.0f);
        // d2 = atof(ct_data[4])*10.0f;
        ct.cond = d1;
        // ct.temp = d2;
        ct.valid  = 1;

        //PRINT_TIME;
    }

    if (!strcmp(ct_data[0],"$YXMTW"))
    {
        int d2;
        // d2 = atof(ct_data[1])*10.0f;
        d2 = (int)(atof(ct_data[1])*10.0f);
        // ct.cond = d1;
        ct.temp = d2;
        ct.valid  = 1;
    }

    if (!strcmp(ct_data[0],"$SDMTW"))
    {
        int d2;
        // d2 = atof(ct_data[1])*10.0f;
        d2 = (int)(atof(ct_data[1])*10.0f);
        // ct.cond = d1;
        ct.temp = d2;
        ct.valid  = 1;
    }

    // debugprintf(" **** Alti-meter: %d(c), %d(t) \r\n\r\n", ct.cond, ct.temp);
}





//11.9760,  1.87518,  15.2111, 24 Feb 2012, 11:50:12
void parsing_sbe(char *a_str)
{
}





void task_ct3919(void)
{
    static u16 line_ptr = 0;
    static int ref_cnt = 0;
    char ch;

    TM_watchCT3919 = 0;

    if (fg_start_sensing == 0)  return;

    if (uart_getch( PORT_ALTIMETER, &ch))
    {
        //sensor_status.b.ct3919 = 3;
        sensor_status.w |= 3<<(3*2);

        if (0 == is_special_ch(ch))
        {
            switch(ch)
            {
                case 0x0A:
                    ct_line[line_ptr++] = 0x0D;
                    ct_line[line_ptr++] = 0x0A;
                    ct_line[line_ptr] = '\0';


                    if (is_alti_rawdata_display() == 1)
                    {
                        debugstring(ct_line);
                    }

                    // parsing
                    {
                        parsing_ct3919(ct_line);
                    }

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

                case ',':
                case ':':
                    // 11.9760,  1.87518,  15.2111, 24 Feb 2012, 11:50:12
                    ct_line[line_ptr++] = ch;
                    ref_cnt++;
                    break;

                default:
                    ct_line[line_ptr++] = ch;
                    break;
            }
        }
    }
}


