#pragma once


//#define	SB_S_IRI1	0
//#define	SB_S_IRI2	1
#define	SB_S_AIO	2
#define	SB_S_MOSE	3
#define	SB_S_PTB210	4 	//12
#define SB_S_CHK1	6
#define SB_S_ATM1	7
#define	SB_S_ATM2	8
#define	SB_S_CHK2	9
#define	SB_S_SHOCK	10 	//13
#define	SB_S_GPS	11 	//9
#define	SB_S_CT3919	12	//4
#define	SB_S_DCS	13 	//5
#define	SB_S_DCS1	14
#define	SB_S_HMP155	15 	//11


//-----------------------------------------------
#ifdef __K_SB16C1058_C__
//-----------------------------------------------


//-----------------------------------------------
#else
//-----------------------------------------------

void EIRQ0_ISR(void);
void EIRQ1_ISR(void);

//-----------------------------------------------
#endif
//-----------------------------------------------



// PAGE 0.
#define SB_UART_THR 	0
#define SB_UART_RBR 	0
#define SB_UART_IER 	1
#define SB_UART_FCR 	2
#define SB_UART_ISR 	2
#define SB_UART_LCR 	3
#define SB_UART_MCR 	4
#define SB_UART_LSR 	5
#define SB_UART_MSR 	6
#define SB_UART_SPR 	7

// PAGE 1.
#define SB_UART_DLL 	0
#define SB_UART_DLM 	1
//		SB_UART_FCR 	2
//		SB_UART_ISR 	2
// 		SB_UART_LCR 	3
//		SB_UART_MCR 	4
//		SB_UART_LSR 	5
//		SB_UART_MSR 	6
//		SB_UART_SPR 	7

// PAGE 2.
// 		-- 				0
#define SB_UART_GICR 	1
#define SB_UART_GISR 	2
// 		SB_UART_LCR 	3
//		SB_UART_MCR 	4
#define SB_UART_TCR 	5
#define SB_UART_RCR 	6
#define SB_UART_FSR 	7

// PAGE 3.
#define SB_UART_PSR 	0
#define SB_UART_ATR 	1
#define SB_UART_EFR 	2
// 		SB_UART_LCR 	3
#define SB_UART_XON1 	4
#define SB_UART_XON2 	5
#define SB_UART_XOFF1 	6
#define SB_UART_XOFF2 	7

// PAGE 4.
//		SB_UART_PSR 	0
#define SB_UART_AFR 	1
#define SB_UART_XRCR 	2
// 		SB_UART_LCR 	3
#define SB_UART_TTR 	4
#define SB_UART_RTR 	5
#define SB_UART_FUR 	6
#define SB_UART_FLR 	7



//BOOL sb_uart_config ( int ch, int baud, UART_DATABITS databits, UART_STOPBITS stopbit, UART_PARITY parity );
BOOL sb_uart_config ( int ch, int baud );

void sb_set_register(int ch, u8 reg, u8 data);
u8 sb_get_register(int ch, u8 reg);
BOOL sb_uart_putch ( int n, char a_ch );

BOOL sb_printstring ( int n, U8 *buf );
void sb_printf ( int n, const char *rsp,	... );


void sb_test_tx_out(void);

//-----------------------------------------------
#ifdef __K_SB16C1058_C__
//-----------------------------------------------


#define UART_FIFO_DEPTH_SB 16
#define SB_UART_CHANNEL_MAX 16


#define SB_CHIP_A	0x00	// 00 xxx xxx
#define SB_CHIP_B	0x40	// 01 xxx xxx
//#define SB_CHIP_B	0x80	// 01 xxx xxx

#define SB_UART_0	0x00	// xx 00 0 xxx
#define SB_UART_1	0x08	// xx 00 1 xxx
#define SB_UART_2	0x10	// xx 01 0 xxx
#define SB_UART_3	0x18	// xx 01 1 xxx
#define SB_UART_4	0x20	// xx 10 0 xxx
#define SB_UART_5	0x28	// xx 10 1 xxx
#define SB_UART_6	0x30	// xx 11 0 xxx
#define SB_UART_7	0x38	// xx 11 1 xxx


#define THR 	0
#define RBR 	0
#define IER 	1
#define FCR 	2
#define ISR 	2
#define LCR 	3
#define MCR 	4
#define LSR 	5
#define MSR 	6
#define SPR 	7

#define DLL 	0
#define DLM 	1



#define SB_PIN_RESET_H()	(*R_GPOHIGH(1) |= (1<<6) )
#define SB_PIN_RESET_L()	(*R_GPOLOW(1) |= (1<<6) )

#define SB_PIN_RESET0_H()	(*R_GPOHIGH(1) |= (1<<6) )
#define SB_PIN_RESET0_L()	(*R_GPOLOW(1) |= (1<<6) )

#define SB_PIN_RESET1_H()	(*R_GPOHIGH(1) |= (1<<5) )
#define SB_PIN_RESET1_L()	(*R_GPOLOW(1) |= (1<<5) )

#define SB_PIN_IOR_H()		(*R_GPOHIGH(5) |= (1<<7) )
#define SB_PIN_IOR_L()		(*R_GPOLOW(5)  |= (1<<7) )

#define SB_PIN_IOW_H()		(*R_GPOHIGH(5) |= (1<<6) )
#define SB_PIN_IOW_L()		(*R_GPOLOW(5)  |= (1<<6) )

#define SB_PIN_CS_H()		(*R_GPOHIGH(5) |= (1<<3) )
#define SB_PIN_CS_L()		(*R_GPOLOW(5)  |= (1<<3) )





void sb_set_baudrate(int ch, u32 a_baud);


//-----------------------------------------------
#else
//-----------------------------------------------


void SB_init(void);
//void SB_setSPR(u8 val1);
//u32  SB_getSPR(void);
//void SB_setUartCh(u8 a_ch);
//void SB_setBaudrate(u32 a_baud);

BOOL sb_uart_putch ( int n, char ch );
void sb_test_readAll(int n);

//-----------------------------------------------
#endif
//-----------------------------------------------
