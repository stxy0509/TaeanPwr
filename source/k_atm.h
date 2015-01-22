#pragma once

// constant define ------------------------------------------------------------
#define Q_ATM_MAX  1024


// typedef --------------------------------------------------------------------

typedef struct //_atm_data
{
    u8 valid;
    // char date[] dir;
    // double speed;
    // double pitch;
    // double roll;
} atm_data_T;


typedef struct 
{
    char	date[13];
    int 	sno;
    int 	head;
    int		pitch;
    int		roll;
    int 	temp;
    int 	cond;
    int 	bat;
    u32 	press[20];
} deepPressure_T;

#if (BUOY_SPEC == 'D')
void dp_stk_init(void);
int dp_stk_push(deepPressure_T t);
int dp_stk_pop(deepPressure_T *t);
void dp_stk_print(void);
int is_dp_stk_empty(void);
#endif


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#ifdef __K_ATM_C__
#else
// extern ---------------------------------------------------------------------

extern atm_data_T atm1;
extern atm_data_T atm2;

extern t_SENSOR_Q atm1_q;
extern t_SENSOR_Q atm2_q;
extern ALIGN4 char atm1_q_buf[];
extern ALIGN4 char atm2_q_buf[];
extern ALIGN4 char atm1_line[];
extern ALIGN4 char atm2_line[];
extern ALIGN4 char atm1_data[];
extern ALIGN4 char atm2_data[];


#endif


// function prototype ---------------------------------------------------------

void atm1_q_put(char ch);
char atm1_q_get(void);
void atm1_q_init(void);

void init_atm1_data(void);
void parsing_atm1(char *a_str);
void task_atm1(void);


void atm2_q_put(char ch);
char atm2_q_get(void);
void atm2_q_init(void);

void init_atm2_data(void);
void parsing_atm2(char *a_str);
void task_atm2(void);


void clr_atm1_valid(void);
int is_atm1_valid(void);
int  get_atm1_len(void);

void clr_atm2_valid(void);
int is_atm2_valid(void);
int  get_atm2_len(void);

