#pragma once


#define C_IRIDIUM_1     3
#define C_IRIDIUM_2     3

#define PORT_WM215		3

//-----------------------------------------------
#ifdef __K_IRIDIUM_C__
//-----------------------------------------------

// int iri_port = C_IRIDIUM_1;
int iri_port = C_IRIDIUM_2;


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

void task_iridium(void);
void task_iri_rcv(void);


int  get_iri1_init(void);
void set_iri1_init(int a_val);

int  get_iri2_init(void);
void set_iri2_init(int a_val);

void iri1_rcv_q_init(void);
void iri2_rcv_q_init(void);

#define iridium_printf(A)   print_string (iri_port, A)


//---- iridium event -----------------------------
#define EVT_IRI_START_MSG		10
#define EVT_IRI_CLIMATE_MSG		20
#define EVT_IRI_E_QUAKE_MSG		30

u32 iri_get_event(void);
void iri_clr_event(void);
void iri_set_event(u32 a_evt);

int iri_rcv_get(int no, char *ch);
u32 get_iri1_rcv_len(void);
u32 get_iri2_rcv_len(void);

int wm_rcv_get(char *ch);
void wm_rcv_q_init(void);

