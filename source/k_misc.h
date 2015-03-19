#pragma once


#define UartPutCh	uart_putch 

#define OK		1
#define YES		1
#define NG		0

#define UART_DEBUG		0
#define UART_SYS_MCU	1




//-----------------------------------------------
//-----------------------------------------------
#ifdef __K_MISC_C__
//-----------------------------------------------


//-----------------------------------------------
#else
//-----------------------------------------------


//-----------------------------------------------
#endif
//-----------------------------------------------

void print_string(int a_ch, const char* str);
char is_special_ch(char a_ch);

//void iridium_printf(const char * const fmt, ...);


