#pragma once

// constant define ------------------------------------------------------------
#define Q_PTB_MAX  1000



// typedef --------------------------------------------------------------------

typedef struct _ptb_data
{
    u8 valid;
    int n;
    double pressure;
    double pressure_sum;
    double pressure_a;
} ptb_data_T;





// function prototype ---------------------------------------------------------


void ptb_q_put(char ch);
char ptb_q_get(void);
void ptb_q_init(void);
void init_ptb210_data(void);
void task_ptb210(void );


void set_ptb210_valid(u8 a_val);
u8 is_ptb210_valid(void);
double get_ptb210_press(void);


// ----------------------------------------------------------------------------
#ifdef __K_PTB210_C__
// ----------------------------------------------------------------------------

U32 tmr_PTB_data_display = 0;

// ----------------------------------------------------------------------------
#else
// ----------------------------------------------------------------------------

extern ptb_data_T ptb;

extern t_SENSOR_Q ptb_q;
extern ALIGN4 char ptb_q_buf[];
extern ALIGN4 char ptb_line[];

extern U32 tmr_PTB_data_display;

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------



