#pragma once


#define Q_AIO_MAX  512


// typedef --------------------------------------------------------------------
typedef struct _aio_data
{
    u8 valid;
    int    n;
    int    n_ws;
    int    n_wd;
    int    n_temp;
    int    n_humi;

    double ws;
    double ws_sum;
    double ws_a;
    double w_x;
    double w_y;
    double w_ax;
    double w_ay;
    double w_sx;
    double w_sy;
    double w_vd;
    //double w_vy;
    double wd;
    double temp;
    double temp_sum;
    double temp_a;
    double humi;
    double humi_sum;
    double humi_a;

    int    n_press;         //131012
    int    valid_press;     //131012
    double press;
    double press_sum;
    double press_a;
} aio_data_T;



// function prototype ---------------------------------------------------------
void task_aio(void);
//char is_special_ch(char a_ch);

void init_aio_data(void);

void aio_q_init(void);
void aio_q_put(char ch);

u8 is_aio_valid(void);
u8 is_aio_valid_press(void);
void set_aio_valid(u8 a_val);
double get_aio_ws(void);
double get_aio_wd(void);
double get_aio_temp(void);
double get_aio_humi(void);
double get_aio_press(void);








//-----------------------------------------------
#ifdef __K_AIO_C__
//-----------------------------------------------
u32 tmr_AIO_data_display = 0;

//-----------------------------------------------
#else
//-----------------------------------------------
extern aio_data_T aio;

extern t_SENSOR_Q aio_q;
extern ALIGN4 char aio_q_buf[];
extern ALIGN4 char aio_line[];

extern u32 tmr_AIO_data_display;


//-----------------------------------------------
#endif
//-----------------------------------------------



