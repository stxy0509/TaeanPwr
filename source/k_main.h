#pragma once

#ifndef __K_MAIN_H__
#define __K_MAIN_H__

#define RTC_chkMfpPort()     ((*R_GP1ILEV) & 0x80)



typedef struct _SENSOR_Q {
    //char *buf;
    U32 front;
    U32 rear;
    U32 len;
} t_SENSOR_Q;

typedef struct _SENSOR_STK {
    //char *buf;
    int top;
    // U32 rear;
    // U32 len;
} SENSOR_STK_t;


// #define FSZ_MAX     (1024*3500) //3.5Mbyte
#define FSZ_MAX     (1024*10000) //10 Mbyte     //131019
#define FSZ_MAX_15M (1024*15000) //15 Mbyte     //141018    for MOSE
#define FSZ_MAX_25M (1024*25000) //25 Mbyte     //141108    for MOSE

typedef union
{
    struct
    {
        u32 _reserved0:19;              /*!< bit:  0..18  Reserved                           */
        u32 shock:1;                    /*!< bit:     19  Negative condition code flag       */
        u32 send:1;                     /*!< bit:     20  Negative condition code flag       */
        u32 atm1:1;                     /*!< bit:     21  Negative condition code flag       */
        u32 atm2:1;                     /*!< bit:     22  Negative condition code flag       */
        u32 history:1;                  /*!< bit:     23  Thumb bit        (read 0)          */
        u32 ptb210:1;                   /*!< bit:     24  Thumb bit        (read 0)          */
        u32 wave:1;                     /*!< bit:     25  Thumb bit        (read 0)          */
        u32 dcs:1;                      /*!< bit:     26  Saturation condition flag          */
        u32 ct3919:1;                   /*!< bit:     27  Overflow condition code flag       */
        u32 hmp155:1;                   /*!< bit:     28  Overflow condition code flag       */
        u32 gps:1;                      /*!< bit:     29  Carry condition code flag          */
        u32 aio:1;                      /*!< bit:     30  Zero condition code flag           */
        u32 mose:1;                     /*!< bit:     31  Negative condition code flag       */
    } b;                              /*!< Structure used for bit  access                  */
    u32 w;                            /*!< Type      used for word access                  */
} SensorBakSize_Type;


typedef struct
{
    u16     init_flag;
    char      s0;
    char      s1;
    char      s2;
    char      s3;
    char      s4;
    char      s5;
    char      s6;
    char      s7;
    char      s8;
    char      s9;
    char      sa;
    char      sb;
    char      sc;
    char      sd;
    char      se;
    char      sf;
    char      sr;
    u8      interval;
    double  ref_lat;
    double  ref_lon;
    u8      ref_flag;
    u8      ref_distance;
    u16     ref_shock;
    u8      mode;
} env_data_T;




//-----------------------------------------------
#ifdef __K_MAIN_C__
//-----------------------------------------------
// k_main.c file

// variables

u32 main_idx = 0;
ALIGN4 char newMsg[MSG_LENGTH];
SensorBakSize_Type SensorBakSize;

int sb1_high_cnt=0;
int sb2_high_cnt=0;
int fg_sb_high_10ms=0;

// Buoy Info.
int buoyNo = 1;   //1:S88

int logout_port = 0;

int fg_start_sensing = 0;

SensorStatus_T sensor_status;
env_data_T env;

// functions
void boardinit();

void sysWDT_init(void);
void sysWDT_CntClear(void);


//-----------------------------------------------
#else
//-----------------------------------------------

//extern
extern ALIGN4 char newMsg[];
extern SensorBakSize_Type SensorBakSize;

extern int sb1_high_cnt;
extern int sb2_high_cnt;
extern int fg_sb_high_10ms;

extern int buoyNo;
extern int logout_port;

extern int sb1_ok_cnt;
extern int sb2_ok_cnt;

extern int fg_start_sensing;

extern SensorStatus_T sensor_status;
extern env_data_T env;

//-----------------------------------------------
#endif
//-----------------------------------------------




//-----------------------------------------------
#endif  //#ifndef __K_INCLUDES_H__


void sensor_q_init(void);
int get_battery_level(void);


void env_init(void);
void env_print(void);
void env_read(void);
void env_save(void);

void env_set_pos(double a_lat, double a_lon);
void env_set_distance(int a_dist);
void env_set_interval(int a_dist);
void env_set_shock(int a_shock);
void env_set_sensor(int a_sid, int a_val);
void env_set_mode(int a_mode);

void setEnableHeartbeat(int a_val);

int  get_tm_sbdreg(void);
void set_tm_sbdreg(int a_val);

void wifi_reset(void);

