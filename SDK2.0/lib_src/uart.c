/******************************************************************************
 Copyright (C) 2012      Advanced Digital Chips Inc.
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#include "adstar.h"

#ifdef CONFIG_UART_RX_INTERRUPT
//#warning "UART RX interrupt mode Build"
#endif
#ifdef CONFIG_UART_TX_INTERRUPT
//#warning "UART TX interrupt mode Build"
#endif

typedef struct _UART_RING_BUF
{

#ifdef CONFIG_UART_RX_INTERRUPT
	U8    *RingBufRxInPtr;  // Pointer to where next character will be inserted
	U8    *RingBufRxOutPtr; // Pointer from where next character will be extracted
	U8    RingBufRx[UART_BUF_SIZE]; // Ring buffer character storage (Rx)
	U16   RingBufRxCtr;     // Number of characters in the Rx ring buffer
#endif

#ifdef CONFIG_UART_TX_INTERRUPT
	U8    *RingBufTxInPtr;    // Pointer to where next character will be inserted
	U8    *RingBufTxOutPtr; // Pointer from where next character will be extracted
	U8    RingBufTx[UART_BUF_SIZE];  // Ring buffer character storage (Tx)
	U16   RingBufTxCtr;     // Number of characters in the Tx ring buffer
#endif
} UART_RING_BUF;

static UART_RING_BUF *ubuf[UART_CHANNEL_MAX] = {0,};

static void InitRingBuf ( int ch )
{
	if ( ubuf[ch] == 0 )
	{
		ubuf[ch] = malloc ( sizeof ( UART_RING_BUF ) );
	}

	UART_RING_BUF *pRing = ubuf[ch];
#ifdef CONFIG_UART_RX_INTERRUPT
	pRing->RingBufRxCtr    = 0;
	pRing->RingBufRxInPtr  = &pRing->RingBufRx[0];
	pRing->RingBufRxOutPtr = &pRing->RingBufRx[0];
#endif
#ifdef CONFIG_UART_TX_INTERRUPT
	pRing->RingBufTxCtr    = 0;
	pRing->RingBufTxInPtr  = &pRing->RingBufTx[0];
	pRing->RingBufTxOutPtr = &pRing->RingBufTx[0];
#endif
}

static __inline__ U32 getbaseaddress ( int ch )
{
	return ( U32 ) R_UART_BASE ( ch );
}

static inline UART_RING_BUF *getringbuf ( int ch )
{
	return ubuf[ch];
}

void uart_rx_int_enable ( int ch, BOOL b )
{
	U32	uart_base = getbaseaddress ( ch );

	if ( b )
		* ( U32 * ) ( uart_base + UART_IER ) |= F_UIE_RDAIEN;
	else
		* ( U32 * ) ( uart_base + UART_IER ) &= ~F_UIE_RDAIEN;
}

void uart_tx_int_enable ( int ch, BOOL b )
{
	U32	uart_base = getbaseaddress ( ch );

	if ( b )
		* ( U32 * ) ( uart_base + UART_IER ) |= F_UIE_THEIEN;
	else
		* ( U32 * ) ( uart_base + UART_IER ) &= ~F_UIE_THEIEN;
}
void uart_status_int_enable ( int ch, BOOL b )
{
	U32	uart_base = getbaseaddress ( ch );

	if ( b )
		* ( U32 * ) ( uart_base + UART_IER ) |= F_UIE_RLSIEN;
	else
		* ( U32 * ) ( uart_base + UART_IER ) &= ~F_UIE_RLSIEN;
}


static void getuartfifo ( int channel, UART_RING_BUF *pbuf )
{
	U32 iir;
	char ch;
	U32 uart_base = getbaseaddress(channel);
	iir = * ( volatile U32 * ) ( uart_base + UART_IIR );
	iir &= 0xf;

	switch ( iir )
	{
#ifdef CONFIG_UART_TX_INTERRUPT

	case 2:
		if ( pbuf->RingBufTxCtr > 0 )
		{
			int i;
			int txmax = pbuf->RingBufTxCtr > UART_FIFO_DEPTH ? UART_FIFO_DEPTH : pbuf->RingBufTxCtr;

			for ( i = 0; i < txmax; i++ )
			{
				* ( volatile U8 * ) ( uart_base + UART_THR ) = *pbuf->RingBufTxOutPtr;
				pbuf->RingBufTxOutPtr++;

				if ( pbuf->RingBufTxOutPtr == &pbuf->RingBufTx[UART_BUF_SIZE] )
				{
					pbuf->RingBufTxOutPtr = &pbuf->RingBufTx[0];
				}
			}

			pbuf->RingBufTxCtr -= i;
		}

		if ( pbuf->RingBufTxCtr == 0 )
			uart_tx_int_enable ( channel, FALSE );

		break;
#endif
#ifdef CONFIG_UART_RX_INTERRUPT

	case 6:
		debugstring ( "UART Line Status Error(Overrun,Frame,Parity)\r\n" );

	case 4:
	case 0xc:
		while ( ( * ( volatile U32 * ) ( uart_base + UART_LSR ) ) & F_ULS_DRDY ) //data ready
		{
			ch = * ( volatile U8 * ) ( uart_base + UART_RBR );

			if ( pbuf->RingBufRxCtr < UART_BUF_SIZE )
			{
				pbuf->RingBufRxCtr++;
				*pbuf->RingBufRxInPtr = ch;
				pbuf->RingBufRxInPtr++;

				/* Wrap OUT pointer     */
				if ( pbuf->RingBufRxInPtr == &pbuf->RingBufRx[UART_BUF_SIZE] )
				{
					pbuf->RingBufRxInPtr = &pbuf->RingBufRx[0];
				}
			}
		}

		break;
#endif
	}
}

void Uart0ISR()
{
	getuartfifo ( 0, ubuf[0] );
}

void Uart1ISR()
{
	getuartfifo ( 1, ubuf[1] );
}

void Uart2ISR()
{
	getuartfifo ( 2, ubuf[2] );
}

void Uart3ISR()
{
	getuartfifo ( 3, ubuf[3] );
}

void Uart4ISR()
{
	getuartfifo ( 4, ubuf[4] );
}

BOOL uart_config ( int ch, int baud, UART_DATABITS databits, UART_STOPBITS stopbit, UART_PARITY parity )
{
	U32 apbclock;
	volatile U32 uart_base;

	if ( ch >= UART_CHANNEL_MAX )
		return FALSE;

	uart_base = getbaseaddress ( ch );
	InitRingBuf ( ch );
	apbclock = get_apb_clock();
	U16 divisor = apbclock / ( baud * 16 ) ;

	if ( ( apbclock % ( baud * 16 ) ) > ( ( baud * 16 ) / 2 ) )
		divisor++;

	U32 lcr;//line control register
	lcr = ( ( stopbit - 1 ) << 2 ) + ( databits - 5 );

	switch ( parity )
	{
	case UART_PARODD:
		lcr |= F_ULC_PEN;
		break;

	case UART_PAREVEN:
		lcr |= F_ULC_PEN | F_ULC_EPS;
		break;

	case UART_PARNONE:
	default:
		break;
	}

	* ( volatile U32 * ) ( uart_base + UART_FCR ) = F_UFC_FIFOEN | F_UFC_RFTL_1;
	lcr |= F_ULC_DLAB;
	* ( volatile U32 * ) ( uart_base + UART_LCR ) = lcr;
	* ( volatile U32 * ) ( uart_base + UART_DIV_LO ) = divisor & 0xff;
	* ( volatile U32 * ) ( uart_base + UART_DIV_HI ) = ( divisor >> 8 ) & 0xff;
	lcr &= ~F_ULC_DLAB; //divisor latch access disable, fifo control write mode.
	* ( volatile U32 * ) ( uart_base + UART_LCR ) = lcr;
	* ( volatile U32 * ) ( uart_base + UART_IER ) = 0; //disable rx/tx interrupts
	uart_rx_flush ( ch );
	uart_tx_flush ( ch );
#if defined(CONFIG_UART_RX_INTERRUPT)||defined(CONFIG_UART_TX_INTERRUPT)
	switch ( ch )
	{
	case 0:
		*R_IINTMOD ( INTNUM_UART0 / 32 ) &= ~ ( 1 << ( INTNUM_UART0 - ( 32 * ( INTNUM_UART0 / 32 ) ) ) ); //for the first empty interrupt
		set_interrupt ( INTNUM_UART0, Uart0ISR );
		enable_interrupt ( INTNUM_UART0, TRUE );
		break;

	case 1:
		*R_IINTMOD ( INTNUM_UART1 / 32 ) &= ~ ( 1 << ( INTNUM_UART1 - ( 32 * ( INTNUM_UART1 / 32 ) ) ) );
		set_interrupt ( INTNUM_UART1, Uart1ISR );
		enable_interrupt ( INTNUM_UART1, TRUE );
		break;

	case 2:
		*R_IINTMOD ( INTNUM_UART2 / 32 ) &= ~ ( 1 << ( INTNUM_UART2 - ( 32 * ( INTNUM_UART2 / 32 ) ) ) );
		set_interrupt ( INTNUM_UART2, Uart2ISR );
		enable_interrupt ( INTNUM_UART2, TRUE );
		break;

	case 3:
		*R_IINTMOD ( INTNUM_UART3 / 32 ) &= ~ ( 1 << ( INTNUM_UART3 - ( 32 * ( INTNUM_UART3 / 32 ) ) ) );
		set_interrupt ( INTNUM_UART3, Uart3ISR );
		enable_interrupt ( INTNUM_UART3, TRUE );
		break;	
	
	case 4:
		*R_IINTMOD ( INTNUM_UART4 / 32 ) &= ~ ( 1 << ( INTNUM_UART4 - ( 32 * ( INTNUM_UART4 / 32 ) ) ) );
		set_interrupt ( INTNUM_UART4, Uart4ISR );
		enable_interrupt ( INTNUM_UART4, TRUE );
		break;
	}

#endif
#ifdef CONFIG_UART_RX_INTERRUPT
	uart_rx_int_enable ( ch, TRUE );
#endif
	return TRUE;
}

void uart_rx_flush ( int ch )
{
	U32 uart_base = getbaseaddress ( ch );
	CRITICAL_ENTER();
	* ( volatile U32 * ) ( uart_base + UART_FCR ) |= ( 1 << 1 ); //rx fifo reset
	CRITICAL_EXIT();
}
void uart_tx_flush ( int ch )
{
	U32 uart_base = getbaseaddress ( ch );
	CRITICAL_ENTER();
	* ( volatile U32 * ) ( uart_base + UART_FCR ) |= ( 1 << 2 ); //tx fifo reset
	CRITICAL_EXIT();
}

BOOL uart_getch ( int n, char *ch )
{
#ifdef CONFIG_UART_RX_INTERRUPT
	UART_RING_BUF *pbuf = getringbuf ( n );

	if ( pbuf == 0 )
		return FALSE;

	CRITICAL_ENTER();

	if ( pbuf->RingBufRxCtr > 0 ) /* No, decrement character count */
	{
		pbuf->RingBufRxCtr--; /* No, decrement character count */
		*ch = *pbuf->RingBufRxOutPtr++; /* Get character from buffer */

		/* Wrap OUT pointer     */
		if ( pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[UART_BUF_SIZE] )
		{
			pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
		}

		CRITICAL_EXIT();
		return TRUE;
	}
	else
	{
		CRITICAL_EXIT();
		return FALSE;
	}

#else
	U32 uart_base = getbaseaddress ( n );

	if ( uart_base == 0 )
		return FALSE;

	U32 lsr = * ( U32 * ) ( uart_base + UART_LSR );

	if ( lsr & F_ULS_DRDY )
	{
		*ch = * ( u8 * ) ( uart_base + UART_RBR );
		return TRUE;
	}
	else
		return FALSE;

#endif
}
int uart_getdata ( int n, U8 *buf, int bufmax )
{
	int i;
#ifdef CONFIG_UART_RX_INTERRUPT
	UART_RING_BUF *pbuf = getringbuf ( n );

	if ( pbuf == 0 )
		return FALSE;


	for ( i = 0; i < bufmax; i++ )
	{
		if ( pbuf->RingBufRxCtr > 0 ) /* No, decrement character count */
		{
			CRITICAL_ENTER();
			pbuf->RingBufRxCtr--; /* No, decrement character count */
			CRITICAL_EXIT();
			buf[i] = *pbuf->RingBufRxOutPtr++; /* Get character from buffer */

			/* Wrap OUT pointer     */
			if ( pbuf->RingBufRxOutPtr == &pbuf->RingBufRx[UART_BUF_SIZE] )
			{
				pbuf->RingBufRxOutPtr = &pbuf->RingBufRx[0];
			}
		}
		else
		{
			break;
		}
	}

#else
	U32 uart_base = getbaseaddress ( n );

	if ( uart_base == 0 )
		return FALSE;

	for ( i = 0; i < bufmax; i++ )
	{
		U32 lsr = * ( U32 * ) ( uart_base + UART_LSR );

		if ( lsr & F_ULS_DRDY )
		{
			buf[i] = * ( u8 * ) ( uart_base + UART_RBR );
		}
		else
			break;
	}

#endif
	return i;
}
BOOL uart_putch ( int n, char ch )
{
	U32 uart_base = getbaseaddress ( n );

	if ( uart_base == 0 )
		return FALSE;

#ifdef CONFIG_UART_TX_INTERRUPT
	UART_RING_BUF *pbuf = getringbuf ( n );

	if ( pbuf == 0 )
		return FALSE;


	if ( pbuf->RingBufTxCtr < UART_BUF_SIZE )
	{
		CRITICAL_ENTER();
		pbuf->RingBufTxCtr++;    /* No, increment character count*/
		CRITICAL_EXIT();
		*pbuf->RingBufTxInPtr = ch;    /* Put character into buffer*/
		pbuf->RingBufTxInPtr++;

		if ( pbuf->RingBufTxInPtr == &pbuf->RingBufTx[UART_BUF_SIZE] )
		{
			pbuf->RingBufTxInPtr = &pbuf->RingBufTx[0];
		}

		if ( ( * ( U32 * ) ( uart_base + UART_IER ) & F_UIE_THEIEN ) == 0 )
		{
			uart_tx_int_enable ( n, TRUE );
		}

		return TRUE;
	}
	else
	{
		return FALSE;
	}

#else

	//wait empty
	while ( ! ( ( * ( volatile U32 * ) ( uart_base + UART_LSR ) )  & F_ULS_TEMP ) );

	* ( volatile u8 * ) ( uart_base + UART_THR ) = ch;
	return TRUE;
#endif
}

BOOL uart_putdata ( int n, U8 *buf, int len )
{
	int i;

	for ( i = 0; i < len; i++ )
		uart_putch ( n, buf[i] );

	return TRUE;
}
BOOL uart_putstring ( int n, U8 *buf )
{
	while ( *buf )
	{
		uart_putch ( n, *buf );
		buf++;
	}

	return TRUE;
}

BOOL uart_putstring ( int n, U8 *buf );

static int debugch = DEBUG_CHANNEL;
void set_debug_channel ( int ch )
{
	if ( ch > 7 )
		return;

	debugch = ch;
}
int get_debug_channel()
{
	return debugch;
}

static int ch4_switch = 0;
void set_ch4_switch(int a_onoff)
{
	ch4_switch = a_onoff;
}
int get_ch4_switch(void)
{
	return ch4_switch;
}

void debugstring ( const char *str )
{
	while ( *str )
	{
#ifdef CONFIG_UART_TX_INTERRUPT

		while ( 1 )
		{
			if ( uart_putch ( debugch, *str ) )
				break;
		}

#else
		uart_putch ( debugch, *str );
		if (ch4_switch==1)
		{
			uart_putch ( 4, *str );
		}
#endif
		str++;
	}
}
void r_debugprintf( const char* filename, int linenum,const char *rsp,	... )
{
	short	int len = 0;
	char print_buf[512];
	va_list args;

	sprintf(print_buf,"%s:%d,",filename,linenum);
	debugstring(print_buf);
	if	( rsp )
	{
		va_start ( args, rsp );
		len = vsnprintf ( print_buf, sizeof ( print_buf ), rsp, args );
		va_end ( args );
	}
	debugstring(print_buf);
}

void debugprintf ( const char *rsp,	... )
{
	short	int len = 0;
	char print_buf[512];
	va_list args;

	if	( rsp )
	{
		va_start ( args, rsp );
		len = vsnprintf ( print_buf, sizeof ( print_buf ), rsp, args );
		va_end ( args );
	}

	debugstring ( print_buf );
}

