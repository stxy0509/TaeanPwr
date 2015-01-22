#pragma once

//#define Q_MAX   (512+1)
//#define Q_MAX   (2048+1)

typedef struct _MOSE_DATA {
    float x;
    float y;
    float z;
} mose_data_T;



void q_init(void);
void q_clear(void);
void q_put(mose_data_T k);
int  q_is_empty(void);
mose_data_T q_get(void);

int q_start_position(void);
mose_data_T q_read(int t_front);

double get_mose_lat(void);
double get_mose_lon(void);
int get_mose_pos_valid(void);
void set_mose_pos_valid(int a_flag);

float get_mose_Hmax(void);
float get_mose_Tmax(void);
float get_mose_H3(void);
float get_mose_t3(void);
float get_mose_pwd(void);

float get_moseL_Hmax(void);
float get_moseL_Tmax(void);
float get_moseL_H3(void);
float get_moseL_t3(void);
float get_moseL_pwd(void);

int get_mose_data_valid(void);
void set_mose_data_valid(int a_val);


// constant define ------------------------------------------------------------
#define Q_MOSE_MAX  (2048*4)


// typedef --------------------------------------------------------------------

// typedef struct _mose_data
// {
//     u8 valid;
//     double dir;
//     double speed;
//     double pitch;
//     double roll;
// } mose_data_T;


typedef struct _MOSE_OUTDATA {
    U8 year;
    U8 mon;
    U8 day;
    U8 hour;
    U8 min;
    U8 sec[6];
    double hmax;
    double tmax;
    double h3;
    double t3;
    double pwd;
} _MOSE_OUTDATA_T;




//-----------------------------------------------
#ifdef __K_MOSE_C__
//-----------------------------------------------

U8 evt_calc_wave = 0;
U32 tmr_MOSE_data_display = 0;

//-----------------------------------------------
#else
//-----------------------------------------------
// extern

extern mose_data_T mose;

extern t_SENSOR_Q mose_q;
extern ALIGN4 char mose_q_buf[];
extern ALIGN4 char mose_line[];

extern U8 evt_calc_wave;
extern U32 tmr_MOSE_data_display;


//-----------------------------------------------
#endif
//-----------------------------------------------


// function prototype

void mose_q_put(char ch);
char mose_q_get(void);
void mose_q_init(void);
u32 get_mose_q_len(void);

void init_mose_data(void);
//void parsing_mose(char *a_str);
int parse_input_string(void);

void task_mose(void);

void task_calc(void);
void task_send_msg(void);

void q_init_HF_mbuf(void);
void q_init_LF_mbuf(void);
