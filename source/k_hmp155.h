#pragma once


#define Q_HMP155_MAX  512


// typedef --------------------------------------------------------------------
typedef struct _hmp155_data
{
    int valid;
    int n;
    double temp;
    double temp_sum;
    double temp_a;
    //double humi;
    //double humi_sum;
    //double humi_a;
} hmp155_data_T;



// function prototype ---------------------------------------------------------
void task_hmp155(void);
//char is_special_ch(char a_ch);

void init_hmp155_data(void);

void hmp155_q_init(void);
void hmp155_q_put(char ch);

void set_hmp155_valid(u8 a_val);
u8 is_hmp155_valid(void);       
double get_hmp155_temp(void);   
//double get_hmp155_humi(void);   





//-----------------------------------------------
#ifdef __K_HMP155_C__
//-----------------------------------------------

u32 tmr_HMP_data_display = 0;


//-----------------------------------------------
#else
//-----------------------------------------------
extern hmp155_data_T hmp155;

extern t_SENSOR_Q hmp155_q;
extern ALIGN4 char hmp155_q_buf[];
extern ALIGN4 char hmp155_line[];

extern u32 tmr_HMP_data_display;


//-----------------------------------------------
#endif
//-----------------------------------------------



