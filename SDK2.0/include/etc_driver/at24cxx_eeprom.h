/*
 */

#pragma once

int at24cxx_eeprom_set_freq (int freq);
int at24cxx_eeprom_write (u16 addr, u8 *buffer, int length);
int at24cxx_eeprom_read (u16 addr, u8 *buffer, int length);


/* Device Address Word consist of mandatory pattern : 1010 000X
 * If X is 0, Device Write
 * If X is 1, Device Read
 */
#define AT24CXX_EEPROM_ADDR     (0x50<<1)

#define AT24CXX_EEPROM_PAGESIZE 32
#define AT24CXX_EEPROM_MAXADDR  (4096)	//4096 Byte=32*1024 bit 



