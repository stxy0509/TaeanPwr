#define __K_MISC_C__

#include "k_includes.h"



void print_string(int a_ch, const char* str)
{
    while (*str)
    {
#ifdef CONFIG_UART_TX_INTERRUPT
        while (1)
        {
            if (UartPutCh(a_ch,*str))
                break;
        }
#else
        UartPutCh(a_ch,*str);
#endif
        str++;
    }
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





