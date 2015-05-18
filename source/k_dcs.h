#pragma once

// constant define ------------------------------------------------------------
#define Q_DCS_MAX  1024


// typedef --------------------------------------------------------------------

typedef struct _dcs_data
{
    u8 valid;
    double dir;
    double speed;
    double pitch;
    double roll;
} dcs_data_T;






// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#ifdef __K_DCS_C__
#else
// extern ---------------------------------------------------------------------

extern dcs_data_T dcs;

extern t_SENSOR_Q dcs_q;
extern ALIGN4 char dcs_q_buf[];
extern ALIGN4 char dcs_line[];


#endif


// function prototype ---------------------------------------------------------

void dcs_q_put(char ch);
char dcs_q_get(void);
void dcs_q_init(void);

void init_dcs_data(void);
void parsing_dcs(char *a_str);
void task_dcs(void);

int is_dcs_valid(void);       
void set_dcs_valid(int a_val);
double get_dcs_dir(void);     
double get_dcs_speed(void);   

char * get_4050_temp(void);		// (+)150518

