#pragma once

// constant define ------------------------------------------------------------
#define Q_SHOCK_MAX  1000



// typedef --------------------------------------------------------------------

typedef struct _shock_data
{
    int valid;
    double temp;
    double humi;
    int shk;
} shock_data_T;





// function prototype ---------------------------------------------------------


void shock_q_put(char ch);
char shock_q_get(void);
void shock_q_init(void);
void init_shock_data(void);
void task_shock(void );


int  get_shock_valid(void);
void set_shock_valid(u8 a_val);
int get_shock_shk(void);
// double get_shock_temp(void);
// double get_shock_humi(void);

void dec_shk_interval(void);


// ----------------------------------------------------------------------------
#ifdef __K_SHOCK_C__
// ----------------------------------------------------------------------------

U32 tmr_SHOCK_data_display = 0;

// ----------------------------------------------------------------------------
#else
// ----------------------------------------------------------------------------

extern shock_data_T shock;

extern t_SENSOR_Q shock_q;
extern ALIGN4 char shock_q_buf[];
extern ALIGN4 char shock_line[];

extern U32 tmr_SHOCK_data_display;

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------



