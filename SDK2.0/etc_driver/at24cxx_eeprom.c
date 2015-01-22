#include "adstar.h"
#include "../include/etc_driver/at24cxx_eeprom.h"

/*
 * addr : Word Address
 * buffer : Writing Buffer
 * length : Size of Writing
 */
int at24cxx_eeprom_write (U16 addr, U8 *buffer, int length)
{  
	U32 startaddr = addr;
	U32 endaddr = addr+length-1;
	U32 startpage;
	U32 endpage;
	U32 offset;
	U32 i;
	int wcnt=0,len, ret;
    U8 waddr[2];
	
	if ((length <= 0) || (addr > AT24CXX_EEPROM_MAXADDR)) {
        debugstring("Invalid Address\n\r");
        return -1;
    }
	
	startpage = startaddr/AT24CXX_EEPROM_PAGESIZE;
	endpage = endaddr/AT24CXX_EEPROM_PAGESIZE;
	
	for(i=startpage;i<endpage+1;i++)
	{
		offset = addr%AT24CXX_EEPROM_PAGESIZE;
		
		if(offset + length > AT24CXX_EEPROM_PAGESIZE)
		{
			len = AT24CXX_EEPROM_PAGESIZE - offset;
		}
		else
		{
			len = length;
		}

		waddr[0] = addr >> 8;
		waddr[1] = addr & 0xff;

		ret = twi_write (AT24CXX_EEPROM_ADDR, waddr, 2, buffer, len);
		if (ret < 0) {
			PRINTLINE;		
			
			break;
		}
		
		/*  Write Cycle Time of AT24CXX EEPROM: 5ms */
		delayms(5);
		
		buffer += len;
		addr += len;
		wcnt += len;
		length -= len;
	} 
    return wcnt;
}

/*
 * addr : Word Address
 * buffer : Reading Buffer
 * length : Size of Reading
 */
int at24cxx_eeprom_read (u16 addr, u8 *buffer, int length)
{
    int rcnt;
    U8 waddr[2];

    if ((length <= 0) || (addr > AT24CXX_EEPROM_MAXADDR)) {
        debugstring("Invalid Address\n\r");	
        return -1;
	}

    waddr[0] = addr >> 8;
    waddr[1] = addr & 0xff;

    rcnt = twi_read (AT24CXX_EEPROM_ADDR, waddr, 2, buffer, length);

    return rcnt;
}
