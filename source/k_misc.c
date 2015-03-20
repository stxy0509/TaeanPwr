#define __K_MISC_C__

#include "k_includes.h"



void print_string(int a_ch, const char* str)
{
    while (*str)
    {
#ifdef CONFIG_UART_TX_INTERRUPT
        while (1)
        {
            if (uart_putch(a_ch,*str))
                break;
        }
#else
        uart_putch(a_ch,*str);
#endif
        str++;
    }
}

char is_special_ch(char a_ch)
{
    switch (a_ch)
    {
        case 0x09:  //tab
        case 0x0A:  //LF
        case 0x0D:  //CR
            return 0;
            break;
    }

    if ((a_ch < 0x20)||(a_ch > 0x7E)) return 1;
    return 0;
}


/*
void iridium_printf(const char * const fmt, ...)
{
    va_list args;
    int n;
    char buf[128];

    if (fmt)
    {
        va_start(args, fmt);
        n = my_vsprintf(buf, fmt, args);
        va_end(args);
        print_string(iri_port, buf);
    }
}
*/


/*void iridium_printf ( const char *rsp, ... )
{
    short   int len = 0;
    char print_buf[2048];   //[512];
    va_list args;

    if  ( rsp )
    {
        va_start ( args, rsp );
        len = vsnprintf ( print_buf, sizeof ( print_buf ), rsp, args );
        va_end ( args );
    }

    print_string ( iri_port, print_buf );
}
*/





