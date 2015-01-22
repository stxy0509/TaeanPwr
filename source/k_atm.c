#define __K_ATM_C__

#include "k_includes.h"

//dcs_data_T dcs;

t_SENSOR_Q atm1_q;
ALIGN4 char atm1_q_buf[Q_ATM_MAX];
ALIGN4 char atm1_line[256];
ALIGN4 char atm1_tmp[600];
ALIGN4 char atm1_data[600];


t_SENSOR_Q atm2_q;
ALIGN4 char atm2_q_buf[Q_ATM_MAX];
ALIGN4 char atm2_line[256];
ALIGN4 char atm2_tmp[600];
ALIGN4 char atm2_data[600];


//>>>>>>>>-------------------------------------->>>>>>>
// #if (BUOY_SPEC == 'D')

// #define Q_DP_MAX  6
// deepPressure_T dp_q_buf[Q_DP_MAX+1];
// t_SENSOR_Q dp_q;

// void dp_q_put(deepPressure_T t)
// {
//     // copy NEW to REAR
//     //dp_q_buf[dp_q.rear] = ch;
//     strncpy(dp_q.date, t.date,12);
//     dp_q.sno = 

//     ++dp_q.rear;
//     dp_q.rear %= Q_DP_MAX;
//     dp_q.len++;
// }

// #endif
//<<<<<<<<-------------------------------------<<<<<<<<<



//>>>>>>>>-------------------------------------->>>>>>>
#if (BUOY_SPEC == 'D')

#define STK_DP_MAX  (10)
deepPressure_T dp_stk_buf[STK_DP_MAX];
SENSOR_STK_t dp_stk;

void dp_stk_init(void)
{
    dp_stk.top = -1;
}


int is_dp_stk_empty(void)
{
    if (dp_stk.top == -1)
    {
        return (1);
    }
    return (0);
}


int dp_stk_push(deepPressure_T t)
{
    // debugstring("--->>> dp stack push...\r\n");
    debugprintf("top= %d\r\n", dp_stk.top);
    // debugprintf("(STK_DP_MAX-1)= %d\r\n", (STK_DP_MAX-1));

    if ( dp_stk.top >= (STK_DP_MAX-1) )
    {
        //Full
// PRINTLINE;
        return(-1);
    }

// PRINTLINE;
    // debugprintf("date=%s sno=%d\r\n", t.date, t.sno);

    dp_stk_buf[++dp_stk.top] = t;
    // debugprintf("top= %d\r\n", dp_stk.top);
    return(1);
}

int dp_stk_pop(deepPressure_T *t)
{
    deepPressure_T t1;

// PRINTLINE;    
    // debugstring("--->>> dp stack pop...\r\n");
    // debugprintf("top= %d\r\n", dp_stk.top);
    
    if (dp_stk.top < 0)
    {
        return(-1);
    }
	t1 = dp_stk_buf[dp_stk.top--];
    *t = t1;
    // debugprintf("t1.date=%s\r\n", t1.date);
    // debugprintf("*t.date=%s\r\n", t->date);

// PRINTLINE;    
    return(1);
}

void dp_stk_print(void)
{
    int i;
    debugstring("\r\n\r\n--->>> dp stack print...\r\n");
    debugprintf("top= %d\r\n", dp_stk.top);

    for (i = dp_stk.top; i >=0 ; i--)
    {
        debugprintf("%s %d\r\n", dp_stk_buf[i].date, dp_stk_buf[i].sno);
    }
    debugstring("<<<--- dp stack print...\r\n\r\n");
}
#endif
//<<<<<<<<-------------------------------------<<<<<<<<<

extern SensorBakSize_Type SensorBakSize;

void atm1_q_put(char ch)
{
    atm1_q_buf[atm1_q.rear] = ch;
    ++atm1_q.rear;
    atm1_q.rear %= Q_ATM_MAX;
    atm1_q.len++;
}
void atm2_q_put(char ch)
{
    atm2_q_buf[atm2_q.rear] = ch;
    ++atm2_q.rear;
    atm2_q.rear %= Q_ATM_MAX;
    atm2_q.len++;
}

char atm1_q_get(void)
{
    char ch;
    if (atm1_q.front == atm1_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = atm1_q_buf[atm1_q.front];
        ++atm1_q.front;
        atm1_q.front %= Q_ATM_MAX;
        atm1_q.len--;
    }
    return ch;
}

void atm1_q_init(void)
{
    atm1_q.rear = 0;
    atm1_q.front = 0;
    atm1_q.len = 0;
}

char atm2_q_get(void)
{
    char ch;
    if (atm2_q.front == atm2_q.rear)
    {
        ch = -1;    //empty
    }
    else
    {
        ch = atm2_q_buf[atm2_q.front];
        ++atm2_q.front;
        atm2_q.front %= Q_ATM_MAX;
        atm2_q.len--;
    }
    return ch;
}

void atm2_q_init(void)
{
    atm2_q.rear = 0;
    atm2_q.front = 0;
    atm2_q.len = 0;
}


void init_atm_data(void)
{
    // dcs.valid = 0;
    // dcs.dir = 0.0f;
    // dcs.speed = 0.0f;
    // dcs.pitch = 0.0f;
    // dcs.roll = 0.0f;
}

void init_dpData(deepPressure_T *t)
{
    int i;

    // debugstring("init_dpData()--->>>\r\n");

    // strcpy(t->date,"999999999999");
    // debugprintf("date: %s\r\n", t->date);

    t->sno= 999;
    t->head=999;
    t->pitch=9999;
    t->roll=9999;
    t->temp=99999;
    t->cond=9999;
    t->bat=999;
    for (i=0; i<20; i++)
    {
        t->press[i]=99999999;
        // debugprintf("press[%d]: %d\r\n", i, t->press[i]);
    }

    // debugstring("<<<----init_dpData()\r\n");

}
//---------------------------------동해부이 --------------------------

#if (BUOY_SPEC == 'D')

void parsing_atm(char *a_str)
{
    static int sno = 0;

    char p_data[60][25];
    int i = 0;
    int j = 0;
    int k = 0;
    int item_no=0;
    //u8 retval = 0;
    char ch;
    int e30_idx = 0;
    // deepPressure_T t;

    while (1)
    {
        ch = a_str[k++];

        if ( ch=='\0')
        {
           p_data[i][j] = '\0';
           break;
        }
        else if ( (ch== ',') )
        {
           p_data[i][j] = '\0';
           i++;
           j=0;
        }
        else
        {
           if (ch != ' ')
           {
               p_data[i][j++] = ch;
           }
        }
    }
    item_no=i;

#if 0
   for (j=0; j<=i; j++)
   {
       debugprintf("%s\r\n",p_data[j]);
   }
#endif
/*
$DATE,000101001500,003                                 
$HEAD,-1      
$PICT,-001.05             
$ROLL,0074.48             
$TEMP,026.02            
$COND,-0.00           
$BATT,234         
$AA,*000116.04131003                    
$BB,*000116.05103685                    
$CC,*000116.06107596                    
$DD,*000116.07046790                    
$EE,*000116.07784629                    
$FF,*000116.08515941                    
$GG,*000116.09360449                    
$HH,*000116.10182424                    
$II,*000116.10875698                    
$JJ,*000116.11392363                    
$KK,*000116.11847726                    
$LL,*000116.12343998                    
$MM,*000116.12855587                    
$NN,*000116.13344850                    
$OO,*000116.13903479                    
$PP,*000116.14347530                    
$QQ,*000116.14651889                    
$RR,*000116.15016348                    
$SS,*000116.15364800                    
$TT,*000116.15603749                    
*/
    {
        float f;
        int d;
        int i,j;
        int idx=0;
        char buf[20];

        // init_dpData(&t);

        // for (i=0;i<=item_no; i++)
        // {
        //     if (!strcmp(p_data[i],"DATE"))
        //     {
        //         strncpy(t.date,p_data[i+1], 12);
        //         t.date[12] = '\0';

        //         t.sno  = atoi(p_data[i+2]);
        //         // debugprintf("date: %s, %d\r\n",t.date, t.sno);
        //         break;
        //     }
        // }

        //strcpy(s_msg_e05,"9999999999999999999");
        // strcpy(s_msg_e30,"9999999999999999999");

        d = 999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"HEAD"))
            {
                f= atof(p_data[i+1]);
                if (f<0)
                {
                    f += 360.0f;
                }
                d = (int)f;
                if ((d>360)||(d<0)) d = 999;
                else if (d==360)    d = 0; 

                // t.head = (int)f;
                break;
            }
        }
        debugprintf("head: %03d\r\n",d);
        if (d != 999)
        {
            sprintf(buf, "%03d", d);
            strncpy(s_msg_e05+19, buf, 3);
            strncpy(s_msg_e30+19, buf, 3);
        }

        d=9999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"PICT"))
            {
                d = (int)(atof(p_data[i+1])*10.0f);
                if ((d < -900)||(d>900))
                    d = 9999;
                break;
            }
        }
        debugprintf("PICT: %04d\r\n",d);
        if (d != 9999)
        {
            sprintf(buf, "%04d", d);
            strncpy(s_msg_e05+22, buf, 4);
            strncpy(s_msg_e30+22, buf, 4);
        }

    // 202 byte + '\0'
    // 0.................. ... .... .... ..... .... ... ........ ........ ........ ........ ........ ........ ........ ..1..... ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ......2.
    // 0.........1........ .2. .... .... 3.... .... .4. ........ 5....... ..6..... ....7... ......8. ........ 9....... ..0..... ....1... ......2. ........ 3....... ..4..... ....5... ......6. ........ 7....... ..8..... ....9... ......0.
    // 0123456789012345678 901 2345 6789 01234 5678 901 23456789 01234567 89012345 67890123 45678901 23456789 01234567 89012345 67890123 45678901 23456789 01234567 89012345 67890123 45678901 23456789 01234567 89012345 67890123 45678901
    // ||||||||||||||||||| ||| |||| |||| ||||| |||| ||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||||||||
    // D1E8811yymmddhhmmss Hea Pitc Roll Temp- Cond Bat 1------- 2------- 3------- 4------- 5------- 6------- 7------- 8------- 9------- 10------ 11------ 12------ 13------ 14------ 15------ 16------ 17------ 18------ 19------ 20------

        d=9999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"ROLL"))
            {
                d = (int)(atof(p_data[i+1])*10.0f);
                // debugprintf("roll=%d\r\n", d);
                if ((d < -900)||(d>900))
                    d = 9999;
                break;
            }
        }
        debugprintf("roll: %04d\r\n",d);
        if (d != 9999)
        {
            sprintf(buf, "%04d", d);
            strncpy(s_msg_e05+26, buf, 4);
            strncpy(s_msg_e30+26, buf, 4);
        }


        d=99999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"TEMP"))
            {
                d = (int)(atof(p_data[i+1])*100.0f);
                if ((d < -9999)||(d>9999))
                    d = 99999;
                break;
            }
        }
        debugprintf("temp: %05d\r\n",d);
        if (d != 99999)
        {
            sprintf(buf, "%05d", d);
            strncpy(s_msg_e05+30, buf, 5);
            strncpy(s_msg_e30+30, buf, 5);
        }


        d=9999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"COND"))
            {
                d = (int)(atof(p_data[i+1])*100.0f);
                if ((d < 0)||(d>9999))
                    d = 9999;
                break;
            }
        }
        debugprintf("cond: %04d\r\n",d);
        if (d != 9999)
        {
            sprintf(buf, "%04d", d);
            strncpy(s_msg_e05+35, buf, 4);
            strncpy(s_msg_e30+35, buf, 4);
        }

    // 202 byte + '\0'
    // 0.................. ... .... .... ..... .... ... ........ ........ ........ ........ ........ ........ ........ ..1..... ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ......2.
    // 0.........1........ .2. .... .... 3.... .... .4. ........ 5....... ..6..... ....7... ......8. ........ 9....... ..0..... ....1... ......2. ........ 3....... ..4..... ....5... ......6. ........ 7....... ..8..... ....9... ......0.
    // 0123456789012345678 901 2345 6789 01234 5678 901 23456789 01234567 89012345 67890123 45678901 23456789 01234567 89012345 67890123 45678901 23456789 01234567 89012345 67890123 45678901 23456789 01234567 89012345 67890123 45678901
    // ||||||||||||||||||| ||| |||| |||| ||||| |||| ||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||||||||
    // D1E8811yymmddhhmmss Hea Pitc Roll Temp- Cond Bat 1------- 2------- 3------- 4------- 5------- 6------- 7------- 8------- 9------- 10------ 11------ 12------ 13------ 14------ 15------ 16------ 17------ 18------ 19------ 20------
      
        d=999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"BATT"))
            {
                d = (int)(atof(p_data[i+1]));
                if ((d < 0)||(d>300))
                    d = 999;
                break;
            }
        }
        debugprintf("bat: %03d\r\n",d);
        if (d != 999)
        {
            sprintf(buf, "%03d", d);
            strncpy(s_msg_e05+39, buf, 3);
            strncpy(s_msg_e30+39, buf, 3);
        }


        rtc_isSecUpdate();
        {
            int m = rtc_time.min;
            if      (m< 5)    e30_idx = 0;
            else if (m<10)    e30_idx = 1;
            else if (m<15)    e30_idx = 2;
            else if (m<20)    e30_idx = 3;
            else if (m<25)    e30_idx = 4;
            else if (m<30)    e30_idx = 5;
            else if (m<35)    e30_idx = 0;
            else if (m<40)    e30_idx = 1;
            else if (m<45)    e30_idx = 2;
            else if (m<50)    e30_idx = 3;
            else if (m<55)    e30_idx = 4;
            else if (m<60)    e30_idx = 5;
        }
        


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"AA"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0])+4);   
                    d = (int)(f*10000.0f);
                }
                break;
            }
        }
        debugprintf("AA: %08d\r\n",d);
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*0), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*0), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"BB"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0])+4);    d = (int)(f*10000.0f);
                    // debugprintf("BB: %d\r\n",t.press[1]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*1), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*1), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"CC"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0])+4);   d = (int)(f*10000.0f);
                    // debugprintf("cc: %d\r\n",t.press[2]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*2), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*2), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"DD"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));   d = (int)(f*10000.0f);
                    // debugprintf("dd: %d\r\n",t.press[3]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*3), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*3), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"EE"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));   d = (int)(f*10000.0f);
                    // debugprintf("ee: %d\r\n",d4]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*4), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*4), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"FF"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("ff: %d\r\n",d5]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*5), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*5), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"GG"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("gg: %d\r\n",d6]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*6), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*6), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"HH"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("hh: %d\r\n",d7]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*7), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*7), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"II"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));      d = (int)(f*10000.0f);
                    // debugprintf("ii: %d\r\n",d8]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*8), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*8), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"JJ"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));      d = (int)(f*10000.0f);
                    // debugprintf("jj: %d\r\n",d9]);
                }
               break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*9), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*9), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"KK"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));    d = (int)(f*10000.0f);
                    // debugprintf("kk: %d\r\n",d10]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*10), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*10), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"LL"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("ll: %d\r\n",d11]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*11), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*11), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"MM"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("mm: %d\r\n",d12]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*12), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*12), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"NN"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("nn: %d\r\n",d13]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*13), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*13), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"OO"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("oo: %d\r\n",d14]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*14), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*14), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"PP"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));      d = (int)(f*10000.0f);
                    // debugprintf("pp: %d\r\n",d15]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*15), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*15), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"QQ"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("qq: %d\r\n",d16]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*16), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*16), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"RR"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));     d = (int)(f*10000.0f);
                    // debugprintf("rr: %d\r\n",d17]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*17), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*17), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"SS"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));    d = (int)(f*10000.0f);
                    // debugprintf("ss: %d\r\n",d18]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*18), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*18), buf, 8);
        }


        d=99999999;
        for (i=0;i<=item_no; i++)
        {
            if (!strcmp(p_data[i],"TT"))
            {
                if (strlen(p_data[i+1])==15)
                {
                    f= atof(&(p_data[i+1][0+4]));      d = (int)(f*10000.0f);
                    // debugprintf("TT: %d\r\n",d19]);
                }
                break;
            }
        }
        if (d != 99999999)
        {
            sprintf(buf, "%08d", d);
            strncpy(s_msg_e05+(42+8*19), buf, 8);
            strncpy(s_msg_e30+(42+(e30_idx*20*8)+8*19), buf, 8);
        }

        s_msg_e05[202] = '\0';
        s_msg_e30[202+(20*8*5)] = '\0';

        debugprintf("s_msg_e05: \r\n");
        // debugstring(s_msg_e05);
        debugstring("\r\n");
        // debugprintf("s_msg_e30: \r\n%s\r\n", s_msg_e30);
        debugprintf("s_msg_e30: \r\n");
        // debugstring(s_msg_e30);
        debugstring("\r\n");



// PRINTLINE;
        // for(i=0; i<20; i++)
        // {
        //     f= (atof((&p_data[16+(i*2)][0]+4)));   t.press[i] = (int)(f*10000.0f);
        //     //debugprintf("t.press[%d]= %d\r\n", i, t.press[i]);
        // }

        // if (t.sno != sno)
        // {
        //     sno = t.sno;

        //     dp_stk_push(t);
        //     dp_stk_print();
        // }
    }




/*
0  DATE,120524123404,331,
3  HEAD,-031.03,
5  PICT,-003.40,
7  ROLL,-001.55,
9  TEMP,020.74,
11 COND,02.27,
13 BATT,215,
15 AA,000115.40953902,
17 BB,000115.40959138,
19 CC,000115.40962721,
21 DD,000115.40974387,
23 EE,000115.40940078,
25 FF,000115.40961176,
27 GG,000115.40987085,
29 HH,000115.40991133,
31 II,000115.40990250,
33 JJ,000115.41027519,
35 KK,000115.41030768,
37 LL,000115.40981151,
39 MM,000115.40958779,
41 NN,000115.40953008,
43 OO,000115.40947402,
45 PP,000115.40941925,
47 QQ,000115.40932456,
49 RR,000115.40921934,
51 SS,000115.40920148,
53 TT,000115.40909300
*/

}
#endif

char is_special_ch_atm(char a_ch)
{
    switch (a_ch)
    {
        case 0x09:  //tab
        case 0x0A:  //LF
        case 0x0D:  //CR
            return 0;
            break;
    }

    if ((a_ch < 0x20)||(a_ch > 0x7E)) return 1;
    return 0;
}


int atm1_valid = 0;
int atm1_len = 0;
void set_atm1_valid(void)   {    atm1_valid = 1;    }
void clr_atm1_valid(void)   {    atm1_valid = 0;    }
int is_atm1_valid(void)     {    return atm1_valid; }
void set_atm1_len(int a_val)   {    atm1_len = a_val;    }
int  get_atm1_len(void)     {    return atm1_len; }

int atm2_valid = 0;
int atm2_len = 0;
void set_atm2_valid(void)   {    atm2_valid = 1;    }
void clr_atm2_valid(void)   {    atm2_valid = 0;    }
int is_atm2_valid(void)     {    return atm2_valid; }
void set_atm2_len(int a_val)   {    atm2_len = a_val;    }
int  get_atm2_len(void)     {    return atm2_len; }


/*
Benthos ATM-88X 4G
LF Frequency Band
Standard Modem version 6.9.7 (4G)
Warning battery low
CONNECT 01200 bits/sec

DATA(0575):$DATE,130721112700,*17
$HEAD,186.9,*20
$PITC,-00.5,*38
$ROLL,-00.8,*26
$TEMP,+29.40,*06
$COND,00.01,*29
$PRES,999.80,*0B
$BATT,23.5,*19
$SBAT,99.9,*13
$Hmax,99.99,*12
$Tmax,99.9,*37
$Hsig,99.99,*1B
$Tsig,99.9,*3E
$Wdir,999,*11
$NUMC,10,*14
$CU0E,-998,*76
$CU0N,-998,*7D
$CU1E,-998,*77
$CU1N,-998,*7C
$CU2E,-998,*74
$CU2N,-998,*7F
$CU3E,-998,*75
$CU3N,-998,*7E
$CU4E,-998,*72
$CU4N,-998,*79
$CU5E,-998,*73
$CU5N,-998,*78
$CU6E,-998,*70
$CU6N,-998,*7B
$CU7E,-998,*71
$CU7N,-998,*7A
$CU8E,-998,*7E
$CU8N,-998,*75
$CU9E,-998,*7F
$CU9N,-998,*74

Source:010  Destination:001
CRC:Pass MPD:01.2 SNR:19.4 AGC:39 SPD:+00.0 CCERR:011
<EOP>
*/

//---------------------------------남해부이-------------------------------------------
#if (BUOY_SPEC == 'K')
void task_atm1(void)
{
    static int line_ptr = 0;
    char ch;

    static char sp[2];  // signal for save start : 검출패턴 ):   ---> DATA(1234):
    static char st[2];  // signal for save stop  : 검출패턴 0x0A S--> (줄바꿈)Source

    static int fg_save_data = 0;
    static int data_idx = 0;


    TM_watchATM1 = 0;

    ch = atm1_q_get();
    

    if (fg_start_sensing == 0)  return;

    if (ch != -1)
    {
        //sensor_status.b.atm1 = 3;
        sensor_status.w |= 3<<(8*2);
        
        if ( ch < 0x20 )
        {
            if (!( (ch==0x09)||(ch==0x0D)||(ch==0x0A) ))
            {
                return;
            }
        }
        else if ( ch > 0x7E)
        {
            return;
        }

        // 다음은 순전히 SD에 저장하기 위함이다.---->
        atm1_line[line_ptr++] = ch;
        atm1_line[line_ptr] = '\0';
        if ( (line_ptr >100) || (ch == 0x0A) )
        {
            //save to SD
            if (sdc_read_detectPin()==SDC_INSERTED)
            {
                u32 fsz;
                //PRINTLINE;
                sdc_saveDataToFile(FN_ATM1, atm1_line, &fsz);
                //PRINTVAR(fsz);
                if (fsz > FSZ_MAX)
                {
                    SensorBakSize.b.atm1 = 1;
                }
            }
            line_ptr = 0;
        }
        // <----


        sp[0] = sp[1];  sp[1] = ch;
        st[0] = st[1];  st[1] = ch;
        if ( (sp[0]==')') && (sp[1]==':') )
        {
            // save start
            fg_save_data = 1;
            data_idx = 0;
        }
        else
        {
            if (fg_save_data==1)
            {
                if ( (st[0]==0x0A) && (st[1]=='S') )
                {
                    // stop save
                    memcpy(atm1_data, atm1_tmp, 600);

// PRINTLINE;
                    debugstring("----------atm1_data-----------\r\n");
                    debugstring(atm1_data);
                    
                    set_atm1_valid();
                    set_atm1_len(data_idx);
                    fg_save_data = 0;
                    data_idx = 0;
                }
                else
                {
                    //save data to atm1_tmp[data_idx]
                    if ( (ch == 0x0D) || (ch==0x0A) )
                    {
                        ch = '!';
                    }
                    atm1_tmp[data_idx++] = ch;
                    atm1_tmp[data_idx] = '\0';
                }
            }
        }
    }
}


void task_atm2(void)
{
    static int line_ptr = 0;
    char ch;

    static char sp[2];  // signal for save start : 검출패턴 ):   ---> DATA(1234):
    static char st[2];  // signal for save stop  : 검출패턴 0x0A S--> (줄바꿈)Source

    static int fg_save_data = 0;
    static int data_idx = 0;


    TM_watchATM2 = 0;

    ch = atm2_q_get();

    if (fg_start_sensing == 0)  return;

    if (ch != -1)
    {
        //sensor_status.b.atm2 = 3;
        sensor_status.w |= 3<<(9*2);

        if ( ch < 0x20 )
        {
            if (!( (ch==0x09)||(ch==0x0D)||(ch==0x0A) ))
            {
                return;
            }
        }
        else if ( ch > 0x7E)
        {
            return;
        }

        // 다음은 순전히 SD에 저장하기 위함이다.---->
        atm2_line[line_ptr++] = ch;
        atm2_line[line_ptr] = '\0';
        if ( (line_ptr >100) || (ch == 0x0A) )
        {
            //save to SD
            if (sdc_read_detectPin()==SDC_INSERTED)
            {
                u32 fsz;
                //PRINTLINE;
                sdc_saveDataToFile(FN_ATM2, atm2_line, &fsz);
                //PRINTVAR(fsz);
                if (fsz > FSZ_MAX)
                {
                    SensorBakSize.b.atm2 = 1;
                }
            }
            line_ptr = 0;
        }
        // <----


        sp[0] = sp[1];  sp[1] = ch;
        st[0] = st[1];  st[1] = ch;
        if ( (sp[0]==')') && (sp[1]==':') )
        {
            // save start
            fg_save_data = 1;
            data_idx = 0;
        }
        else
        {
            if (fg_save_data==1)
            {
                if ( (st[0]==0x0A) && (st[1]=='S') )
                {
                    // stop save
                    memcpy(atm2_data, atm2_tmp, 600);

// PRINTLINE;
                    debugstring("----------atm2_data-----------\r\n");
                    debugstring(atm2_data);
                    
                    set_atm2_valid();
                    set_atm2_len(data_idx);
                    fg_save_data = 0;
                    data_idx = 0;
                }
                else
                {
                    //save data to atm2_tmp[data_idx]
                    if ( (ch == 0x0D) || (ch==0x0A) )
                    {
                        ch = '!';
                    }
                    atm2_tmp[data_idx++] = ch;
                    atm2_tmp[data_idx] = '\0';
                }
            }
        }
    }
}
#endif

//---------------------------------동해부이-------------------------------------------
/*
>Lowpower         
> 
CONNECT 00800 bits/sec                      

DATA(0547):$DATE,000101001500,003                                 
$HEAD,-1      
$PICT,-001.05             
$ROLL,0074.48             
$TEMP,026.02            
$COND,-0.00           
$BATT,234         
$AA,*000116.04131003                    
$BB,*000116.05103685                    
$CC,*000116.06107596                    
$DD,*000116.07046790                    
$EE,*000116.07784629                    
$FF,*000116.08515941                    
$GG,*000116.09360449                    
$HH,*000116.10182424                    
$II,*000116.10875698                    
$JJ,*000116.11392363                    
$KK,*000116.11847726                    
$LL,*000116.12343998                    
$MM,*000116.12855587                    
$NN,*000116.13344850                    
$OO,*000116.13903479                    
$PP,*000116.14347530                    
$QQ,*000116.14651889                    
$RR,*000116.15016348                    
$SS,*000116.15364800                    
$TT,*000116.15603749                    

Source:020  Destination:002                           
CRC:Pass MPD:00.5 SNR:22.9 AGC:18 SPD:+00.0 CCERR:013                                                     
<EOP>     
Lowpower        
> 
*/


#if (BUOY_SPEC == 'D')
void task_atm1(void)
{
    static int line_ptr = 0;
    char ch;

    static char sp[2];  // signal for save start : 검출패턴 ):   ---> DATA(1234):
    static char st[2];  // signal for save stop  : 검출패턴 0x0A S--> (줄바꿈)Source

    static int fg_save_data = 0;
    static int data_idx = 0;

    //char tmp_data[600];

    TM_watchATM1 = 0;

    ch = atm1_q_get();
    

    if (fg_start_sensing == 0)  return;

    if (ch != -1)
    {
        //sensor_status.b.atm1 = 3;
        sensor_status.w |= 3<<(8*2);
        
        // if ((ch<0x20))
        // {
        //     if (!( (ch==0x09) ||(ch==0x0D)||(ch==0x0A) ))
        //     {
        //         return;
        //     }
        // }
        switch (ch)
        {
            case 0x0D:
            case 0x0A:
            case 0x09:
            case '$':
            case '*':
            case '-':
            case ',':
            case '.':
            case '(':
            case ')':
            case ':':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                break;
            default:
                return;
        }


        // 다음은 순전히 SD에 저장하기 위함이다.---->
        atm1_line[line_ptr++] = ch;
        atm1_line[line_ptr] = '\0';
        if ( (line_ptr >100) || (ch == 0x0A) )
        {
            //save to SD
            if (sdc_read_detectPin()==SDC_INSERTED)
            {
                u32 fsz;
                //PRINTLINE;
                sdc_saveDataToFile(FN_ATM1, atm1_line, &fsz);
                //PRINTVAR(fsz);
                if (fsz > FSZ_MAX)
                {
                    SensorBakSize.b.atm1 = 1;
                }
            }
            line_ptr = 0;
        }
        // <----


        sp[0] = sp[1];  sp[1] = ch;
        st[0] = st[1];  st[1] = ch;
        if ( (sp[0]==')') && (sp[1]==':') )
        {
            // save start
            fg_save_data = 1;
            data_idx = 0;
        }
        else
        {
            if (fg_save_data==1)
            {
                if ( (st[0]==0x0A) && (st[1]=='S') )
                {
                    // stop save
                    //memcpy(atm1_data, atm1_tmp, 600);

// PRINTLINE;
                    debugstring(atm1_tmp);

                    {
                        int i = 0;
                        int j = 0;
                        char ch;
                        while(1)
                        {
                            ch = atm1_tmp[i++];
                            if (ch == '\0') 
                            {
                                atm1_data[j++] = ch;
                                break;
                            }

                            switch(ch)
                            {
                                case '$':
                                case '*':
                                case ')':
                                case 0x0D:
                                    break;
                                case ':':
                                case 0x0A:
                                    atm1_data[j++] = ',';
                                    break;
                                default:
                                    atm1_data[j++] = ch;
                                    break;
                            }
                        }
                        // debugstring(atm1_data);

                    }

                    debugstring("----------atm1_data-----------\r\n");
                    parsing_atm(atm1_data);
                    
                    set_atm1_valid();
                    set_atm1_len(data_idx);
                    fg_save_data = 0;
                    data_idx = 0;
                }
                else
                {
                    //save data to atm1_tmp[data_idx]
                    if ( (ch == 0x0D) || (ch==0x0A) )
                    {
                        //ch = '!';
                    }
                    atm1_tmp[data_idx++] = ch;
                    atm1_tmp[data_idx] = '\0';
                }
            }
        }
    }
}

void task_atm2(void)
{
    static int line_ptr = 0;
    char ch;

    static char sp[2];  // signal for save start : 검출패턴 ):   ---> DATA(1234):
    static char st[2];  // signal for save stop  : 검출패턴 0x0A S--> (줄바꿈)Source

    static int fg_save_data = 0;
    static int data_idx = 0;

    //char tmp_data[600];

    TM_watchATM2 = 0;

    ch = atm2_q_get();

    if (fg_start_sensing == 0)  return;

    if (ch != -1)
    {
        //sensor_status.b.atm2 = 3;
        sensor_status.w |= 3<<(9*2);

        // if ((ch<0x20))
        // {
        //     if (!( (ch==0x09) ||(ch==0x0D)||(ch==0x0A) ))
        //     {
        //         return;
        //     }
        // }
        switch (ch)
        {
            case 0x0D:
            case 0x0A:
            case 0x09:
            case '$':
            case '*':
            case '-':
            case ',':
            case '.':
            case '(':
            case ')':
            case ':':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                break;
            default:
                return;
        }

        // 다음은 순전히 SD에 저장하기 위함이다.---->
        atm2_line[line_ptr++] = ch;
        atm2_line[line_ptr] = '\0';
        if ( (line_ptr >100) || (ch == 0x0A) )
        {
            //save to SD
            if (sdc_read_detectPin()==SDC_INSERTED)
            {
                u32 fsz;
                //PRINTLINE;
                sdc_saveDataToFile(FN_ATM2, atm2_line, &fsz);
                //PRINTVAR(fsz);
                if (fsz > FSZ_MAX)
                {
                    SensorBakSize.b.atm2 = 1;
                }
            }
            line_ptr = 0;
        }
        // <----


        sp[0] = sp[1];  sp[1] = ch;
        st[0] = st[1];  st[1] = ch;
        if ( (sp[0]==')') && (sp[1]==':') )
        {
            // save start
            fg_save_data = 1;
            data_idx = 0;
        }
        else
        {
            if (fg_save_data==1)
            {
                if ( (st[0]==0x0A) && (st[1]=='S') )
                {
                    // stop save
                    // memcpy(atm2_data, atm2_tmp, 600);

// PRINTLINE;
                    debugstring(atm2_tmp);

                    {
                        int i = 0;
                        int j = 0;
                        char ch;
                        while(1)
                        {
                            ch = atm2_tmp[i++];
                            if (ch == '\0') 
                            {
                                atm2_data[j++] = ch;
                                break;
                            }

                            switch(ch)
                            {
                                case '$':
                                case '*':
                                case ')':
                                case 0x0D:
                                    break;
                                case ':':
                                case 0x0A:
                                    atm2_data[j++] = ',';
                                    break;
                                default:
                                    atm2_data[j++] = ch;
                                    break;
                            }
                        }
                        // debugstring(atm2_data);

                    }

                    debugstring("----------atm2_data-----------\r\n");
                    parsing_atm(atm2_data);
                    
                    set_atm2_valid();
                    set_atm2_len(data_idx);
                    fg_save_data = 0;
                    data_idx = 0;
                }
                else
                {
                    //save data to atm2_tmp[data_idx]
                    if ( (ch == 0x0D) || (ch==0x0A) )
                    {
                        //ch = '!';
                    }
                    atm2_tmp[data_idx++] = ch;
                    atm2_tmp[data_idx] = '\0';
                }
            }
        }
    }
}
#endif

