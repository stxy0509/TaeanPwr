#pragma once


#define Q_CT_MAX  1000

#define PORT_ALTIMETER		2


// typedef --------------------------------------------------------------------
typedef struct _ct_data
{
    u8 valid;
    int cond;
    int temp;
} ct_data_T;



// function prototype ---------------------------------------------------------
void task_ct3919(void);
char is_special_ch(char a_ch);

void init_ct_data(void);

void ct_q_init(void);
void ct_q_put(char ch);
int is_ct_valid(void);
void set_ct_valid(int a_val);
int get_ct_cond(void);
int get_ct_temp(void);


//-----------------------------------------------
#ifdef __K_CT3919_C__
//-----------------------------------------------

//-----------------------------------------------
#else
//-----------------------------------------------
extern ct_data_T ct;

extern t_SENSOR_Q ct_q;
extern ALIGN4 char ct_q_buf[];
extern ALIGN4 char ct_line[];


//-----------------------------------------------
#endif
//-----------------------------------------------



