#pragma once

#include "k_config.h"
#ifdef USE_WM800







typedef enum {
    nATZ=0,
    nATE0,
    nAT_NOTIFY,
    nAT_SIOFLOW,
    nAT_KEYE,
    nAT_CRM,
    nATDT1501,
    nAT_TCPOPEN,
    nAT_TCPWRITE,
    nAT_TCPCLOSE,
    nAT_TCPEXIT,
} WM800CMDs;


//-----------------------------------------------
#ifdef __K_IRIDIUM_C__
//-----------------------------------------------

//int iri_port = C_IRIDIUM_1;
// int iri_port = C_IRIDIUM_2;


//-----------------------------------------------
#else
//-----------------------------------------------

extern int iri_current_state;
extern int iri_port;

//-----------------------------------------------
#endif
//-----------------------------------------------




int iridium_Process_1(int a_option);
// void iridium_powerON(void);
// void iridium_powerOFF(void);
u8 wm800_process_init(u8 a_option);
u8 cdma_process_send(u8 a_option);
u8 cdma_process_sms(u8 a_option);

void task_wm800(void);
void task_wm800_rcv(void);
void task_cdma_sms(void);

void set_sms_arrived(void);

int  get_iri1_init(void);
void set_iri1_init(int a_val);

// int  get_iri2_init(void);
// void set_iri2_init(int a_val);

void iri1_rcv_q_init(void);
// void iri2_rcv_q_init(void);

#define iridium_printf(A)   print_string (COMM_CHANNEL, A)
#define modem_printf(A)     print_string (COMM_CHANNEL, A)



//---- iridium event -----------------------------
#define EVT_IRI_START_MSG       10
#define EVT_IRI_CLIMATE_MSG     20
#define EVT_IRI_E_QUAKE_MSG     30

u32 iri_get_event(void);
void iri_clr_event(void);
void iri_set_event(u32 a_evt);

// int iri_rcv_get(int no, char *ch);
u32 get_iri1_rcv_len(void);
// u32 get_iri2_rcv_len(void);

void ctrl_sensor(char a_id, char a_code);

#endif  //ifdef USE_IRIDIUM

