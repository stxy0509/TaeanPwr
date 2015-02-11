#pragma once



#define	DEFERD_Q_MAX	1500


struct _echoData {
	int gps_valid;
	int depth_valid;
	int temp_valid;

    int lat_d;
    int lat_md;
    int lat_mf;
    int lon_d;
    int lon_md;
    int lon_mf;

    int depth;
    int temp;
};

typedef struct {
    int gps_valid;
    int depth_valid;
    int temp_valid;

    int lat_d;
    int lat_md;
    int lat_mf;
    int lon_d;
    int lon_md;
    int lon_mf;

    int depth;
    int temp;
} echoData_T;


typedef struct  {
    unsigned int lat_d:  7;
    unsigned int lat_md: 6;
    unsigned int lat_mf:14;
    unsigned int lon_d:  8;
    unsigned int lon_md: 6;
    unsigned int lon_mf:14;

    unsigned int depth: 10;
    unsigned int temp:   9;     // 74 bit data

    unsigned int gps_valid:  1;
    unsigned int depth_valid:1;
    unsigned int temp_valid: 1;

    unsigned int dmy:   19;     // 19 bit dummy
} echoData_bin_T;

typedef union {

    struct  {
        unsigned int lat_d:  7;
        unsigned int lat_md: 6;
        unsigned int lat_mf:14;
        unsigned int lon_d:  8;
        unsigned int lon_md: 6;
        unsigned int lon_mf:14;

        unsigned int depth: 10;
        unsigned int temp:   9;     // 74 bit data

        unsigned int gps_valid:  1;
        unsigned int depth_valid:1;
        unsigned int temp_valid: 1;

        unsigned int dmy:   19;     // 19 bit dummy
    } s;

    // struct {
    //     unsigned i;
    // } s;
    unsigned char str[12];
} echo1_T;



//-----------------------------------------------
#ifdef __K_MSG_C__
//-----------------------------------------------

//S,W=707(기상), D1EXX00=130(기상), D1EXX10=1002(지진Normal모드,30분), D1EXX11=202(지진모드,5분)
//char msg_k1[1024];
//char msg_k0[40];


//--------------------------------------
ALIGN4 char s_msg[MSG_LENGTH];

// ALIGN4 char s_msg_c[MSG_LENGTH];  	//(+)131017
// int fgMsgC_ready = 0;				//(+)131017

#if (BUOY_SPEC == 'D')
ALIGN4 char s_msg_e05[MSG_LENGTH];  	//(+)131017
ALIGN4 char s_msg_e30[MSG_LENGTH];  	//(+)131017
// int fgMsgE_ready = 0;				//(+)131017

char time_buf[20]="999999999999";
#endif
ALIGN4 char msg_queue[DEFERD_Q_MAX][MSG_LENGTH];
int q_front;
int q_rear;


//-----------------------------------------------
#else
//-----------------------------------------------

//extern char msg_k1[];
//extern char msg_k0[];


//--------------------------------------
extern ALIGN4 char s_msg[MSG_LENGTH];

// extern ALIGN4 char s_msg_c[MSG_LENGTH];  //(+)131017
// extern int fgMsgC_ready;				//(+)131017

#if (BUOY_SPEC == 'D')
extern ALIGN4 char s_msg_e05[MSG_LENGTH];  //(+)131017
extern ALIGN4 char s_msg_e30[MSG_LENGTH];  //(+)131017
// extern int fgMsgE_ready;				//(+)131017

extern char time_buf[20];
#endif

extern ALIGN4 char msg_queue[DEFERD_Q_MAX][MSG_LENGTH];
extern int q_front;
extern int q_rear;


//-----------------------------------------------
#endif
//-----------------------------------------------

//char *make_msg_k0(void);
char *make_msg_k1(void);
char * make_msg_ky(char t_id, int a_result);
char *make_msg_kx(char a_code);

void make_msg_press_5(void);
void make_msg_press_30(void);

//deferd message queue
void init_q(void);
char q_putData(void);
char q_getData_only(void);
char q_getData(int num);
char q_pop(void);
char is_q_empty(void);
char is_q_full(void);
int is_q_dataNum(void);

void make_msg_second(void);
void init_echoData(void);

void set_secData_display(void);
void set_minData_display(void);



extern float getShtTemp(void);
extern float getShtHumi(void);


//------------------------------------------------------------
// Stack for defered Data
//
//



