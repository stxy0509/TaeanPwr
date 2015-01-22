#pragma once


/* Device Address Word consist of mandatory pattern : 1010 000X
 * If X is 0, Device Write
 * If X is 1, Device Read
 */
#define AT24CXX_EEPROM_ADDR     (0x50<<1)

#define AT24CXX_EEPROM_PAGESIZE 32
#define AT24CXX_EEPROM_MAXADDR  (4096)	//4096 Byte=32*1024 bit 
 

//-----------------------------------------------
#ifdef __K_TWI_C__
//-----------------------------------------------
// local

//-----------------------------------------------
#else
//-----------------------------------------------
// extern

void twi_init(void);


//-----------------------------------------------
#endif
//-----------------------------------------------



int at24cxx_eeprom_set_freq (int freq);
int at24cxx_eeprom_write (u16 addr, u8 *buffer, int length);
int at24cxx_eeprom_read (u16 addr, u8 *buffer, int length);


int rtcc_rd(u8 *buffer);
int rtcc_wr(u8 time_var, u8 rtcc_reg);
void ini_time(void);
void ini_rtcc(void);

void rtc_gettime(rtcTime* tm);

