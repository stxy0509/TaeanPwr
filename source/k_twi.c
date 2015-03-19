#define __K_TWI_C__

#include "k_includes.h"


void twi_init(void)
{
	twi_set_freq (100000);
}




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




//************************************************************************************
//                      I2C RTCC DRIVERS
//************************************************************************************
int rtcc_wr(u8 time_var, u8 rtcc_reg)
                              // writes a constant/variable in a RTCC register
{
 	// i2c_start()             ;  // start I2C communication
  //  i2c_wr(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
  //  i2c_wr(rtcc_reg)        ;  // write the register's ADDRESS
  //  i2c_wr(time_var)        ;  // write byte variable in the register
  //  i2c_stop()              ;  // stop I2C communication
  //  dly39us()        ;

    int rcnt;
    U8 waddr[2];
    U8 buffer[8];// = {};

    //if ((length <= 0) || (addr > AT24CXX_EEPROM_MAXADDR)) {
    //    debugstring("Invalid Address\n\r");
    //    return -1;
	//}

    buffer[1] = 0;	buffer[0]= time_var;

    waddr[1] = 0;	waddr[0] = rtcc_reg;

    rcnt = twi_write2 (ADDR_RTCC_WRITE, rtcc_reg, buffer, 1);

    return rcnt;
}  // free time STOP - START
//.....................................................................................
int rtcc_rd(u8 *buffer)  // reads a register of the RTCC
{
	// i2c_start()             ;  // start I2C communication
	// i2c_wr(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
	// i2c_wr(rtcc_reg)        ;  // write the register ADDRESS
	// i2c_restart()           ;  // RESTART for READS
	// i2c_wr(ADDR_RTCC_READ)  ;  // send the DEVICE ADDRESS for RTCC READS.
	// i2c_rd()                ;  // read register (stored in 'rtcc_buf')
	// i2c_nack()              ;  // NOACK from MASTER (last read byte)
	// i2c_stop()              ;  // stop I2C communication
	// dly39us()               ;  // free time STOP - START
	// return rtcc_buf    ;

    int rcnt;
    U8 waddr[2];
    //U8 buffer[8];// = {};

    //if ((length <= 0) || (addr > AT24CXX_EEPROM_MAXADDR)) {
    //    debugstring("Invalid Address\n\r");
    //    return -1;
	//}

    //buffer[1] = 0;	buffer[0]= time_var;

    waddr[1] = 0;	waddr[0] = 0;

    rcnt = twi_read (ADDR_RTCC_WRITE, waddr, 1, buffer, 7);

    {
    	//int i;
    	u8 tmp, i;	//,j,k;

    	tmp = buffer[0];
    	i=0;
    	if (tmp&0x80)	i=1;
    	tmp &= (~0x80);
    	rtc_time.sec= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("0=%02x: ST[%d] sec[%d]\r\n", buffer[0], i, rtc_time.sec);
		//rtc_time.sec = j;

    	tmp = buffer[1];
    	rtc_time.min= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("1=%02x: min[%d]\r\n", buffer[1], rtc_time.min);
    	//rtc_time.min = j;

    	tmp = buffer[2];
    	i=0;
    	if (tmp&0x40)	i=1;
    	tmp &= (~0x40);
    	rtc_time.hour= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("2=%02x: 1224[%d], hour[%d]\r\n", buffer[2], i, rtc_time.hour);
    	//rtc_time.hour = j;

    	tmp = buffer[3];
    	//debugprintf("3=%02x: \r\n", buffer[3]);

    	tmp = buffer[4];
    	tmp &= (~0xC0);
    	rtc_time.day= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("4=%02x: date[%d]\r\n", buffer[4], rtc_time.day);
    	//rtc_time.date = j;

    	tmp = buffer[5];
    	tmp &= (~0xE0);
    	rtc_time.mon = (tmp>>4)*10 + (tmp & 0x0f);
    	// debugprintf("5=%02x: mon[%d]\r\n", buffer[5], rtc_time.mon);

    	tmp = buffer[6];
    	rtc_time.year = (tmp>>4)*10 + (tmp & 0x0f) + 2000;
    	// debugprintf("6=%02x: yr[%d]\r\n", buffer[6], rtc_time.year);
    }
    return rcnt;
}  // store the read byte in 'time_var'

/*
//................................................................................
int rtcc_rd(u8 *buffer)  // reads a register of the RTCC
{
    int rcnt;
    U8 waddr[2];
    //U8 buffer[8];// = {};

    //if ((length <= 0) || (addr > AT24CXX_EEPROM_MAXADDR)) {
    //    debugstring("Invalid Address\n\r");
    //    return -1;
	//}

    //buffer[1] = 0;	buffer[0]= time_var;

    waddr[1] = 0;	waddr[0] = 0;

    rcnt = twi_read (ADDR_RTCC_WRITE, waddr, 1, buffer, 7);

    {
    	//int i;
    	u8 tmp, i,j,k;

    	tmp = buffer[0];
    	i=0;
    	if (tmp&0x80)	i=1;
    	tmp &= (~0x80);
    	rtc_time.sec= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("0=%02x: ST[%d] sec[%d]\r\n", buffer[0], i, rtc_time.sec);
		//rtc_time.sec = j;

    	tmp = buffer[1];
    	rtc_time.min= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("1=%02x: min[%d]\r\n", buffer[1], rtc_time.min);
    	//rtc_time.min = j;

    	tmp = buffer[2];
    	i=0;
    	if (tmp&0x40)	i=1;
    	tmp &= (~0x40);
    	rtc_time.hour= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("2=%02x: 1224[%d], hour[%d]\r\n", buffer[2], i, rtc_time.hour);
    	//rtc_time.hour = j;

    	tmp = buffer[3];
    	//debugprintf("3=%02x: \r\n", buffer[3]);

    	tmp = buffer[4];
    	tmp &= (~0xC0);
    	rtc_time.day= (tmp>>4)*10 + (tmp & 0x0f);
    	//debugprintf("4=%02x: date[%d]\r\n", buffer[4], rtc_time.day);
    	//rtc_time.date = j;

    	tmp = buffer[5];
    	tmp &= (~0xE0);
    	rtc_time.mon = (tmp>>4)*10 + (tmp & 0x0f);
    	// debugprintf("5=%02x: mon[%d]\r\n", buffer[5], rtc_time.mon);

    	tmp = buffer[6];
    	rtc_time.year = (tmp>>4)*10 + (tmp & 0x0f) + 2000;
    	// debugprintf("6=%02x: yr[%d]\r\n", buffer[6], rtc_time.year);
    }
    return rcnt;
}  // store the read byte in 'time_var'
*/

//................................................................................
void rtc_gettime(rtcTime* tm)
{
    int rcnt;
    U8 waddr[2];
    U8 buffer[8];
   	u8 tmp;	//, i,j,k;

    waddr[1] = 0;	waddr[0] = 0;

    rcnt = twi_read (ADDR_RTCC_WRITE, waddr, 1, buffer, 7);

	tmp = buffer[0] & (~0x80);
	tm->sec = (tmp>>4)*10 + (tmp & 0x0f);

	tm->min= (buffer[1]>>4)*10 + (buffer[1] & 0x0f);

	tmp = buffer[2] & (~0x40);
	tm->hour= (tmp>>4)*10 + (tmp & 0x0f);

	tmp = buffer[4] & (~0xC0);
	tm->day= (tmp>>4)*10 + (tmp & 0x0f);

	tmp = buffer[5] & (~0xE0);
	tm->mon = (tmp>>4)*10 + (tmp & 0x0f);

	tm->year = (buffer[6]>>4)*10 + (buffer[6] & 0x0f) + 2000;
}

void rtc_settime(rtcTime* tm)
{
	u8 tmp1,tmp2;
    debugprintf("RTC set_time\r\n");

	rtcc_wr(0x30,ADDR_SEC) ;  //init SEC   register, set START bit

	delayms(2);
	//if((day&OSCON)==OSCON) {;}       // if oscillator = already running, do nothing.
	//else
	//{                           // if oscillator = not running, set time/date(arbitrary)
	// and SART oscillator/ crystal
    tmp1 = tm->year-2000;
    tmp2 = (tmp1/10 )*16 + (tmp1%10 );
    //debugprintf("Y: %d %x\r\n",tmp1,tmp2);
	rtcc_wr(tmp2,ADDR_YEAR)    ;     // initialize YEAR  register

    tmp1 = tm->mon;
    tmp2 = (tmp1/10 )*16 + (tmp1%10 );
    //debugprintf("M: %d %x\r\n",tmp1,tmp2);
	rtcc_wr(tmp2,ADDR_MNTH)    ;     // initialize MONTH register

    tmp1 = tm->day;
    tmp2 = (tmp1/10 )*16 + (tmp1%10 );
    //debugprintf("D: %d %x\r\n",tmp1,tmp2);
	rtcc_wr(tmp2,ADDR_DATE)    ;     // initialize DATE  register

    tmp1 = tm->hour;
    tmp2 = (tmp1/10 )*16 + (tmp1%10 );
    //debugprintf("H: %d %x\r\n",tmp1,tmp2);
	rtcc_wr(tmp2,ADDR_HOUR)    ;     // initialize HOUR  register

    tmp1 = tm->min;
    tmp2 = (tmp1/10 )*16 + (tmp1%10 );
    //debugprintf("M: %d %x\r\n",tmp1,tmp2);
	rtcc_wr(tmp2,ADDR_MIN)     ;     // initialize MIN   register

    tmp1 = tm->sec;
    tmp2 = (tmp1/10 )*16 + (tmp1%10 );
    //debugprintf("S: %d %x\r\n",tmp1,tmp2);
	rtcc_wr((tmp2)|START_32KHZ,ADDR_SEC) ;
    //}  //init SEC   register, set START bit
}

void rtc_start(void)
{
    u8 tmp1,tmp2;
    tmp1 = 10;
    tmp2 = (tmp1/10 )*16 + (tmp1%10 );
    //debugprintf("S: %d %x\r\n",tmp1,tmp2);
    rtcc_wr((tmp2)|START_32KHZ,ADDR_SEC) ;
    //}  //init SEC   register, set START bit
}




//................................................................................
void ini_rtcc(void)                  // initialization of the RTCC(MCP79410)
{                                      // START bit is located in the sec register
	u8 buffer[10];
	u8 day;

	rtcc_rd(buffer);           // read day + OSCON bit

	day = buffer[3];           // read day + OSCON bit
	// keep the previous value of 'seconds'
	rtcc_wr(SQWE+ALM_NO+MFP_01H, ADDR_CTRL); // square vawe on MFP, no alarms, MFP = 1Hz
	// write this init constant in CONTROL REG
	// final settings are: // crystal = started,
	// 24H format,square vawe on MFP,no alarms, MFP=1Hz
	rtcc_wr(day|VBATEN, ADDR_DAY) ;
}      // enable the battery back-up


//.........................................................................................
void ini_time(void)              // initialization of time/date:2010_03_01 , 9am.
{                                  // it initializes also :24H format, and START OSC.
	u8 day;

	rtcc_wr(0x30,ADDR_SEC) ;  //init SEC   register, set START bit

	if((day&OSCON)==OSCON) {;}       // if oscillator = already running, do nothing.
	else {                           // if oscillator = not running, set time/date(arbitrary)
	// and SART oscillator/ crystal
	rtcc_wr(0x14,ADDR_YEAR)    ;     // initialize YEAR  register
	rtcc_wr(0x10,ADDR_MNTH)    ;     // initialize MONTH register
	rtcc_wr(0x17,ADDR_DATE)    ;     // initialize DATE  register
	rtcc_wr(0x09,ADDR_HOUR)    ;     // initialize HOUR  register
	rtcc_wr(0x01,ADDR_MIN)     ;     // initialize MIN   register
	rtcc_wr(0x00|START_32KHZ,ADDR_SEC) ; }  //init SEC   register, set START bit
}


