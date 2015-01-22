// File Name    : c_i2c_rtcc_mcset_p18f87j11_b06.c 
// RTCC = MC79410, PIC18 family, C language. 
// Dependencies : p18f87j11.h 
// Processor    : PIC18F87J11   
// Hardware     : EVB  =  PIC18 EXPLORER
//                RTCC =  MCP79410  
// I.D.E.       : MPLAB + C18 compiler   
// Company      : Microchip Technology , Inc. 
// Author       : Alexandru Valeanu 
//...........................................................................
//                   SOFTWARE  LICENSE AGREEMENT    
//...........................................................................
// "Microchip Technology Inc." ("Microchip") licenses this software to you 
// solely for use with Microchip's Serial RTCC products.  
// The software is owned by Microchip and/or its licensors, and is protected 
// under applicable copyright laws.  All rights reserved.
// SOFTWARE IS PROVIDED "AS IS."  MICROCHIP AND ITS LICENSOR EXPRESSLY 
// DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING 
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
// FOR A PARTICULAR PURPOSE,OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP 
// AND ITS LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR 
// CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, 
// COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY 
// CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE 
// THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS."
//***************************************************************************
// History      :  V1.0 - Initial Release 
//...........................................................................
// File Description : This is the file for ANxxxx                    
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
//          HARDWARE  SCHEMATIC : EVB = PIC18 EXPLORER  
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ 
//        ___________________      ___________      __________________
//   _--_|          RC3/SCK1 |--->|SCK        |--->|DB0-DB7           |
//  | S1 |RB0  SPI  RC5/SDO1 |--->|SDI        |--->|RS      LCD       |
// _|_   |                   |    |  MCP23S17 |--->|E      LUMIX      |
//       |               RA2 |--->|/CS        |    |                  | 
//       |   PIC18F87J11     |    |___________|    |__________________|
//       |                   |     _______________
//   _--_|RA5       RC4/SDA1 |<-->|SDA RTCC  Vbat |__     S1 = MENU KEY            
//  | S2 |     I2C  RC3/SCL1 |--->|SCL MCP79410   | _|_   S2 = INCR KEY
// _|_   |         RA4/T0CKI |<---|MFP            | --- BAT 
//       |___________________|    |_______________| _|_   (SDA/SCL have pull-ups) 
//                                                        (S1 /S2  have pull-ups)  
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
//    APPLICATION'S  DESCRIPTION (ELECTRONIC WATCH : DISPLAY + SETUP) 
//............................................................................
// The firmware creates a list of drivers usefull to interface 
// the PIC18 Explorer board with MCP79410 I2C RTCC. 
// The presented drivers are: 
// - SPI basic    - SPI expander (MCP23S17) 
// - LCD (LUMIX)  - keyboard (S1 = MENU & S2 = INCREMENT) 
// - I2C basic    - I2C RTCC  - Interrupts based on TMR0(counter input = MFP).
// Moreover, the application displays on the LCD all 6 time/date variables:
// year, month, date, hour, minutes, seconds, using interrupts generated
// by TMR0's overflow, according to MFP which is connected to the counter
// input of TMR0.
// The main function performs the setup of the above variables, using the 
// 2 onboard push-buttons: S1 = MENU KEY ; S2 = INCREMENT KEY.
// The setup menu is entered through the MENU KEY, which stops interrupts.
// Every action on the MENU KEY shifts the setup to the next variable. 
// The LCD cursor points to the variable that must be set. 
// Every action of INCR KEY will increment the related variable. 
// Once the menu is exit(after 'seconds'), interrupts are again enabled and
// the RTCC counts again.    
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
//                         CONFIGURATION  BITS 
//.................................................................................
//    #pragma config   FOSC = HSPLL,   FCMEN = OFF, IESO = OFF   // CONFIG1H
//    #pragma config  WDTEN = OFF                                // CONFIG2H
//    #pragma config  XINST = OFF                                // CONFIG4L 
// //.................................................................................
//    #include "p18f87j11.h"              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                    GLOBAL CONSTANTS RTCC - ADDRESSES 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     #define  ADDR_EEPROM_WRITE 0xae       //  DEVICE ADDR for EEPROM (writes)   
     #define  ADDR_EEPROM_READ  0xaf       //  DEVICE ADDR for EEPROM (reads)  
     #define  ADDR_RTCC_WRITE   0xde       //  DEVICE ADDR for RTCC MCHP  (writes) 
     #define  ADDR_RTCC_READ    0xdf       //  DEVICE ADDR for RTCC MCHP  (reads) 
//.................................................................................  
     #define  SRAM_PTR          0x20       //  pointer of the SRAM area (RTCC) 
     #define  ADDR_EEPROM_SR    0xff       //  STATUS REGISTER in the  EEPROM
//.................................................................................
     #define  ADDR_SEC          0x00       //  address of SECONDS      register 
     #define  ADDR_MIN          0x01       //  address of MINUTES      register 
     #define  ADDR_HOUR         0x02       //  address of HOURS        register 
     #define  ADDR_DAY          0x03       //  address of DAY OF WK    register 
     #define  ADDR_STAT         0x03       //  address of STATUS       register 
     #define  ADDR_DATE         0x04       //  address of DATE         register  
     #define  ADDR_MNTH         0x05       //  address of MONTH        register 
     #define  ADDR_YEAR         0x06       //  address of YEAR         register 
     #define  ADDR_CTRL         0x07       //  address of CONTROL      register 
     #define  ADDR_CAL          0x08       //  address of CALIB        register 
     #define  ADDR_ULID         0x09       //  address of UNLOCK ID    register
//.................................................................................
     #define  ADDR_ALM0SEC      0x0a       //  address of ALARMO SEC   register 
     #define  ADDR_ALM0MIN      0x0b       //  address of ALARMO MIN   register 
     #define  ADDR_ALM0HR       0x0c       //  address of ALARMO HOUR  register 
     #define  ADDR_ALM0CTL      0x0d       //  address of ALARM0 CONTR register
     #define  ADDR_ALM0DAT      0x0e       //  address of ALARMO DATE  register 
     #define  ADDR_ALM0MTH      0x0f       //  address of ALARMO MONTH register 
//................................................................................. 
     #define  ADDR_ALM1SEC      0x11       //  address of ALARM1 SEC   register 
     #define  ADDR_ALM1MIN      0x12       //  address of ALARM1 MIN   register 
     #define  ADDR_ALM1HR       0x13       //  address of ALARM1 HOUR  register 
     #define  ADDR_ALM1CTL      0x14       //  address of ALARM1 CONTR register
     #define  ADDR_ALM1DAT      0x15       //  address of ALARM1 DATE  register 
     #define  ADDR_ALM1MTH      0x16       //  address of ALARM1 MONTH register 
//.................................................................................        
     #define  ADDR_SAVtoBAT_MIN 0x18       //  address of T_SAVER MIN(VDD->BAT)
     #define  ADDR_SAVtoBAT_HR  0x19       //  address of T_SAVER HR (VDD->BAT) 
     #define  ADDR_SAVtoBAT_DAT 0x1a       //  address of T_SAVER DAT(VDD->BAT) 
     #define  ADDR_SAVtoBAT_MTH 0x1b       //  address of T_SAVER MTH(VDD->BAT) 
//..................................................................................
     #define  ADDR_SAVtoVDD_MIN 0x1c       //  address of T_SAVER MIN(BAT->VDD)
     #define  ADDR_SAVtoVDD_HR  0x1d       //  address of T_SAVER HR (BAT->VDD) 
     #define  ADDR_SAVtoVDD_DAT 0x1e       //  address of T_SAVER DAT(BAT->VDD) 
     #define  ADDR_SAVtoVDD_MTH 0x1f       //  address of T_SAVER MTH(BAT->VDD)        
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                  GLOBAL CONSTANTS RTCC - INITIALIZATION 
//..................................................................................
     #define  START_32KHZ  0x80            //  start crystal: ST = b7 (ADDR_SEC)
     #define  LP           0x20            //  mask for the leap year bit(MONTH REG)   
     #define  HOUR_12      0x40            //  12 hours format   (ADDR_HOUR)   
     #define  PM           0x20            //  post-meridian bit (ADDR_HOUR) 
     #define  OUT_PIN      0x80            //  = b7 (ADDR_CTRL) 
     #define  SQWE         0x40            //  SQWE = b6 (ADDR_CTRL) 
     #define  ALM_NO       0x00            //  no alarm activated        (ADDR_CTRL) 
     #define  ALM_0        0x10            //  ALARM0 is       activated (ADDR_CTRL)
     #define  ALM_1        0x20            //  ALARM1 is       activated (ADDR_CTRL)
     #define  ALM_01       0x30            //  both alarms are activated (ADDR_CTRL)
     #define  MFP_01H      0x00            //  MFP = SQVAW(01 HERZ)      (ADDR_CTRL)  
     #define  MFP_04K      0x01            //  MFP = SQVAW(04 KHZ)       (ADDR_CTRL)  
     #define  MFP_08K      0x02            //  MFP = SQVAW(08 KHZ)       (ADDR_CTRL)  
     #define  MFP_32K      0x03            //  MFP = SQVAW(32 KHZ)       (ADDR_CTRL)  
     #define  MFP_64H      0x04            //  MFP = SQVAW(64 HERZ)      (ADDR_CTRL)
     #define  ALMx_POL     0x80            //  polarity of MFP on alarm  (ADDR_ALMxCTL)   
     #define  ALMxC_SEC    0x00            //  ALARM compare on SEC      (ADDR_ALMxCTL)              
     #define  ALMxC_MIN    0x10            //  ALARM compare on MIN      (ADDR_ALMxCTL)   
     #define  ALMxC_HR     0x20            //  ALARM compare on HOUR     (ADDR_ALMxCTL)   
     #define  ALMxC_DAY    0x30            //  ALARM compare on DAY      (ADDR_ALMxCTL)   
     #define  ALMxC_DAT    0x40            //  ALARM compare on DATE     (ADDR_ALMxCTL)   
     #define  ALMxC_ALL    0x70            //  ALARM compare on all param(ADDR_ALMxCTL)    
     #define  ALMx_IF      0x08            //  MASK of the ALARM_IF      (ADDR_ALMxCTL)
     #define  OSCON        0x20            //  state of the oscillator(running or not)
     #define  VBATEN       0x08            //  enable battery for back-up         
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                   GLOBAL CONSTANTS LCD (LUMIX - SAMSUNG CONTROLLER)  
//.......................................................................................
//      #define N1_ROW        0x00            //  start address of DDRAM / 1 line
//      #define N2_ROW1       0x00            //  start address of DDRAM ROW1 / 2 lines 
//      #define N2_ROW2       0x40            //  start address of DDRAM ROW2 / 2 lines
// //....................................................................................... 
//      #define CLEAR         0x01            //  DDRAM=00h, cursor left(row1), I/D=1
//      #define HOME          0x02            //  DDRAM=00h, curs orig, display no shift
// //.......................................................................................
//      #define MODE          0x04            //  entry mode (inc/dec + shift display) 
//      #define SHR           0x02            //  mode = shift right 
//      #define SHIFT         0x01            //  shift entire display
// //....................................................................................... 
//      #define DISPLAY       0x08            //  open DISPLAY, CURSOR, BLINK 
//      #define D_ON          0x04            //  DISPLAY = ON 
//      #define C_ON          0x02            //  CURSOR  = ON 
//      #define B_ON          0x01            //  BLINK   = ON 
// //.......................................................................................
//      #define CD_SHIFT      0x10            //  shift cursor or (display+cursor) 
//      #define CD            0x08            //  shift display+cursor 
//      #define RSH           0x04            //  right shift
// //....................................................................................... 
//      #define FUNCTION      0x20            //  function set: data length, lines, fonts 
//      #define DL8           0x10            //  data length = 8 bits 
//      #define LINES         0x08            //  numer of lines = 1/2 (1 = 2L)
//      #define FONTS         0x04            //  type of font  
// //.......................................................................................
//      #define SET_CGRAM     0x40            //  0x40 + AC5-0 
//      #define SET_DDRAM     0x80            //  0x80 + AC6-0
//   All commands need RS=0 ; when RS=1, data will be written into the LCD 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                   GLOBAL CONSTANTS - SPI_DEMUX = MCP23S17
//.......................................................................................
//     #define IOCON         0x0a             //  CONTROL REGISTER for SPI expander
//     #define DEMUX_ADDR    0x40             //  device addr for the DEMUX (A210=000)
//     #define IODIRA        0x00             //  DIR for PORTA (0x00 for OUTPUTS)   
//     #define IODIRB        0x01             //  DIR for PORTB (0x00 for OUTPUTS) 
//     #define GPIOA         0x12             //  general purpose PORTA                          
//     #define GPIOB         0x13             //  general purpose PORTB
//     #define OLATA         0x14             //  latch for PORTA  
//     #define OLATB         0x15             //  latch for PORTB 
// //.......................................................................................
//     #define CMND_LCD_DIS  0x00             //  mask sent to PORTA (dis LCD for CMNDs) 
//     #define CMND_LCD_EN   0x40             //  mask sent to PORTA (en  LCD for CMNDs)  
//     #define DATA_LCD_DIS  0x80             //  mask sent to PORTA (dis LCD for DATA) 
//     #define DATA_LCD_EN   0xc0             //  mask sent to PORTA (en  LCD for DATA)
// //.......................................................................................
//     #define ASCII_DIGIT   0x30             //  ASCII '0' (usefull to display digits) 
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// //                        GLOBAL CONSTANTS - KEYBOARD 
// //.......................................................................................
//      #define KEYB_MENU 0x01          // code for the MENU KEY (enter,exit,change cursor) 
//      #define KEYB_INCR 0x02          // code for the INCR KEY (increments values)    
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                         GLOBAL BIT VARIABLES
//.......................................................................................
     // #define  NCS_SPI  PORTAbits.RA2        // /CS for the SPI_DEMUX (MCP23S17)
     // #define  KEYB1    PORTBbits.RB0        // switch1 on RB0 
     // #define  KEYB2    PORTAbits.RA5        // switch2 on RA5 
     // #define  SQVAW    PORTAbits.RA4        // square vawe MFP from RTCC      
     // #define  T0_OVFL  INTCONbits.TMR0IF    // Timer0 overflow 
     // #define  T0_ON    T0CONbits.TMR0ON     // Timer0 start/stop
     // #define T1_OVFL   PIR1bits.TMR1IF      // overflow of TMR1  
     // #define T1_ON     T1CONbits.TMR1ON     // start/stop T1 bit
     // #define T3_OVFL   PIR2bits.TMR3IF      // Timer3 OVERFLOW 
     // #define T3_ON     T3CONbits.TMR3ON     // Timer3 start/stop              
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬ 
//                        GLOBAL  VARIABLES UNINITIALIZED
//.......................................................................................
     //#pragma udata   // un-initialized variables
     unsigned char sec, min, hr,day, dat, mon, yr ; // time variables, BCD format
     unsigned char rtcc_buf       ;   // global data buffer for the RTCC
     unsigned char err_cnt = 0x00 ;   // error counter ; slave's I2C_NACK 
     unsigned char err_flg = 0x00 ;   // error flag ; reacts at slave's NACK & MSSP1IF 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                         GLOBAL VARIABLES INITIALIZED
//.......................................................................................
    //#pragma romdata const_table                  // constant table in ROM 
    //rom const  unsigned char *tim  = "time "   ; // 'time' string  
    //rom const  unsigned char *date = "date 20" ; // 'date' string , add millenium  
    //#pragma romdata                              // end of the ROM table    
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
//                             FUNCTIONS' PROTOTYPES 
//***************************************************************************************
//   void ferror(void)                       ; // fatal error (no ACK from the I2C_slave 
//   void spi_wrbyte(unsigned char spi_data) ; // streams out a byte on the SPI bus
//   void wrcmnd_lcd(unsigned char cmnd_lcd) ; // writes a command byte to the LCD
//   void wrdata_lcd(unsigned char data_lcd) ; // writes a data byte to the LCD
//   void wrstr_lcd(const rom unsigned char *str_lcd); // writes a string(message) to the LCD
// //.......................................................................................   
  // void ini_gpio_led(void)  ;    // init PORTD to display on LEDs 
  // void ini_gpio_other(void);    // RA5=S2(input),RB0=S1(input) = onboard switches
  //                               // RA2=NCS_SPI = /CS for the SPI_DEMUX, RA4 = MFP 
  // void ini_tim0(void)      ;    // init Timer0=counter: stop, no prescaler, clear overflow
  //                               // TMRO is used as counter(counts seconds on MFP signal)
  // void ini_tim1(void)      ;    // init Timer1: 16b, no ext osc, int osc, no psc, stop
  //                               // TMR1 is used to create delays used by the interrupt func
  // void ini_tim3(void)      ;    // init Timer3: 16b, int osc, psc=8, stop 
  //                               // TMR3 is used to create delays in the main function(KEYB) 
  // void ini_spi1(void)      ;    // init SPI module on MSSP1 
  void ini_i2c1(void)      ;    // init I2C module on MSSP1
//   void stop_mssp1(void)    ;    // stop MSSP1 module (SPI or I2C)   
//   void ini_spidemux(void)  ;    // init SPI DEMUX chip (SPI expander)
//   void clr_lcd(void)       ;    // clears the LCD   
//   void ini_lcd(void)       ;    // init the LCD module (LUMIX)
// //.......................................................................................
//   void dlt1(unsigned int steps);// basic delay in steps(100 nsec) based on T1 
//                                 // for a 10Mhz bus speed    : max_delay = 06.55 ms
//   void dlt3(unsigned int steps);// 10Mhz:8=1.25Mhz(0.8usec) : max_delay = 52.43 ms   
//   void dly1_5ms(void) ;         // used in some LCD commands (TMR1)
//   void dly39us(void)  ;         // used in some LCD commands (TMR1)
//   void dly43us(void)  ;         // used in some LCD commands (TMR1)          
//   void dly5ms(void)   ;         // write cycle time through delays, instead polling(EEPROM)
//   void dly100ms(void) ;         // used in keyboards functions      (TMR3) 
//   void dly1s(void)    ;         // 1       second delay = 4 * 250ms (TMR3) 
// //......................................................................................
//   void poll_if_tim1(void);      // timeout for MSSP1IF, in case of removing the PICTAIL
//   void i2c_start(void)  ;       // I2C   START command
  void i2c_restart(void);       // I2C RESTART command (for reads) 
  void i2c_stop(void)   ;       // I2C    STOP command 
  void i2c_wr(unsigned char i2c_data); // I2C WRITE BYTE
  void i2c_rd(void)     ;       // I2C READ BYTE (returns 'rtcc_buf') 
  void i2c_ack(void)    ;       // MASTER ACK 
  void i2c_nack(void)   ;       // MASTER NOACK        
  void ini_rtcc(void)   ;       // initialization of the RTCC. (MCP79410)
  void ini_time(void)   ;       // initialize time/date variables 
  void rtcc_wr(unsigned char time_var, unsigned char rtcc_reg) ; 
                                // writes a byte from 'time_var' in a RTCC register  
 unsigned char rtcc_rd(unsigned char rtcc_reg) ;  
                                // results of reads are temporary stored in 'rtcc_buf' 
//.....................................................................................
// Functions to display time variables on the LCD are specific for every
// time variable, because of specific masks in RTCCs registers. 
// Below functions will convert a double BCD byte in 2 ASCII bytes.
   // void sec_to_lcd(void) ;      // displays SECONDS   
   // void min_to_lcd(void) ;      // displays MINUTES 
   // void  hr_to_lcd(void) ;      // displays HOURS 
   // void dat_to_lcd(void) ;      // displays DATE 
   // void mon_to_lcd(void) ;      // displays MONTH 
   // void  yr_to_lcd(void) ;      // displays YEAR
//..................................................................................
   // unsigned char keyb_press(void)   ;  // waits a key (if not, loops for ever) 
   //                                     // returns 2 codes : MENU KEY / INCR KEY
   // void back_lcd(unsigned char pos) ;  // returns back cursor with 'pos' positions(LCD)
   // void del_lcd(unsigned char pos)  ;  // deletes back 'pos' positions(LCD)   
   void incr_yr(void)               ;  // increments 'year'    variable (watch setup)   
   void incr_mon(void)              ;  // increments 'month'   variable (watch setup) 
   void incr_dat(void)              ;  // increments 'date'    variable (watch setup) 
   void incr_hr(void)               ;  // increments 'hour'    variable (watch setup) 
   void incr_min(void)              ;  // increments 'minute'  variable (watch setup) 
   void incr_sec(void)              ;  // increments 'seconds' variable (watch setup) 
//.......................................................................................
//    void display_time(void)          ;  // displays time variables, when the MENU is not 
//                                        // entered.(in the interrupt function) 
// //.......................................................................................
//    void ini_intr(void)              ;  // initialize  interrupts (on TMR0) 
//    void start_intr(void)            ;  // enable      interrupts (on TMR0) 
//    void stop_intr(void)             ;  // disable     interrupts (on TMR0) 
//    void InterruptServiceHigh(void)  ;  // declare the interrupt service routine 
//    #pragma code InterruptVectorHigh = 0x08 // declare InterruptVector, high priority(TMR0)
//    void InterruptVectorHigh(void) 
//    { _asm 
//      goto InterruptServiceHigh         // jump to the interrupt routine     
//     _endasm }                                                  
//**************************************************************************************
//                         FUNCTIONS' DEFINITIONS 
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
     void ferror(void)        // fatal error (used on I2C_NOACK from the slave)
   {  err_flg = 0x01    ;     // if error, set error flag (NOACK from slave) 
      LATD = err_cnt ;  }     // LEDs = error counter     
//......................................................................................
//                     LOW LEVEL INITIALIZATION FUNCTIONS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                      GPIOs : LEDs + SWITCHES 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//     void ini_gpio_led(void)  
//   { TRISD = 0x00 ; LATD = 0x00 ; }   // PORTD = OUT, LEDs = OFF  
// //.......................................................................................
//    void ini_gpio_other(void) // RA5=S2(input),RB0=S1(input) = onboard switches
//                              // RA2=NCS_SPI = /CS for the SPI_DEMUX, RA4 = MFP(TMR0 input)  
//    { TRISB = 0xff         ;  // RB0=S1 = input ( all inputs ) 
//      WDTCONbits.ADSHR = 1 ;  // switch to the alternate bank of SFRs 
//      ANCON0 = 0x1f        ;  // AN4 - AN0 = digital inputs (PORTA) 
//      WDTCONbits.ADSHR = 0 ;  // switch to the default   bank of SFRs 
//      TRISA = 0xfb         ;  // RA2 = OUT = NCS_SPI ; all other pins = IN 
//      NCS_SPI = 1      ;   }  // rise-up the chip_select signal(SPI DEMUX chip)
// //................................................................................
//    void ini_tim0(void)       // TMR0 initialization 
//    { T0CON = 0x28     ;      // TMR0 stopped, prescaler by-passed, T0=counter   
//      T0_OVFL = 0      ;  }   // clear overflow flag
// //................................................................................
//    void ini_tim1(void)       // TMR1 initialization 
//    { T1CON   = 0xc0   ;      // 16b, no ext osc, internal osc, no psc, stop timer 
//      T1_OVFL = 0      ;  }   // clear overflow flag ; used for delays in interrupts 
// //................................................................................
//    void ini_tim3(void)       // TMR3 initialization 
//   { T3CON   = 0xb0      ;    // 16b, internal osc, psc=8, stop timer 
//     T3_OVFL = 0     ;   }    // clear overflow flag 
// //................................................................................
//    void ini_spi1(void)      // init the SPI module on MSSP1
//    { TRISC = 0xd7    ;      // C4 = SDI1 = SDA1 = in
//                             // C5 = SDO1 = out,   C3 = SCK1 = SCL1 = out 
//      SSP1STAT = 0x40 ;      // CKE = 1          ; CK=iddle_low, shift on CK+
//      SSP1CON1 = 0x22 ;   }  // CKP = 0, FOSC/64 ; see the SPI_expander's datasheet
// //..................................................................................                                             
//    void ini_i2c1(void)      // init the I2C module on MSSP1
//    { TRISC    = 0xff  ;     // all inputs ; SDA & SCL = inputs 
//                             // TRISC=0xdf/d8 ; SCL=C3 , SDA=C4 = IN ; SDO(SPI)=C5=OUT 
// //   SSP1ADD  = 0x63  ;     // baud rate = 100 khz ; 10mhz:100 = 100 khz
//      SSP1ADD  = 0xc7  ;     // baud rate =  50 khz   
//      SSP1STAT = 0x80  ;     // slew rate dis, SMBUS disabled  
//      SSP1CON1 = 0x28  ;     // enable module, I2C master SSP1ADD=baud rate
//      SSP1CON2 = 0x00  ;  }  // clear all control bits                       
// //..................................................................................
//    void stop_mssp1(void) 
// { SSP1CON1bits.SSPEN = 0 ; }// disable MSSP1 module                             
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
//                        TIMER BASED DELAY FUNCTIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//    void dlt1(unsigned int steps)   // delay based on TMR1 (used in the interrupt func)
//                                    // basic delay in steps(100 nsec) ; see osc set
//                                    // for a 10Mhz bus speed : max_delay = 6.55ms 
//  {                                 // load the 16b delay value ( in steps ) 
//       TMR1H = ~(steps/256)&0xff;   // first MSB (complement, because timer1++) 
//       TMR1L = ~(steps%256)&0xff;   //       LSB (complement, because timer1++)   
//       T1_ON = 1             ;      // start counter : bus_ck = 10 Mhz, no prescaler  
//   while(!T1_OVFL)           ;      // wait T1_OVFL to rise-up 
//       T1_ON = 0             ;      // stop CNT (no ck for the timer)
//       T1_OVFL = 0           ;  }   // clear overflow flag  
// //...............................................................................
//    void dlt3(unsigned int steps)   // delay based on TMR3 ; used in the main function
//                                    // basic delay in steps(800 nsec) ; see osc set
//                                    // for a 10Mhz bus speed : max_delay = 52.43 msec  
//  {                                 // load the 16b delay value ( in steps ) 
//       TMR3H = ~(steps/256)&0xff;   // first MSB (complement, because timer3++) 
//       TMR3L = ~(steps%256)&0xff;   //       LSB (complement, because timer3++)    
//       T3_ON = 1             ;      // start counter : bus_ck = 10 Mhz, psc = 8  
//   while(!T3_OVFL)           ;      // wait T3_OVFL to rise-up
//       T3_ON = 0             ;      // stop CNT (no ck for the timer)
//       T3_OVFL = 0           ;  }   // clear overflow flag  
// //...............................................................................
//  void dly1_5ms(void)               // 1.5 msec delay, based on TMR1
// {   dlt1(15300)     ;      }       //  delay used in some LCD commands 
// //...............................................................................
//  void dly39us(void)                // 39 usec   delay, based on TMR1 
// {   dlt1(390)       ;      }       //  delay used in some LCD commands 
// //...............................................................................
//  void dly43us(void)                // 43 usec   delay, based on TMR1  
// {   dlt1(430)       ;      }       // delay used in some LCD commands 
// //...............................................................................
//  void dly5ms(void)                 // write cycle time through delays,(for EEPROM)
// { dlt3(6250)             ;   }     // instead of polling. Mainly, used in this app 
//                                    // to create long delays : 100ms, 1 sec. (TMR3)  
// //...............................................................................
//  void dly100ms(void)               // 100 msec delay, based on TMR3 
// { unsigned char k = 20    ;        //  20 x 5 msec = 100 msec 
//   while(--k) { dly5ms() ; }  }     //  used in keyb functions, to avoid hazard   
// //...............................................................................
//  void dly1s(void)                  //  1 sec delay, based on TMR3 
// { unsigned int k = 200    ;        //  200 x 5 msec = 1 sec 
//   while(--k) { dly5ms()   ; } }    //  used in messages displayed on LEDs . 
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
//                     BASIC COMMUNICATION FUNCTIONS 
//................................................................................
//    void spi_wrbyte(unsigned char spi_data) // writes a byte on the SPI bus (for LCD)
//  { SSP1BUF = spi_data       ;          // load data buffer 
//    while(!PIR1bits.SSP1IF)  ;          // poll the SPI interrupt flag 
//    PIR1bits.SSP1IF = 0      ;    }     // clear the flag after end of transmission                              
// //................................................................................ 
//    void wrcmnd_lcd(unsigned char cmnd_lcd)  // WRITES A COMMAND BYTE IN THE LCD 
//  { NCS_SPI = 0                    ;    // assert /CS of the SPI_DEMUX chip
//    spi_wrbyte(DEMUX_ADDR)         ;    // send on the line the device address(DEMUX)
//    spi_wrbyte(GPIOB)              ;    // write the address of PORTB 
//    spi_wrbyte(cmnd_lcd)           ;    // send to PORTB the desired command 
//    NCS_SPI = 1                    ;    // end command byte transmission 
//                                        // PORTB = command for the LCD 
//                                        // need to write it in the LCD through 
//                                        // RS & EN signals on PORTA
//    dly39us()                      ;    // short delay between NCS pulses 
//    NCS_SPI = 0                    ;    // assert /CS of the SPI_DEMUX chip 
//    spi_wrbyte(DEMUX_ADDR)         ;    // send on the line the device address(DEMUX)
//    spi_wrbyte(GPIOA)              ;    // write the address of PORTA
//    spi_wrbyte(CMND_LCD_EN)        ;    // enable  command writes in LCD(RS=0;EN=1)
//    NCS_SPI = 1                    ;    // end command transmission 
//    dly39us()                      ;    // short delay between commands 
//    NCS_SPI = 0                    ;    // activate /CS signal of the SPI expander 
//    spi_wrbyte(DEMUX_ADDR)         ;    // launch the device address (SPI EXPANDER) 
//    spi_wrbyte(GPIOA)              ;    // send PORTA's address 
//    spi_wrbyte(CMND_LCD_DIS)       ;    // disable writes command for the LCD(RS=0;EN=0)
//    NCS_SPI = 1                    ;    // end spi writes 
//    dly39us()                  ;   }    // short delay after command  
// //....................................................................................
//   void wrdata_lcd(unsigned char data_lcd)  // WRITES A DATA BYTE IN THE LCD 
//  { NCS_SPI = 0                    ;    // assert /CS of the SPI_DEMUX chip
//    spi_wrbyte(DEMUX_ADDR)         ;    // send on the line de device address(DEMUX)
//    spi_wrbyte(GPIOB)              ;    // write the address of PORTB 
//    spi_wrbyte(data_lcd)           ;    // send to PORTB the desired data byte 
//    NCS_SPI = 1                    ;    // end data byte transmission 
//                                        // PORTB = data byte for the LCD 
//                                        // need to write it in the LCD through 
//                                        // RS & EN signals on PORTA 
//    dly39us()                      ;    // short delay between NCS_SPI pulses
//    NCS_SPI = 0                    ;    // assert /CS of the SPI_DEMUX chip 
//    spi_wrbyte(DEMUX_ADDR)         ;    // send on the line the device address(DEMUX)
//    spi_wrbyte(GPIOA)              ;    // write the address of PORTA
//    spi_wrbyte(DATA_LCD_EN)        ;    // enable  data writes in LCD(RS=0;EN=1)
//    NCS_SPI = 1                    ;    // end enable command 
//    dly39us()                      ;    // short delay between writes 
//    NCS_SPI = 0                    ;    // activate again /CS of the SPI expander 
//    spi_wrbyte(DEMUX_ADDR)         ;    // launch the device address 
//    spi_wrbyte(GPIOA)              ;    // send PORTA address 
//    spi_wrbyte(DATA_LCD_DIS)       ;    // disable data writes in LCD(RS=0;EN=0)
//    NCS_SPI = 1                    ;    // stop data writes  
//    dly43us()                  ;   }    // short delay after data writes, end writes     
// //..................................................................................
//  void wrstr_lcd(const rom unsigned char *str_lcd)// writes a string(message) in the LCD
//   {unsigned char k = 0            ;    // char counter 
//    while(str_lcd[k]!='\0')             // till the end of the string  
//   {wrdata_lcd(str_lcd[k]) ; k++ ; } }  // launch string's char (includes 43us delay)  
// //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
// //         HIGH LEVEL INITIALIZATION FUNCTIONS (SPI DEMUX & LCD) 
// //.................................................................................
//   void ini_spidemux(void)          // init the SPI EXPANDER chip 
//   { NCS_SPI = 0             ;      // activate /CS of the SPI expander 
//     spi_wrbyte(DEMUX_ADDR)  ;      // write DEMUX  address 
//     spi_wrbyte(IODIRA)      ;      // write IODIRA address (direction reg for PORTA)  
//     spi_wrbyte(0x00)        ;      // GPIOA = OUT 
//     NCS_SPI = 1             ;      // end IODIRA writes 
//     dly39us()               ;      // short delay between NCS pulses 
//     NCS_SPI = 0             ;      // start IODIRB programming 
//     spi_wrbyte(DEMUX_ADDR)  ;      // send SPI EXPANDER address 
//     spi_wrbyte(IODIRB)      ;      // send IODIRB address (direction register for PORTB) 
//     spi_wrbyte(0x00)        ;      // GPIOB = OUT 
//     NCS_SPI = 1             ;      // end IODIRB writes 
//     dly39us()               ;      // short delay between commands 
//     NCS_SPI = 0             ;      // start GPIOA programming   
//     spi_wrbyte(DEMUX_ADDR)  ;      // write DEMUX  address 
//     spi_wrbyte(GPIOA)       ;      // write GPIOA address 
//     spi_wrbyte(0x00)        ;      // GPIOA = 00h  
//     NCS_SPI = 1             ;      // end GPIOA writes 
//     dly39us()               ;      // short delay between NCS pulses 
//     NCS_SPI = 0             ;      // start GPIOB programming 
//     spi_wrbyte(DEMUX_ADDR)  ;      // send DEMUX address 
//     spi_wrbyte(GPIOB)       ;      // send GPIOB address 
//     spi_wrbyte(0x00)        ;      // GPIOB = 00h 
//     NCS_SPI = 1             ;      // end GPIOB writes 
//     dly39us()           ;   }      // short delay at the end of the command 
// //...............................................................................
//   void clr_lcd(void)               // clears the LCD, cursor left up corner
//                                    // first row, DDRAM_ADDR = 00    
//   { wrcmnd_lcd(CLEAR)       ;      // launch CLEAR command
//     dly1_5ms()              ;   }  // long delay after CLEAR command 
// //...............................................................................        
//   void ini_lcd(void)                     // LCDs initialization 
//   { clr_lcd()                          ; // clear LCD (CLEAR command)   
//   wrcmnd_lcd(FUNCTION+DL8+LINES)       ; // 8bits, 2 lines  
//   wrcmnd_lcd(MODE+SHR)                 ; // shift right cursor MODE 
//   wrcmnd_lcd(DISPLAY+D_ON+C_ON+B_ON) ; } // display,cursor,blink=ON                                 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                       I2C DRIVERS 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  void poll_if_tim1(void) {            // poll 2 flags: MSSP1IF & T1
  TMR1H = ~(10000/256)&0xff ; 
  TMR1L = ~(10000%256)&0xff ;          //  1msec delay 
  T1_ON = 1 ;                          //  start timer1
  while(!(PIR1bits.SSP1IF|T1_OVFL)) ;  // wait at least on flag to rise
  if (T1_OVFL) { err_flg = 0x01 ; }    // if timeout, set error flag 
  PIR1bits.SSP1IF = 0  ;               // in any case, clear all   
  T1_OVFL=0 ; T1_ON=0  ;          }    // necessary flags 
//................................................................................
  void i2c_start(void)             // START I2C communication 
  { SSP1CON2bits.SEN = 1    ;      // START bit (cleared by hw in the end) 
    poll_if_tim1()    ;     }      // poll MSSP1IF with 1msec timeout                                                                   
//.................................................................................                              
  void i2c_restart(void)           // RESTART I2C communication (change to 'reads') 
  { SSP1CON2bits.RSEN = 1   ;      // REPEATED START bit (cleared by hw in the end) 
    poll_if_tim1()    ;     }      // poll MSSP1IF with 1msec timeout                               
//.................................................................................                              
  void i2c_stop(void)              // STOP I2C communication 
  { SSP1CON2bits.PEN = 1    ;      // STOP bit (cleared by hw in the end) 
    poll_if_tim1()    ;     }      // poll MSSP1IF with 1msec timeout   
//..................................................................................                              
  void i2c_wr(unsigned char i2c_data) // writes a byte in the I2C SLAVE 
  { SSP1BUF = i2c_data      ;      // load char in data buffer ; start streaming
    poll_if_tim1()    ;            // poll MSSP1IF with 1msec timeout  
    if(SSP1CON2bits.ACKSTAT)       // if NOACK from slave  
    { ferror() ;  }                // call 'fatal error' function
    else {;}                }      // if a correct ACK from the slave, do nothing
//.................................................................................                              
  void i2c_rd(void)                // reads a byte on I2C bus ; returns 'rtcc_buf'  
  { SSP1CON2bits.RCEN = 1   ;      // enable I2C receive mode(RCEN=0 after 8cks by hw)
    poll_if_tim1()    ;            // poll MSSP1IF with 1msec timeout 
    rtcc_buf = SSP1BUF      ;  }   // store read result in the related buffer     
//..................................................................................                           
  void i2c_ack(void)               // ACK=0 from MASTER 
  { SSP1CON2bits.ACKDT = 0  ;      // set the MASTER ACK bit 
    SSP1CON2bits.ACKEN = 1  ;      // enable MASTER ACK sequence  
    poll_if_tim1()    ;     }      // poll MSSP1IF with 1msec timeout 
//...................................................................................                           
  void i2c_nack(void)              // NACK=1 from MASTER 
  { SSP1CON2bits.ACKDT = 1  ;      // set the MASTER NOACK bit  
    SSP1CON2bits.ACKEN = 1  ;      // enable MASTER ACK sequence 
    poll_if_tim1()     ;    }      // poll MSSP1IF with 1msec timeout 
//************************************************************************************
//                      I2C RTCC DRIVERS 
//************************************************************************************
  void rtcc_wr(unsigned char time_var, unsigned char rtcc_reg) 
                              // writes a constant/variable in a RTCC register 
 { i2c_start()             ;  // start I2C communication   
   err_cnt = 0x01          ;  // set error variable = 01
   i2c_wr(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
   err_cnt = 0x02          ;  // set error variable = 02  
   i2c_wr(rtcc_reg)        ;  // write the register's ADDRESS
   err_cnt = 0x03          ;  // set error variable = 03 
   i2c_wr(time_var)        ;  // write byte variable in the register 
   i2c_stop()              ;  // stop I2C communication
   dly39us()        ;      }  // free time STOP - START         
//.....................................................................................                       
 unsigned char rtcc_rd(unsigned char rtcc_reg)  // reads a register of the RTCC   
 { i2c_start()             ;  // start I2C communication
   err_cnt = 0x04          ;  // set error variable = 04 
   i2c_wr(ADDR_RTCC_WRITE) ;  // write DEVICE ADDR for RTCC WRITES
   err_cnt = 0x05          ;  // set error variable = 05  
   i2c_wr(rtcc_reg)        ;  // write the register ADDRESS 
   i2c_restart()           ;  // RESTART for READS 
   err_cnt = 0x06          ;  // set error variable = 06 
   i2c_wr(ADDR_RTCC_READ)  ;  // send the DEVICE ADDRESS for RTCC READS.
   i2c_rd()                ;  // read register (stored in 'rtcc_buf')
   i2c_nack()              ;  // NOACK from MASTER (last read byte)
   i2c_stop()              ;  // stop I2C communication
   dly39us()               ;  // free time STOP - START 
   return rtcc_buf    ;    }  // store the read byte in 'time_var'                                      
//...................................................................................                              
   void ini_rtcc(void)                  // initialization of the RTCC(MCP79410) 
 {                                      // START bit is located in the sec register   
  day = rtcc_rd(ADDR_DAY)   ;           // read day + OSCON bit 
                                        // keep the previous value of 'seconds'  
rtcc_wr(SQWE+ALM_NO+MFP_01H,ADDR_CTRL); // square vawe on MFP, no alarms, MFP = 1Hz 
                                        // write this init constant in CONTROL REG 
                                        // final settings are: // crystal = started,
                                        // 24H format,square vawe on MFP,no alarms, MFP=1Hz
rtcc_wr(day|VBATEN,ADDR_DAY) ;   }      // enable the battery back-up 
 //.........................................................................................                            
    void ini_time(void)              // initialization of time/date:2010_03_01 , 9am.
  {                                  // it initializes also :24H format, and START OSC.
    if((day&OSCON)==OSCON) {;}       // if oscillator = already running, do nothing. 
    else {                           // if oscillator = not running, set time/date(arbitrary)
                                     // and SART oscillator/ crystal    
    rtcc_wr(0x13,ADDR_YEAR)    ;     // initialize YEAR  register           
    rtcc_wr(0x08,ADDR_MNTH)    ;     // initialize MONTH register  
    rtcc_wr(0x28,ADDR_DATE)    ;     // initialize DATE  register  
    rtcc_wr(0x11,ADDR_HOUR)    ;     // initialize HOUR  register  
    rtcc_wr(0x00,ADDR_MIN)     ;     // initialize MIN   register  
    rtcc_wr(0x00|START_32KHZ,ADDR_SEC) ; }  //init SEC   register, set START bit 
                                      }                                   
//.......................................................................................
//                LCD HIGH LEVEL FUNCTIONS 
//.......................................................................................                         
   void sec_to_lcd(void)                // displays SECONDS
 { wrdata_lcd(((sec&(~START_32KHZ))>>4)+ASCII_DIGIT) ; 
                                        // mask the START bit, extract MS digit by shifting
                                        // right 4 times,  add the ASCII digit offset
   wrdata_lcd((sec&0x0f)+ASCII_DIGIT);} // extract LS digit, add offset    
//.......................................................................................                                         
   void min_to_lcd(void)                // displays MINUTES
 { wrdata_lcd((min>>4)+ASCII_DIGIT)  ;  // extract MS digit for minutes  
                                        // minutes have no control bits, no mask needed 
   wrdata_lcd((min&0x0f)+ASCII_DIGIT);} // extract LS digit for minutes                    
 //.......................................................................................
   void  hr_to_lcd(void)                // displays HOURS
 { wrdata_lcd(((hr&(~HOUR_12))>>4)+ASCII_DIGIT) ; // mask the 12 hours format,
                                                  //extract MS digit by shifting right 4 times 
                                                  // add the ASCII digit offset
   wrdata_lcd((hr&0x0f)+ASCII_DIGIT) ; }          // extract LS digit, add offset           
 //.......................................................................................
   void dat_to_lcd(void)                          // displays DATE
  { wrdata_lcd(((dat&0x3f)>>4)+ASCII_DIGIT) ;     // extract MS digit by shifting
                                                  // right 4 times, add ASCII offset 
    wrdata_lcd((dat&0x0f)+ASCII_DIGIT) ; }        // extract LS digit, add offset       
//........................................................................................                                           
   void mon_to_lcd(void)                          // displays MONTH
  { wrdata_lcd(((mon&0x1f)>>4)+ASCII_DIGIT) ;     // mask leap year, extract MS digit
                                                  // by shifting right 4 times 
                                                  // add  the ASCII digit offset 
    wrdata_lcd((mon&0x0f)+ASCII_DIGIT) ; }        // extract LS digit, add offset  
//.........................................................................................
   void  yr_to_lcd(void)                      // displays YEAR      
   { wrdata_lcd((yr>>4)+ASCII_DIGIT)     ;    // extract MS digit by shifting right 4 times,
                                              // add ASCII offset 
    wrdata_lcd((yr&0x0f)+ASCII_DIGIT) ;  }    // extract LS digit, add ASCII offset       
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                      KEYBOARD FUNCTION 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   unsigned char keyb_press(void)     // basic keyboard function 
   { unsigned char keyb_code = 0x00 ; // init keyboard code returned by the function    
   while(KEYB1&&KEYB2)              ; // wait a keyboard action(if not, loop) 
   if(!KEYB1){keyb_code=KEYB_MENU ; } // if MENU KEY = pressed, update keyb code = 01  
   else{ keyb_code = KEYB_INCR    ; } // if INCR KEY : code = 02 
         dly100ms() ; dly100ms()  ;   // override keyboard hazard 
   while((!KEYB1)||(!KEYB2))      ;   // wait end of action 
     return keyb_code         ;     } // returns the code only if a key was pressed
                                      // and thereafter, released. 
//........................................................................................
//                  SECONDARY LCD FUNCTIONS 
//.......................................................................................
   void back_lcd(unsigned char pos)    // turns back cursor with 'pos' positions 
   { unsigned char k = 0            ;  // loop counter 
     while(k<pos)                      // for 'pos' positions
   { wrcmnd_lcd(CD_SHIFT) ; k++ ; } }  // shift left cursor, AC-1, increment loop counter    
//........................................................................................
   void del_lcd(unsigned char pos)     // deletes back 'pos' characters  
   { unsigned char k = 0           ;   // loop counter 
     wrcmnd_lcd(DISPLAY+D_ON+B_ON) ;   // CURSOR OFF , delay after command  
     while(k<pos)                      // for 'pos' positions
   { wrcmnd_lcd(CD_SHIFT) ; k++ ;  }   // shift left cursor, AC-1, counter++
     k = 0                         ; 
     while(k<pos) 
     { wrdata_lcd(' ') ; k++  ;    }   // delete 'pos' characters  
     k = 0                         ; 
      while(k<pos)                     // for 'pos' positions
   { wrcmnd_lcd(CD_SHIFT) ; k++ ;  }   // shift left cursor, AC-1, counter++
 wrcmnd_lcd(DISPLAY+D_ON+C_ON+B_ON); } // light again the cursor  
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                INCREMENT TIME/DATE VARIABLES
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   void incr_yr(void)                // increments YEARS value (2 digits)
                                     // 99-->00 , yy-->y(y+1) , y9-->(y+1)0  
   { if(yr==0x99) { yr = 0x00 ; }    // 99-->00  
     else if ((yr&0x0f)==0x09)       // y9-->(y+1)0    
          { yr=(yr+16)&0xf0 ; }  
          else { yr++       ; }      // yy-->y(y+1)
     yr_to_lcd(); back_lcd(2) ; }    // display year value; turn back cursor, 2 positions
//.......................................................................................
   void incr_mon(void)               // increments MONTHS value 
   { switch(mon) {                   // LP bit was already masked at reads(display_time())
     case 0x09: {mon=0x10;break;}    // 09 --> 10  
     case 0x12: {mon=0x01;break;}    // 12 --> 01 
       default: {mon++   ;break;} }  // in any other case, increment MONTH 
     mon_to_lcd() ; back_lcd(2) ; }  // display month value, turn back cursor 2 positions 
//.......................................................................................
   void incr_dat(void)               // increments DATE  
   { switch(dat) {                   // DATE has no auxiliary flags  
     case 0x31: {dat = 0x01;break;}  // overflow after 31 days 
     case 0x09: {dat = 0x10;break;}  // 09 --> 10 (decimal) 
     case 0x19: {dat = 0x20;break;}  // 19 --> 20 (decimal) 
     case 0x29: {dat = 0x30;break;}  // 29 --> 30 )decimal) 
     default:   {dat++;     break;}} // if no decimal overflow, incr DATE 
     dat_to_lcd()  ; back_lcd(2) ; } // display date value, turn back cursor 2 positions  
//.......................................................................................
   void incr_hr(void)                // increments HOURS value 
   { switch(hr) {                    // none auxiliary control/status bits
     case 0x23: {hr = 0x00;break;}   // '24h' format, overflow after '23'
     case 0x19: {hr = 0x20;break;}   // 19 --> 20 
     case 0x09: {hr = 0x10;break;}   // 09 --> 10 
     default:   {hr++;     break;} } // in any other case, increment value
     hr_to_lcd()  ; back_lcd(2) ;  } // display date value 
//........................................................................................
   void incr_min(void)               // increments MINUTES ('minutes' have no aux flags)  
   { if(min == 0x59)  {min=0x00;}    // overflow after 59 minutes
     else if ((min&0x0f)==0x09)      // m9 --> (m+1)0 
          { min = (min+16)&0xf0 ; }  
          else { min++ ; }           // mm --> m(m+1) 
     min_to_lcd() ; back_lcd(2) ; }  // display new MINUTES value    
//.............................................................................                
  void incr_sec(void)                // increments SECONDS ; bit7 = ST = 1 
  { sec = sec & (~START_32KHZ)    ;  // clear START bit, to simplify increment ops 
    if(sec == 0x59)  {sec=0x00;}     // overflow after 59 seconds 
     else if ((sec&0x0f)==0x09)      // s9 --> (s+1)0 
          { sec = (sec+16)&0xf0 ; }  
          else { sec++ ; }           // ss --> s(s+1)  
    sec_to_lcd() ;  back_lcd(2) ; }  // display new SECONDS value    
//.................................................................................. 
    void display_time(void)          // DISPLAYS ALL TIME VARIABLES : 
                                     // YEAR, MONTH, DATE, HOUR, MINUTES, SECONDS 
    {  stop_mssp1()          ;       // stop MSSP1 module 
       ini_i2c1()            ;       // init I2C1  module to read RTCC 
    yr  = rtcc_rd(ADDR_YEAR) ;       // read YEAR        
    if(err_flg)                      // if error, leave fast the interrupt routine
  { stop_mssp1() ; ini_spi1();       // stop MSSP, init SPI1 module for LCD access
    return ; }                       // break function, return from interrupt 
    mon = rtcc_rd(ADDR_MNTH) ;       // read MONTH 
    if(err_flg)                      // if error, leave fast the interrupt routine
  { stop_mssp1() ; ini_spi1();       // stop MSSP, init SPI1 module for LCD access
    return ; }                       // break function, return from interrupt 
    mon = mon & (~LP)        ;       // mask the leap year bit 
    dat = rtcc_rd(ADDR_DATE) ;       // read DATE 
    if(err_flg)                      // if error, leave fast the interrupt routine
  { stop_mssp1() ; ini_spi1();       // stop MSSP, init SPI1 module for LCD access
    return ; }                       // break function, return from interrupt 
    hr  = rtcc_rd(ADDR_HOUR) ;       // read HOUR
    if(err_flg)                      // if error, leave fast the interrupt routine
  { stop_mssp1() ; ini_spi1();       // stop MSSP, init SPI1 module for LCD access
    return ; }                       // break function, return from interrupt 
    min = rtcc_rd(ADDR_MIN)  ;       // read MIN
    if(err_flg)                      // if error, leave fast the interrupt routine
  { stop_mssp1() ; ini_spi1();       // stop MSSP, init SPI1 module for LCD access
    return ; }                       // break function, return from interrupt 
    sec = rtcc_rd(ADDR_SEC)  ;       // read SEC ; once finished the RTCC's READ
    if(err_flg)                      // if error, leave fast the interrupt routine
  { stop_mssp1() ; ini_spi1();       // stop MSSP, init SPI1 module for LCD access
    return ; }                       // break function, return from interrupt 
    stop_mssp1() ; ini_spi1();       // stop MSSP, init SPI1 module for LCD access 
wrcmnd_lcd(SET_DDRAM+N2_ROW1+07) ;   // set YEAR position on the first row     
yr_to_lcd()  ;  wrdata_lcd(' ')  ;   // display YEAR  + separator    
mon_to_lcd() ;  wrdata_lcd(' ')  ;   // display MONTH + separator  
dat_to_lcd() ;                       // display DATE
wrcmnd_lcd(SET_DDRAM+N2_ROW2+07) ;   // set HOUR position on the second row   
hr_to_lcd()  ;  wrdata_lcd(' ')  ;   // display HOUR  + separator          
min_to_lcd() ;  wrdata_lcd(' ')  ;   // display MIN   + separator    
sec_to_lcd()                ;    }   // display SECONDS, end of DISPLAY function 
//....................................................................................
//                  INTERRUPT  FUNCTIONS 
//....................................................................................
   void ini_intr(void)               // initialization of the Interrupt system 
 { RCONbits.IPEN  = 1       ;        // enable priority mechanism 
   INTCON2bits.TMR0IP = 1   ;        // set TMR0 interrupt priority = high 
   INTCON = 0x80          ; }        // GIEH = 1 , TMR0IE = 0(disable TMR0 intr) 
   
   void stop_intr(void)              // disable interrupts (based on TMR0) 
 { INTCONbits.TMR0IE = 0  ; }        // disable TMR0 interrupts 

   void start_intr(void)             // enable interrupts (based on TMR0) 
 { INTCONbits.TMR0IE = 1  ; }        // enable TMR0 interrupts 

   #pragma interrupt InterruptServiceHigh // displays once/second : time/date 
    void InterruptServiceHigh(void)  // Interrupts Service Routine  
   { TMR0H = 0xff ; TMR0L = 0xff ;   // load again the time values in TMR0 
     T0_OVFL = 0                 ;   // clear the overflow flag
     PIR1bits.SSP1IF = 0         ;   //  if a previous error, clear MSSP1 flag 
     err_flg = 0x00              ;   //  clear error variable 
        LATD = 0x00              ;   //  clear error message  
     SSP1CON2bits.ACKSTAT = 0    ;   //  recover slave's error (NOACK --> ACK)  
                                     //  ini_i2c1() will come in 'display_time()        
     display_time()          ;   }   // display on LCD all 6 time variables   
//************************************************************************************
//                                MAIN FUNCTION 
//¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬¬
     #pragma code    
     void main(void)                  //  sets the time/date variables, using the 2 
                                      //  onboard keys (S1=MENU, S2 = INCREMENT)  
 {  
//     ini_gpio_led()               ;    //  PORTD     init (for LEDs) 
//     ini_gpio_other()             ;    //  PORT A,B  init (onboard switches,NCS_SPI,MFP)
//     ini_tim0()                   ;    //  init Timer0 (counting apps based on MFP) 
//     ini_tim1()                   ;    //  init Timer1 (used for LCD & I2C delays in the INTR) 
//     ini_tim3()                   ;    //  init Timer3 (used for delays in the MAIN FUNCTION)
//     OSCTUNEbits.PLLEN = 1        ;    //  enable PLL 
// //...........................................................................................
//     ini_spi1()                   ;    //  init the SPI1 module for LCD
//     ini_spidemux()               ;    //  init the SPI demux chip 
//     ini_lcd()                    ;    //  init the LCD module (for further messages)
//     wrcmnd_lcd(DISPLAY+D_ON)     ;    //  shut down CURSOR & BLINK (keep the display)  
//     wrstr_lcd(date)              ;    //  write in LCD : "date 20" 
//     wrcmnd_lcd(SET_DDRAM+N2_ROW2);    //  set cursor on the second row 
//     wrstr_lcd(tim)               ;    //  write in LCD : "time" 
// //.......................................................................................
//      stop_mssp1()                ;    //  prepare to switch MSSP1 to I2C 
//      ini_i2c1()                  ;    //  initialize the I2C1 module 
     ini_rtcc()                  ;    //  initialize MCP79410(enable VBAT). 
     ini_time()                  ;    //  at first startup, init time/date vars
                                      //  & related registers in RTCC 
                                      //  2010_03_01 , 9 am, etc. 
                                      //  it initializes also : 12H format and start OSC  
//.......................................................................................  
     TMR0H = 0xff ; TMR0L = 0xff  ;   //  init counter values for 1 step/sec                    
     T0_ON = 1                    ;   //  start counter 
     ini_intr()                   ;   //  init   TMR0 interrupts 
     start_intr()                 ;   //  enable TMRO interrupts        
     while(1) {                       //  infinite loop       
  while(keyb_press()==KEYB_INCR)  ;   //  wait Interrupts, till the MENU_KEY is pressed
  stop_intr()                     ;   //  disable interrupts after entering the setup menu 
  wrcmnd_lcd(DISPLAY+D_ON+C_ON+B_ON); //  start cursor & blink 
  wrcmnd_lcd(SET_DDRAM+N2_ROW1+07);   //  set LCD_addr for YEAR 

  while(keyb_press()==KEYB_INCR)      //  at every action of INCR KEY,
  { incr_yr()  ;  }                   //  year++ , exit only when MENU KEY = ON
  wrcmnd_lcd(SET_DDRAM+N2_ROW1+10);   //  set LCD_addr for MONTH
   
  while(keyb_press()==KEYB_INCR)      //  at every action of INCR KEY,
  { incr_mon() ;  }                   //  month++ , exit only when MENU KEY = ON
  wrcmnd_lcd(SET_DDRAM+N2_ROW1+13);   //  set LCD_addr for DATE 
 
  while(keyb_press()==KEYB_INCR)      //  at every action of INCR KEY,
  { incr_dat() ;  }                   //  date++ , exit only when MENU KEY = ON
  wrcmnd_lcd(SET_DDRAM+N2_ROW2+07);   //  set LCD_addr for HOUR
 
  while(keyb_press()==KEYB_INCR)      //  at every action of INCR KEY,
  { incr_hr()  ;  }                   //  hour++ , exit only when MENU KEY = ON
  wrcmnd_lcd(SET_DDRAM+N2_ROW2+10);   //  set LCD_addr for MINUTES

  while(keyb_press()==KEYB_INCR)      //  at every action of INCR KEY,
  { incr_min() ;  }                   //  minutes++ , exit only when MENU KEY = ON
  wrcmnd_lcd(SET_DDRAM+N2_ROW2+13);   //  set LCD_addr for SECONDS 

  while(keyb_press()==KEYB_INCR)      //  at every action of INCR KEY,
  { incr_sec() ;  }                   //  seconds++ , exit only when MENU KEY = ON
                                      //  once the SETUP MENU is exit
  wrcmnd_lcd(DISPLAY+D_ON)        ;   //  stop cursor & blink 

  stop_mssp1() ; ini_i2c1()       ;   //  switch again to I2C, SSP1CON2=0x00  
  PIR1bits.SSP1IF = 0             ;   //  if a previous error, clear MSSP1 flag 
  err_flg = 0x00                  ;   //  clear error variable
  SSP1CON2bits.ACKSTAT = 0        ;   //  recover slave's error (NOACK --> ACK)
  LATD = 0x00                     ;   //  clear LEDs (if previous error messages)    
  rtcc_wr(yr,ADDR_YEAR)           ;   //  update YEAR    value in RTCC
  rtcc_wr(mon,ADDR_MNTH)          ;   //  update MONTH   value in RTCC
                                      //  LP bit is read only; you may clear it. 
  rtcc_wr(dat,ADDR_DATE)          ;   //  update DATE    value in RTCC
  rtcc_wr(hr,ADDR_HOUR)           ;   //  update HOUR    value in RTCC                
  rtcc_wr(min,ADDR_MIN)           ;   //  update MINUTES value in RTCC
  sec = sec | START_32KHZ         ;   //  restore oscillator START bit
  rtcc_wr(sec,ADDR_SEC)           ;   //  update SECONDS value in RTCC   
  dly100ms()    ;   dly100ms()    ;   //  short delay to avoid keyboard hazard 
  start_intr()                    ;   //  enable again interrupts       
                                  }   //  end of WHILE(1) loop, wait interrupts      
//.......................................................................................        
                                 }    //  END MAIN 
    