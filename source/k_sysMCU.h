#pragma once


#define SYS_CMD_SENSOR_OFF			  '0' //0x00
#define SYS_CMD_SENSOR_ON			    '1' //0x01
#define SYS_CMD_SENSOR_ALL_OFF		'2' //0x02
#define SYS_CMD_SENSOR_ALL_ON		  '3' //0x03
#define SYS_CMD_SENSOR_RESET		  '4' //0x04
#define SYS_CMD_SENSOR_ALL_RESET	'5' //0x05
#define SYS_CMD_IRIDIUM_EXT_OFF		'6' //0x06
#define SYS_CMD_IRIDIUM_EXT_ON		'7' //0x07
#define SYS_CMD_CPU_RESET			    '8' //0x08

// sensor number
  #define SYS_SENSOR_IRI1     '0' //0x00
  #define SYS_SENSOR_IRI2     '1' //0x01
  #define SYS_SENSOR_AIO			'2' //0x02
  #define SYS_SENSOR_MOSE			'3' //0x03
  #define SYS_SENSOR_PTB210   '4' //0x0C
//#define SYS_SENSOR_???      '5' //0x06
  #define SYS_SENSOR_CHK1     '6' //0x06
  #define SYS_SENSOR_ATM1			'7' //0x07
  #define SYS_SENSOR_ATM2			'8' //0x08
  #define SYS_SENSOR_CHK2     '9' //0x06
  #define SYS_SENSOR_SHOCK    'A' //0x0D
  #define SYS_SENSOR_GPS			'B' //0x09
  #define SYS_SENSOR_CT3919   'C' //0x04
  #define SYS_SENSOR_DCS      'D' //0x05
  #define SYS_SENSOR_RESERVED	'E' //0x0E
  #define SYS_SENSOR_HMP155   'F' //0x0B


void cmdSensorControl(u8 a_cmd, u8 a_sno);
void cmdSensorControl_1(u8 a_data1, u8 a_data2, u8 a_data3, u8 a_data4);

void task_sysMCU(void);
void setHeartbeatEvent(void);
void cmdSensorControl_All_ON(void);
void cmdSensorControl_All_OFF(void);
void cmdSensorControl_byEnv(void);

//-----------------------------------------------
#ifdef __K_SYSMCU_C__
//-----------------------------------------------



//-----------------------------------------------
#else
//-----------------------------------------------



//-----------------------------------------------
#endif  // #ifdef __K_SYSMCU_C__


