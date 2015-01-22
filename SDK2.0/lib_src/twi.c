/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "typedef.h"
#include "lib_config.h"
#include "adstar/twi.h"
#include "adstar/pmu.h"
#include "adstar/uart.h"

int twi_set_freq (U32 freq)
{
    unsigned long long tmp;
    U32 twibr0_cnt, twibr1_cnt;

    tmp = (unsigned long long)get_apb_clock()*700/1000000000;    
    twibr0_cnt = tmp + 3;
    twibr1_cnt = (get_apb_clock()/(2*freq)) - ((twibr0_cnt+7) >> 1);
            
	*R_TWIBR0 = twibr0_cnt;
	*R_TWIBR1 = twibr1_cnt;

    return 0;
}

BOOL twi_trans_complete(void)
{
    U32 status;
	int cnt = TWI_RESP_TIME_OUT_COUNT;
	register volatile U32* pSts asm("%r14")=R_TWISTAT;
	
	while(cnt--)
	{
        status = *pSts;
		if((status & 0x80) == 0x80)
			return TRUE;
    }
	debugstring("twi_trans_complete() time out!\r\n");
	return FALSE;
}

BOOL twi_wait_busy(void)    /* TWI Status is Busy */
{
	U32 status;
	int cnt = TWI_RESP_TIME_OUT_COUNT;
	
	register volatile U32* pSts asm("%r14")=R_TWISTAT;
	
    while(cnt--)
	{
        status = *pSts ;
		if((status & F_TWISTAT_TWIBUSY)==0)
			return TRUE;
    }
	debugstring("twi_wait_busy() time out!\r\n");			
	return FALSE;

}

/*
 * devaddr : Base of DEVICE ADDRESS
 * waddr : Buffer of Word Address
 * len : length of 'waddr', 1 or 2
 * flags : TWI_TRANSMIT, TWI_RECEIVE
 */
BOOL twi_start (U8 devaddr, U8 *waddr, int len, int flags)
{
    int i;
    U8 con = 0;
    U16 status;
    con = F_TWICTRL_TWIEN | F_TWICTRL_TWIMOD_MSTART | F_TWICTRL_TWITR_MTRM;
	
	*R_TWIDATA = devaddr;
	
	*R_TWICTRL = con;
	
	if(FALSE == twi_trans_complete())
	{
		return FALSE;
	}

    for (i = 0; i < len; i++) {
		*R_TWIDATA = waddr[i];
		
        if(FALSE == twi_trans_complete())
		{
			return FALSE;
		}
    } 

    if (flags == TWI_RECEIVE) {
        con = F_TWICTRL_TWIEN | F_TWICTRL_TWIMOD_MSTART | F_TWICTRL_REPST_RSTART;
		
		*R_TWICTRL = con;
		*R_TWIDATA = devaddr+1;
		
		if(FALSE == twi_trans_complete())
		{
			return FALSE;
		}

        status  = *R_TWIDATA;     // Read for SCL Generation
			
        if(FALSE == twi_trans_complete())
		{
			return FALSE;
		}
    }
	
	return TRUE;
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * waddr : Buffer of Word Address
 * len : length of 'waddr', 1 or 2
 * buffer : Writing buffer
 * length : Size of Writing
 */
int twi_write(U8 devaddr, U8 *waddr,
        int len, U8 *buffer, int length)
{
    int write_num, i;

    if(length <= 0)
        return -1;

    write_num = 0;

    twi_start (devaddr, waddr, len, TWI_TRANSMIT);

	for (i = 0; i < length; i++) {		
		*R_TWIDATA = buffer[i];// When word is 8bit, use this
			
        if(FALSE == twi_trans_complete())
		{
			return -1;
		}
		
        write_num++;
    }
	*R_TWICTRL = F_TWICTRL_TWIEN;
    return write_num;
}

/*
* devaddr : Base of DEVICE ADDRESS
* waddr : Byte Address
* buffer : Writing buffer
* length : Size of Writing
*/
int twi_write2(U8 devaddr, U8 waddr, U8 *buffer, int length)
{
	int write_num, i;

	if(length <= 0)
		return -1;

	write_num = 0;

	twi_start (devaddr, &waddr, 1, TWI_TRANSMIT);
	for (i = 0; i < length; i++) {
		*R_TWIDATA = buffer[i];
		
		if(FALSE == twi_trans_complete())
		{
			return -1;
		}
		
		write_num++;
	}

	*R_TWICTRL = F_TWICTRL_TWIEN;
	if(FALSE == twi_wait_busy())
	{
		return -1;
	}

	return write_num;
}

/*
 * devaddr : Base of DEVICE ADDRESS
 * addr : Word Address
 * len : length of Word Address (1 or 2)
 * buffer : read buffer
 * length : length of buffer
 */
int twi_read(U8 devaddr, U8 *waddr,
        int len, U8 *buffer, int length)
{
    U32 i;
	U8	tmp;
    int read_cnt = 0;

    twi_start (devaddr, waddr, len, TWI_RECEIVE);

	for ( i = 0; i < length-1; i++ ) {
		buffer[i] = *R_TWIDATA;
		
        if(FALSE == twi_trans_complete())
		{
			return -1;
		}
		
        read_cnt++;
    }

	*R_TWICTRL = F_TWICTRL_TWIEN | F_TWICTRL_TWIMOD_MSTART | F_TWICTRL_TWIAK_NACK;
    buffer[i] = *R_TWIDATA;
    if(FALSE == twi_trans_complete())
	{
		return -1;
	}
    read_cnt++;

    tmp = *R_TWIDATA;    // dummy read
 

	*R_TWICTRL = F_TWICTRL_TWIEN;
    if(FALSE == twi_wait_busy())
	{
		return -1;
	}
    
	return read_cnt;
}
