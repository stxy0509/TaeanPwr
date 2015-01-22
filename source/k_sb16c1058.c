#define __K_SB16C1058_C__

//  <includes>
#include "k_includes.h"

//#define PRINTVAR(A)	DEBUGPRINTF("%s , %d :%s=0x%x(%d)\r\n",__FUNCTION__,__LINE__,#A,A,A)
//  <macros>
//  <data>

//  <functions>

/*
static u8 uart_ch_no = 0;
void SB_setUartCh(int a_ch)
{
	switch (a_ch){
		case 0:		uart_ch_no = (SB_CHIP_A + SB_UART_0);	break;
		case 1:		uart_ch_no = (SB_CHIP_A + SB_UART_1);	break;
		case 2:		uart_ch_no = (SB_CHIP_A + SB_UART_2);	break;
		case 3:		uart_ch_no = (SB_CHIP_A + SB_UART_3);	break;
		case 4:		uart_ch_no = (SB_CHIP_A + SB_UART_4);	break;
		case 5:		uart_ch_no = (SB_CHIP_A + SB_UART_5);	break;
		case 6:		uart_ch_no = (SB_CHIP_A + SB_UART_6);	break;
		case 7:		uart_ch_no = (SB_CHIP_A + SB_UART_7);	break;
		case 8:		uart_ch_no = (SB_CHIP_B + SB_UART_0);	break;
		case 9:		uart_ch_no = (SB_CHIP_B + SB_UART_1);	break;
		case 10:	uart_ch_no = (SB_CHIP_B + SB_UART_2);	break;
		case 11:	uart_ch_no = (SB_CHIP_B + SB_UART_3);	break;
		case 12:	uart_ch_no = (SB_CHIP_B + SB_UART_4);	break;
		case 13:	uart_ch_no = (SB_CHIP_B + SB_UART_5);	break;
		case 14:	uart_ch_no = (SB_CHIP_B + SB_UART_6);	break;
		case 15:	uart_ch_no = (SB_CHIP_B + SB_UART_7);	break;
		default:	uart_ch_no = (SB_CHIP_A + SB_UART_0);	break;
	}
    //debugprintf("%x\r\n", uart_ch_no);
}
*/

// a_ch : 0 ~ 15
static u32 sb_getbaseaddress ( int a_ch )
{
	u32 retval;

	switch (a_ch){
		case 0:		retval = (SB_CHIP_A + SB_UART_0);	break;
		case 1:		retval = (SB_CHIP_A + SB_UART_1);	break;
		case 2:		retval = (SB_CHIP_A + SB_UART_2);	break;
		case 3:		retval = (SB_CHIP_A + SB_UART_3);	break;
		case 4:		retval = (SB_CHIP_A + SB_UART_4);	break;
		case 5:		retval = (SB_CHIP_A + SB_UART_5);	break;
		case 6:		retval = (SB_CHIP_A + SB_UART_6);	break;
		case 7:		retval = (SB_CHIP_A + SB_UART_7);	break;
		case 8:		retval = (SB_CHIP_B + SB_UART_0);	break;
		case 9:		retval = (SB_CHIP_B + SB_UART_1);	break;
		case 10:	retval = (SB_CHIP_B + SB_UART_2);	break;
		case 11:	retval = (SB_CHIP_B + SB_UART_3);	break;
		case 12:	retval = (SB_CHIP_B + SB_UART_4);	break;
		case 13:	retval = (SB_CHIP_B + SB_UART_5);	break;
		case 14:	retval = (SB_CHIP_B + SB_UART_6);	break;
		case 15:	retval = (SB_CHIP_B + SB_UART_7);	break;
		default:	retval = (SB_CHIP_A + SB_UART_0);	break;
	}

	return (retval);
}

void sb_uart_rx_flush ( int ch )
{
	U32 i;
	//CRITICAL_ENTER();
	i= sb_get_register( ch, SB_UART_FCR ) | ( 1 << 1 ); //rx fifo reset
	//* ( volatile U32 * ) ( uart_base + UART_FCR ) |= ( 1 << 1 ); //rx fifo reset
	sb_set_register( ch, SB_UART_FCR, i );
	//CRITICAL_EXIT();
}
void sb_uart_tx_flush ( int ch )
{
	U32 i;
	//CRITICAL_ENTER();
	i= sb_get_register( ch, SB_UART_FCR ) | ( 1 << 2 ); //tx fifo reset
	//* ( volatile U32 * ) ( uart_base + UART_FCR ) |= ( 1 << 2 ); //tx fifo reset
	sb_set_register( ch, SB_UART_FCR, i );
	//CRITICAL_EXIT();
}



void sb_uart_rx_int_enable ( int ch, BOOL b )
{
	U32	i;

	if ( b )
	{
		i= sb_get_register( ch, SB_UART_IER ) | F_UIE_RDAIEN;
		//* ( U32 * ) ( uart_base + UART_IER ) |= F_UIE_RDAIEN;
		sb_set_register( ch, SB_UART_IER, i );
	}
	else
	{
		i= sb_get_register( ch, SB_UART_IER ) & (~F_UIE_RDAIEN);
		//* ( U32 * ) ( uart_base + UART_IER ) &= ~F_UIE_RDAIEN;
		sb_set_register( ch, SB_UART_IER, i );
	}
}


BOOL sb_uart_config ( int ch, int baud )
{
	u8 divisor;
	u8 temp;	//, temp1, temp2, temp3;

	switch ( baud )
	{
		case   4800:	divisor=0x18; 	break;
		case  19200:	divisor=0x06; 	break;
		case 115200:	divisor=0x01; 	break;
		default:		divisor=0x0C; 	break;	//9600
	}


	// 1. set Baud Rate
	//temp = sb_get_register(ch, SB_UART_LCR);
	sb_set_register(ch, SB_UART_LCR, 0x80);

												// page <1>
	sb_set_register(ch, SB_UART_DLL, divisor);
	sb_set_register(ch, SB_UART_DLM, 0x00);

	sb_set_register(ch, SB_UART_MCR, 0x08);		// 0000-0000 --> 0000-1000	//interrupt output enable

	//sb_set_register(ch, SB_UART_LCR, temp);


	// 2. set TTR to 20h
	sb_set_register(ch, SB_UART_LCR, 0xBF);
	sb_set_register(ch, SB_UART_PSR, 0xA5);
												// page <4>
	sb_set_register(ch, SB_UART_TTR, 0x20);

	// 3. set RTR to 80h
	sb_set_register(ch, SB_UART_RTR, 0x80);

	// 4. enable 256-byte FIFO
	sb_set_register(ch, SB_UART_AFR, 0x31);		// 0000-0000 --> 0011-0001  : set GINT

	// 5. set Line Control reg. to 8bit, no parity,1 stop bit
	sb_set_register(ch, SB_UART_PSR, 0xA4);
	sb_set_register(ch, SB_UART_LCR, 0x03);

	sb_set_register(ch, SB_UART_FCR, 0x07);		// reset FIFO & enable ( page 0 )


	// 6. enable TX,RX interrupt
	//sb_set_register(ch, SB_UART_IER, 0x03);
	sb_set_register(ch, SB_UART_IER, 0x01);		// RX INT enable only

	temp = sb_get_register(ch, SB_UART_MCR);
	sb_set_register(ch, SB_UART_MCR,  0x40);	// page <2>
	sb_set_register(ch, SB_UART_GICR, 0xFF);	// global int : non-masking

	sb_set_register(ch, SB_UART_MCR, temp);
	sb_set_register(ch, SB_UART_LCR, 0x03);

	return TRUE;
}




BOOL sb_uart_config_bak ( int ch, int baud, UART_DATABITS databits, UART_STOPBITS stopbit, UART_PARITY parity )
{
	U16 divisor;

	switch ( baud )
	{
		case 4800:		divisor=0x0018; 	break;
		case 115200:	divisor=0x0001; 	break;
		default:		divisor=0x000C; 	break;	//9600
	}


	U32 lcr;	//line control register
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

	sb_set_register(ch, SB_UART_FCR, F_UFC_FIFOEN | F_UFC_RFTL_1);

	lcr |= F_ULC_DLAB;
	sb_set_register(ch, SB_UART_LCR, lcr);
	//* ( volatile U32 * ) ( uart_base + UART_DIV_LO ) = divisor & 0xff;
	sb_set_register(ch, SB_UART_DLL, divisor & 0xff);
	//* ( volatile U32 * ) ( uart_base + UART_DIV_HI ) = ( divisor >> 8 ) & 0xff;
	sb_set_register(ch, SB_UART_DLM, ( divisor >> 8 ) & 0xff);
	lcr &= ~F_ULC_DLAB; //divisor latch access disable, fifo control write mode.
	//* ( volatile U32 * ) ( uart_base + UART_LCR ) = lcr;
	sb_set_register(ch, SB_UART_LCR, lcr);
	//* ( volatile U32 * ) ( uart_base + UART_IER ) = 0; //disable rx/tx interrupts
	sb_set_register(ch, SB_UART_IER, 0); //disable rx/tx interrupts


	sb_uart_rx_flush ( ch );
	sb_uart_tx_flush ( ch );
/*
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
*/
#ifdef CONFIG_UART_RX_INTERRUPT
	sb_uart_rx_int_enable ( ch, TRUE );
#endif
	return TRUE;
}



void SB_setPin_IOW(u8 a_level)
{
	if (a_level == 0) 	SB_PIN_IOW_L();
	else 				SB_PIN_IOW_H();
}

void SB_setPin_IOR(u8 a_level)
{
	if (a_level == 0) 	SB_PIN_IOR_L();
	else 				SB_PIN_IOR_H();
}

void SB_setPin_CS(u8 a_level)
{
	if (a_level == 0) 	SB_PIN_CS_L();
	else 				SB_PIN_CS_H();
}

void SB_setAddress(u32 a_add)
{
	//debugprintf("ADDRESS <<--  %x\r\n", a_add);

	*R_PAF3 |= 0xFFFF;		// (1)
	*R_GPODIR(3) = 0x1FF;		// (2)

	*R_GPDOUT(3) = a_add;
}

void SB_setPortMode_DataInput(void)
{
	*R_GPIDIR(8) |= 0x0FF;		// (2)
	*R_GPODIR(8) |= (1<<8);		// (3)
}

void SB_setPortMode_DataOutput(void)
{
	*R_GPODIR(8) |= 0x0FF;		// (2)
	*R_GPODIR(8) |= (1<<8);		// (3)
}


#define SB_GET_DATA(A)	(A = (*R_GPILEV(8))&0xFF)
#define SB_SET_DATA(A)	(*R_GPDOUT(8) = A);

void SB_reset(void)
{
	//debugstring("SB_reset\r\n");

	//SB_PIN_RESET_L();	delayms(500);
 	SB_PIN_RESET0_H();	//	delayms(1000);
 	SB_PIN_RESET1_H();	delayms(1000);
	SB_PIN_RESET0_L();	//delayms(500);
	SB_PIN_RESET1_L();	//delayms(500);

	sb1_ok_cnt = 0;
	sb2_ok_cnt = 0;
}


void sb_set_register(int ch, u8 reg, u8 data)
{
	u32 uart_base;
	uart_base = sb_getbaseaddress ( ch );

	//debugprintf("SB_setREG ((ch=%d) uart_base=%x, reg=%x, data=%x)\r\n", ch, uart_base, reg, data);

	SB_PIN_CS_H();
	SB_PIN_IOR_H();
	SB_PIN_IOW_H();

	SB_setPortMode_DataOutput();
	SB_setAddress( (u8)(uart_base+reg) );
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_SET_DATA(data);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_CS_L();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_IOW_L();

	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_IOW_H();
	asm("nop");
	asm("nop");

	SB_PIN_CS_H();
	asm("nop");
	asm("nop");
}


u8 sb_get_register(int ch, u8 reg)
{
	u8 data;
	u32 uart_base;
	uart_base = sb_getbaseaddress ( ch );

	//r_debugprintf("uart_base (%x)\r\n", a_add);
	//PRINTVAR(uart_base);

	SB_PIN_CS_H();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_IOR_H();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_IOW_H();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_setPortMode_DataInput();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_setAddress((u8)uart_base+reg);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_CS_L();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_IOR_L();
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_GET_DATA(data);

	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	SB_PIN_IOR_H();
	asm("nop");
	asm("nop");

	SB_PIN_CS_H();
	asm("nop");
	asm("nop");

	return (data);
}



void sb_write_SPR(int ch, u8 val1)
{
	u32 temp;

	//debugprintf("SB_setSPR(%x - %x) = ", SPR, val1);

	temp = sb_get_register(ch, SB_UART_LCR);
	//PRINTVAR(temp);

	sb_set_register(ch, SB_UART_LCR, 0x80);
	sb_set_register(ch, SB_UART_SPR, val1);

	sb_set_register(ch, SB_UART_LCR, temp);
}

u32 sb_read_SPR(int ch)
{
	u32 temp, retval ;

	temp = sb_get_register(ch, SB_UART_LCR);
	sb_set_register(ch, SB_UART_LCR, 0x80);

	retval = sb_get_register(ch, SB_UART_SPR);
	//debugprintf( "%x - %x\r\n", SPR, retval);
	sb_set_register(ch, SB_UART_LCR, temp);

	return retval;
}


BOOL sb_uart_putch ( int n, char a_ch )
{
    sb_set_register(n, SB_UART_THR, a_ch);

	// volatile u8 i, temp;

	// while (1)
	// {
	// 	temp = sb_get_register(n, SB_UART_LCR);
	// 	sb_set_register(n, SB_UART_LCR, 0x80);
	// 	i = sb_get_register(n, SB_UART_LSR) & F_ULS_TEMP;
	// 	sb_set_register(n, SB_UART_LCR, temp);
	// 	if (0 != i )
	// 		break;
	// }

	// asm("nop");
	// asm("nop");
	// asm("nop");
	// // asm("nop");
	// // asm("nop");
	// // asm("nop");
	// temp = sb_get_register(n, SB_UART_LCR);
	// i = sb_get_register(n, SB_UART_MCR);
	// sb_set_register(n, SB_UART_MCR, (i&(~0x40)));

	// sb_set_register(n, SB_UART_THR, a_ch);

	// sb_set_register(n, SB_UART_MCR, i);
	// sb_set_register(n, SB_UART_LCR, temp);

	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	// asm("nop");
	return TRUE;
}



void sb_test_readAll(int n)
{
	u8 t_lcr, t_mcr, t_psr;
	//u8 temp;

	//debugprintf("LCR [ %02X ]\r\n", t_lcr );


	// page 0
	debugprintf("---- page 0 ----\r\n");

	t_lcr = sb_get_register(n, SB_UART_LCR);
	if ( t_lcr & 0x80 )
	{
		sb_set_register(n, SB_UART_LCR, 0x00);
	}
	t_mcr = sb_get_register(n, SB_UART_MCR);
	if (t_mcr & 0x40)
	{
		sb_set_register(n, SB_UART_MCR, 0x00);
	}

	debugprintf("   ** LCR=%02x, MCR=%02X **\r\n", t_lcr, t_mcr);

	debugprintf("RBR [ %02X ]\r\n", sb_get_register(n, SB_UART_RBR) );
	debugprintf("IER [ %02X ]\r\n", sb_get_register(n, SB_UART_IER) );
	debugprintf("ISR [ %02X ]\r\n", sb_get_register(n, SB_UART_ISR) );
	debugprintf("LCR [ %02X ]\r\n", sb_get_register(n, SB_UART_LCR) );
	debugprintf("MCR [ %02X ]\r\n", sb_get_register(n, SB_UART_MCR) );
	debugprintf("LSR [ %02X ]\r\n", sb_get_register(n, SB_UART_LSR) );
	debugprintf("MSR [ %02X ]\r\n", sb_get_register(n, SB_UART_MSR) );
	debugprintf("SPR [ %02X ]\r\n", sb_get_register(n, SB_UART_SPR) );


	//Page 1
	//DLL,DLM
	debugprintf("---- page 1 ----\r\n");

	sb_set_register(n, SB_UART_LCR, 0x80);

	debugprintf("DLL [ %02X ]\r\n", sb_get_register(n, SB_UART_DLL) );
	debugprintf("DLM [ %02X ]\r\n", sb_get_register(n, SB_UART_DLM) );
	debugprintf("ISR [ %02X ]\r\n", sb_get_register(n, SB_UART_ISR) );
	debugprintf("LCR [ %02X ]\r\n", sb_get_register(n, SB_UART_LCR) );
	debugprintf("MCR [ %02X ]\r\n", sb_get_register(n, SB_UART_MCR) );
	debugprintf("LSR [ %02X ]\r\n", sb_get_register(n, SB_UART_LSR) );
	debugprintf("MSR [ %02X ]\r\n", sb_get_register(n, SB_UART_MSR) );
	debugprintf("SPR [ %02X ]\r\n", sb_get_register(n, SB_UART_SPR) );


	// page 2
	debugprintf("---- page 2 ----\r\n");

	sb_set_register(n, SB_UART_LCR, 0x00);
	sb_set_register(n, SB_UART_MCR, 0x40);

	debugprintf("GICR[ %02X ]\r\n", sb_get_register(n, SB_UART_GICR) );
	debugprintf("GISR[ %02X ]\r\n", sb_get_register(n, SB_UART_GISR) );
	debugprintf("LCR [ %02X ]\r\n", sb_get_register(n, SB_UART_LCR ) );
	debugprintf("MCR [ %02X ]\r\n", sb_get_register(n, SB_UART_MCR ) );
	debugprintf("TCR [ %02X ]\r\n", sb_get_register(n, SB_UART_TCR ) );
	debugprintf("RCR [ %02X ]\r\n", sb_get_register(n, SB_UART_RCR ) );
	debugprintf("FSR [ %02X ]\r\n", sb_get_register(n, SB_UART_FSR ) );


	// page 3 / page 4
	// BF
	sb_set_register(n, SB_UART_LCR, 0xBF);
	t_psr = sb_get_register(n, SB_UART_PSR);

	// page 3
	debugprintf("---- page 3 ----\r\n");
	sb_set_register(n, SB_UART_PSR, 0xA4);

	debugprintf("PSR [ %02X ]\r\n", sb_get_register(n, SB_UART_PSR ) );
	debugprintf("ATR [ %02X ]\r\n", sb_get_register(n, SB_UART_ATR ) );
	debugprintf("EFR [ %02X ]\r\n", sb_get_register(n, SB_UART_EFR ) );
	debugprintf("LCR [ %02X ]\r\n", sb_get_register(n, SB_UART_LCR ) );
	debugprintf("XON1[ %02X ]\r\n", sb_get_register(n, SB_UART_XON1) );
	debugprintf("XON2[ %02X ]\r\n", sb_get_register(n, SB_UART_XON2) );
	debugprintf("XOF1[ %02X ]\r\n", sb_get_register(n, SB_UART_XOFF1) );
	debugprintf("XOF2[ %02X ]\r\n", sb_get_register(n, SB_UART_XOFF2) );


	// page 4
	debugprintf("---- page 4 ----\r\n");
	sb_set_register(n, SB_UART_PSR, 0xA5);

	debugprintf("PSR [ %02X ]\r\n", sb_get_register(n, SB_UART_PSR ) );
	debugprintf("AFR [ %02X ]\r\n", sb_get_register(n, SB_UART_AFR ) );
	debugprintf("XRCR[ %02X ]\r\n", sb_get_register(n, SB_UART_XRCR) );
	debugprintf("LCR [ %02X ]\r\n", sb_get_register(n, SB_UART_LCR ) );
	debugprintf("TTR [ %02X ]\r\n", sb_get_register(n, SB_UART_TTR ) );
	debugprintf("RTR [ %02X ]\r\n", sb_get_register(n, SB_UART_RTR ) );
	debugprintf("FUR [ %02X ]\r\n", sb_get_register(n, SB_UART_FUR ) );
	debugprintf("FLR [ %02X ]\r\n", sb_get_register(n, SB_UART_FLR ) );


	// restore
	sb_set_register(n, SB_UART_PSR, (0xA4+t_psr) );
	t_psr = sb_get_register(n, SB_UART_DLL);
	sb_set_register(n, SB_UART_LCR, 0x00);
	sb_set_register(n, SB_UART_MCR, t_mcr);
	t_mcr = sb_get_register(n, SB_UART_MCR);
	sb_set_register(n, SB_UART_LCR, t_lcr);
	t_lcr = sb_get_register(n, SB_UART_LCR);

	debugprintf("   ** LCR=%02x, MCR=%02X ,PSR=%02X **\r\n", t_lcr, t_mcr, t_psr);
}

void sb_test_tx_out(void)
{
	//u8 i;
	char c = 'a';

	//for (i=8; i<16; i++)
	//for (i=0; i<8; i++)
	//for (i=0; i<16; i++)
	{
    	sb_uart_putch(2, c);
        //debugprintf("askjdlf;woejkdjflskdjflksdjflsjdfklsjflkdjsfljdlkfs\r\n");
	}
}

void SB_init(void)
{
	//int i;

	//debugprintf("sb16c1058 init....\r\n");
	debugstring("#");

	enable_interrupt(INTNUM_EIRQ0, FALSE);
	enable_interrupt(INTNUM_EIRQ1, FALSE);


	SB_reset();

 // sb_uart_config(  0, 9600 );		// Iridium 1
 // sb_uart_config(  1, 9600 );		// Iridium 2
	//sb_uart_config(  2, 9600 );		// AIO
	sb_uart_config(  2, 19200 );		// AIO
	sb_uart_config(  3, 4800 );		// MOSE
	sb_uart_config(  4, 9600 );		// PTB210
 // sb_uart_config(  5, 9600 );
  	sb_uart_config(  6, 9600 );		// CHK1 - for check at boot time
	sb_uart_config(  7, 9600 );		// ATM1
	sb_uart_config(  8, 9600 );		// ATM2
 	sb_uart_config(  9, 9600 );		// CHK2 - for check at boot time
	sb_uart_config(  10, 9600 );	// Shock
	sb_uart_config(  11, 4800 );	// GPS
	sb_uart_config(  12, 9600 );	// CT3919
	sb_uart_config(  13, 9600 );	// DCS
	sb_uart_config(  14, 9600 );	// DCS
	sb_uart_config(  15, 9600 );	// HMP155

	delayms(10);
	enable_interrupt(INTNUM_EIRQ0, TRUE);
	enable_interrupt(INTNUM_EIRQ1, TRUE);
}







void isr_sub (u8 ch)
{
	u8 lcr;
	u8 temp1, temp2, temp3;


#if 1 	// for test only
	*R_GPOHIGH(4) |= (1<<0);
#endif


	// read ISR
	lcr = sb_get_register(ch, SB_UART_LCR);
	sb_set_register(ch, SB_UART_LCR, lcr & ~0x80);
	//sb_set_register(ch, SB_UART_LCR, 0x80);

	temp1 = sb_get_register(ch, SB_UART_ISR);
	temp2 = sb_get_register(ch, SB_UART_MCR);

	//debugprintf("isr_sub(%d) isr= %02X\r\n", ch, temp1);

	sb_set_register(ch, SB_UART_MCR, (temp2 | 0x40) );		// page 2

	switch (( temp1 & ~(0xC0)))
	{
		case 0x04:
		case 0x0C:
			// rx-isr
			///debugprintf("rx-isr(%d)\r\n", ch);

			temp3 = sb_get_register(ch, SB_UART_RCR);
			sb_set_register(ch, SB_UART_MCR, temp2 );

			//debugprintf("RCR(%d)= %d\r\n", ch, temp3);

			if (0x40 == (temp1 & 0x40))
			{
				int i,j;
				for (i=0;i<=255;i++)
				{
					j = sb_get_register(ch, SB_UART_RBR);
#if 1
					switch (ch)
					{
						case SB_S_CT3919:
							//ct_q_put(j);
							//debugprintf("%c",j);
							ct_q_buf[ct_q.rear] = j;
							++ct_q.rear;
							ct_q.rear %= Q_CT_MAX;
							ct_q.len++;

							//UartPutCh(UART_DEBUG, j);
							break;

						case SB_S_PTB210:
							//ct_q_put(j);
							//debugprintf("%c",j);
						    ptb_q_buf[ptb_q.rear] = j;
						    ++ptb_q.rear;
						    ptb_q.rear %= Q_PTB_MAX;
						    ptb_q.len++;

							//UartPutCh(UART_DEBUG, j);
							break;

						case SB_S_DCS:
						case SB_S_DCS1:
							//PRINTLINE;
						    dcs_q_buf[dcs_q.rear] = j;
						    ++dcs_q.rear;
						    dcs_q.rear %= Q_DCS_MAX;
						    dcs_q.len++;
							//dcs_q_buf(j);
							//debugprintf("%c",j);
							//PRINTLINE;

							//UartPutCh(UART_DEBUG, j);
							break;
						case SB_S_MOSE:
						    mose_q_buf[mose_q.rear] = j;
						    ++mose_q.rear;
						    mose_q.rear %= Q_MOSE_MAX;
						    mose_q.len++;
						    break;

						case SB_S_AIO:
							aio_q_buf[aio_q.rear] = j;
							++aio_q.rear;
							aio_q.rear %= Q_AIO_MAX;
							aio_q.len++;
						    break;

					   	case SB_S_HMP155:
						    hmp155_q_buf[hmp155_q.rear] = j;
						    ++hmp155_q.rear;
						    hmp155_q.rear %= Q_HMP155_MAX;
						    hmp155_q.len++;
						    break;

						case SB_S_GPS:
							gps_q_buf[gps_q.rear] = j;
							++gps_q.rear;
							gps_q.rear %= Q_GPS_MAX;
							gps_q.len++;
							break;

						case SB_S_ATM1:
						    atm1_q_buf[atm1_q.rear] = j;
						    ++atm1_q.rear;
						    atm1_q.rear %= Q_ATM_MAX;
						    atm1_q.len++;
							break;

						case SB_S_ATM2:
						    atm2_q_buf[atm2_q.rear] = j;
						    ++atm2_q.rear;
						    atm2_q.rear %= Q_ATM_MAX;
						    atm2_q.len++;
							break;

						case SB_S_SHOCK:
						    shock_q_buf[shock_q.rear] = j;
						    ++shock_q.rear;
						    shock_q.rear %= Q_SHOCK_MAX;
						    shock_q.len++;
						    break;

					   	case SB_S_CHK1:
					    	sb1_ok_cnt++;
					    	break;

					   	case SB_S_CHK2:
					    	sb2_ok_cnt++;
					    	break;

						default:
							UartPutCh(UART_DEBUG, j);
							break;
					}
#else
					UartPutCh(UART_DEBUG, j);
#endif

				}
			}
			else
			{
				int i;
				int j;
				for (i=0;i<=temp3;i++)
				{
					j = sb_get_register(ch, SB_UART_RBR);
#if 1
					switch (ch)
					{

						case SB_S_CT3919:
							//ct_q_put(j);
							//debugprintf("%c",j);
							ct_q_buf[ct_q.rear] = j;
							++ct_q.rear;
							ct_q.rear %= Q_CT_MAX;
							ct_q.len++;

							//UartPutCh(UART_DEBUG, j);
							break;

						case SB_S_PTB210:
							//ct_q_put(j);
							//debugprintf("%c",j);
						    ptb_q_buf[ptb_q.rear] = j;
						    ++ptb_q.rear;
						    ptb_q.rear %= Q_PTB_MAX;
						    ptb_q.len++;

							//UartPutCh(UART_DEBUG, j);
							break;

						case SB_S_DCS:
						case SB_S_DCS1:
							//PRINTLINE;
						    dcs_q_buf[dcs_q.rear] = j;
						    ++dcs_q.rear;
						    dcs_q.rear %= Q_DCS_MAX;
						    dcs_q.len++;
							//dcs_q_buf(j);
							//debugprintf("%c",j);
							//PRINTLINE;

							//UartPutCh(UART_DEBUG, j);
							break;
						case SB_S_MOSE:
						    mose_q_buf[mose_q.rear] = j;
						    ++mose_q.rear;
						    mose_q.rear %= Q_MOSE_MAX;
						    mose_q.len++;
						    break;

						case SB_S_AIO:
							aio_q_buf[aio_q.rear] = j;
							++aio_q.rear;
							aio_q.rear %= Q_AIO_MAX;
							aio_q.len++;
						    break;

					   	case SB_S_HMP155:
						    hmp155_q_buf[hmp155_q.rear] = j;
						    ++hmp155_q.rear;
						    hmp155_q.rear %= Q_HMP155_MAX;
						    hmp155_q.len++;
						    break;

						case SB_S_GPS:
							gps_q_buf[gps_q.rear] = j;
							++gps_q.rear;
							gps_q.rear %= Q_GPS_MAX;
							gps_q.len++;
							break;

						case SB_S_ATM1:
						    atm1_q_buf[atm1_q.rear] = j;
						    ++atm1_q.rear;
						    atm1_q.rear %= Q_ATM_MAX;
						    atm1_q.len++;
							break;

						case SB_S_ATM2:
						    atm2_q_buf[atm2_q.rear] = j;
						    ++atm2_q.rear;
						    atm2_q.rear %= Q_ATM_MAX;
						    atm2_q.len++;
							break;

						case SB_S_SHOCK:
						    shock_q_buf[shock_q.rear] = j;
						    ++shock_q.rear;
						    shock_q.rear %= Q_SHOCK_MAX;
						    shock_q.len++;
						    break;

					   	case SB_S_CHK1:
					    	sb1_ok_cnt++;
					    	break;

					   	case SB_S_CHK2:
					    	sb2_ok_cnt++;
					    	break;

						default:
							UartPutCh(UART_DEBUG, j);
							break;
					}
#else
					UartPutCh(UART_DEBUG, j);
#endif
				}
			}
			break;

		case 0x02:
			// tx-isr
			//debugprintf("tx-isr(%d)\r\n", ch);

			temp3 = sb_get_register(ch, SB_UART_TCR);
			sb_set_register(ch, SB_UART_MCR, temp2 );

			//debugprintf("TCR(%d)= %d\r\n", ch, temp3);

			if (0x80 == (temp1 & 0x80))
			{
				int i;	//,j;
				for (i=0;i<=127;i++)
				{
					//j = sb_get_register(ch, SB_UART_RBR);
					//debugprintf("%c",j);
				}
			}
			break;
	}

	// restore LCR,MCR
	sb_set_register(ch, SB_UART_LCR, 0x80);
	sb_set_register(ch, SB_UART_MCR, temp2);
	sb_set_register(ch, SB_UART_LCR, lcr);


#if 1 	// for test only
	*R_GPOLOW(4) |= (1<<0);
#endif

}


#ifndef CRITICAL_ENTER
#define CRITICAL_ENTER()	asm("set 8");asm("sync") // back up interrupt status
#endif

#ifndef CRITICAL_EXIT
#define CRITICAL_EXIT()		asm("clr 8");asm("sync") // restor interrupt status
#endif



void EIRQ0_ISR(void)
{
	// INT-2

	volatile u8 t1,t2;
	volatile u8 ch;
	volatile u8 gisr;

	CRITICAL_ENTER();

	//debugstring("-");

    sb2_high_cnt = 0;


	// read GISR -- IC1???€?œí•˜??ch=8??GISR ??read..

	while (1)
	{
		ch = 15;
		t1 = sb_get_register(ch, SB_UART_LCR);
		sb_set_register(ch, SB_UART_LCR, 0x00) ;
		t2 = sb_get_register(ch, SB_UART_MCR);
		sb_set_register(ch, SB_UART_MCR, 0x40) ;

		//sb_set_register(ch, SB_UART_GICR, 0);
		gisr = sb_get_register(ch, SB_UART_GISR);

		sb_set_register(ch, SB_UART_MCR, t2);
		sb_set_register(ch, SB_UART_LCR, t1);

		if (0 == gisr)
		{
			break;
		}

		//PRINTVAR( gisr );

		//sb2_ok_cnt++;

		if ( gisr & 0x01)	isr_sub( 8);		// uart  8
		if ( gisr & 0x02)	isr_sub( 9);		// uart  9
		if ( gisr & 0x04)	isr_sub(10);		// uart 10
		if ( gisr & 0x08)	isr_sub(11);		// uart 11
		if ( gisr & 0x10)	isr_sub(12);		// uart 12
		if ( gisr & 0x20)	isr_sub(13);		// uart 13
		if ( gisr & 0x40)	isr_sub(14);		// uart 14
		if ( gisr & 0x80)	isr_sub(15);		// uart 15

	}

	//ch = 15;
	//sb_set_register(ch, SB_UART_LCR, 0x00) ;
	//sb_set_register(ch, SB_UART_MCR, 0x40) ;

	//sb_set_register(ch, SB_UART_GICR, 0xFF);

	//sb_set_register(ch, SB_UART_MCR, t2);
	//sb_set_register(ch, SB_UART_LCR, t1);

	CRITICAL_EXIT();
}

void EIRQ1_ISR(void)
{
	// INT-1

	volatile u8 t1,t2;
	volatile u8 ch;
	volatile u8 gisr;

	CRITICAL_ENTER();

	//debugstring("+");

    sb1_high_cnt = 0;

    while (1)
    {
    	// read GISR -- IC1???€?œí•˜??ch=0??GISR ??read..
    	ch = 0;
    	t1 = sb_get_register(ch, SB_UART_LCR);
    	sb_set_register(ch, SB_UART_LCR, 0x00);
    	t2 = sb_get_register(ch, SB_UART_MCR);
    	sb_set_register(ch, SB_UART_MCR, 0x40);		// page 2

    	//sb_set_register(ch, SB_UART_GICR, 0);
    	gisr = sb_get_register(ch, SB_UART_GISR);	// GISR

    	sb_set_register(ch, SB_UART_MCR, t2);
    	sb_set_register(ch, SB_UART_LCR, t1);		// restore LCR, MCR
    	//PRINTVAR( gisr );

    	if (0== gisr)
    	{
    		break;
    	}
    	//
    	//sb1_ok_cnt++;

    	if ( gisr & 0x01)	isr_sub(0);		// uart 0
    	if ( gisr & 0x02)	isr_sub(1);		// uart 1
    	if ( gisr & 0x04)	isr_sub(2);		// uart 2
    	if ( gisr & 0x08)	isr_sub(3);		// uart 3
    	if ( gisr & 0x10)	isr_sub(4);		// uart 4
    	if ( gisr & 0x20)	isr_sub(5);		// uart 5
    	if ( gisr & 0x40)	isr_sub(6);		// uart 6
    	if ( gisr & 0x80)	isr_sub(7);		// uart 7

    }

	// ch = 0;
	//sb_set_register(ch, SB_UART_LCR, 0x00) ;
	//sb_set_register(ch, SB_UART_MCR, 0x40) ;

	//sb_set_register(ch, SB_UART_GICR, 0xFF);

	// sb_set_register(ch, SB_UART_MCR, t2);
	// sb_set_register(ch, SB_UART_LCR, t1);

	CRITICAL_EXIT();
}


BOOL sb_uart_putdata ( int n, U8 *buf, int len )
{
	int i;

	for ( i = 0; i < len; i++ )
		sb_uart_putch ( n, buf[i] );

	return TRUE;
}
BOOL sb_printstring ( int n, U8 *buf )
{
	CRITICAL_ENTER();

	while ( *buf )
	{
		sb_uart_putch ( n, *buf );
		// delayms(50);
		buf++;
	}

	CRITICAL_EXIT();

	return TRUE;
}

void sb_printf ( int n, const char *rsp,	... )
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

	sb_printstring ( n, print_buf );
}


BOOL sb_putch1 ( int n, char a_ch )
{
	CRITICAL_ENTER();

	//while ( *buf )
	{
		sb_uart_putch ( n, a_ch );
		//buf++;
	}

	CRITICAL_EXIT();

	return TRUE;
}



