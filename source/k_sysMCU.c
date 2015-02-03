#define __K_SYSMCU_C__

//-----------------------------------------------
// <includes>
//-----------------------------------------------

#include "k_includes.h"  
  
//-----------------------------------------------
// <macros>
//-----------------------------------------------

// command

//-----------------------------------------------
// <datas>
//-----------------------------------------------

//static u32 sysMCUflag = 0;
	/* bit		meaning
	   ---      -------
		0		hear_beat
		1		cmd 0 : Sensor OFF

	*/

static u8 msg[4];
static u8 chksum[2];		

//-----------------------------------------------
// <functions>
//-----------------------------------------------





static void sendMsg(void)
{
	int i = 0;
	u16 chk = 0;
	
	uart_putch(1,'$');
	//uart_putch(0,'$');
	for (i=0; i<4; i++)
	{
		uart_putch(1,msg[i]);
		//uart_putch(0,msg[i]);
		chk += msg[i];
	}
	chksum[0] = '0';//(u8)((chk & 0xFF00) >> 8); 
	chksum[1] = '0';//(u8)((chk & 0xFF));

	uart_putch(1,chksum[0]);
	//uart_putch(0,chksum[0]);
	uart_putch(1,chksum[1]);
	//uart_putch(0,chksum[1]);
	uart_putch(1,0x0D);
	//uart_putch(0,0x0D);

	delayms(50);
}





void cmdSensorControl(u8 a_cmd, u8 a_sno)
{
	msg[0] = a_cmd;
	msg[1] = a_sno;
	msg[2] = '0';
	msg[3] = '0';
	sendMsg();
}

void cmdSensorControl_1(u8 a_data1, u8 a_data2, u8 a_data3, u8 a_data4)
{
	msg[0] = a_data1;
	msg[1] = a_data2;
	msg[2] = a_data3;
	msg[3] = a_data4;
	sendMsg();
}


void cmdSensorControl_All_ON(void)
{
	//debugprintf("sensor power on : ALL\r\n");
#if 1
	msg[0] = SYS_CMD_SENSOR_ALL_ON;
	msg[1] = '0';
	msg[2] = '0';
	msg[3] = '0';
	sendMsg();
#endif
#if 0
	cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_IRI1);
	cmdSensorControl(SYS_CMD_SENSOR_ON, SYS_SENSOR_IRI2);

	ctrl_sensor('0', env.s0);	// AIO
	ctrl_sensor('1', env.s1);	// MOSE
	ctrl_sensor('3', env.s3);	// CT
	ctrl_sensor('5', env.s5);	// DCS
	ctrl_sensor('7', env.s7);	// GPS
	ctrl_sensor('8', env.s8);	// ATM1
	ctrl_sensor('9', env.s9);	// ATM2
	ctrl_sensor('B', env.sa);	// HMP155
	ctrl_sensor('C', env.sc);	// PTB
	ctrl_sensor('F', env.sf);	// SHOCK
#endif
}

void cmdSensorControl_byEnv(void)
{
	// ctrl_sensor('0', env.s0);	// AIO
	// ctrl_sensor('1', env.s1);	// MOSE
	ctrl_sensor('3', env.s3);	// CT
	// ctrl_sensor('5', env.s5);	// DCS
	ctrl_sensor('7', env.s7);	// GPS
	// ctrl_sensor('8', env.s8);	// ATM1
	// ctrl_sensor('9', env.s9);	// ATM2
	// ctrl_sensor('B', env.sb);	// HMP155
	// ctrl_sensor('C', env.sc);	// PTB
	// ctrl_sensor('F', env.sf);	// SHOCK

}

void cmdSensorControl_All_OFF(void)
{
	//debugprintf("sensor power OFF : ALL\r\n");
	
	msg[0] = SYS_CMD_SENSOR_ALL_OFF;
	msg[1] = '0';
	msg[2] = '0';
	msg[3] = '0';
	sendMsg();
}


static int flagHeartbeatOn = 0;
int isHeartbeatEvent(void)
{
	return (flagHeartbeatOn);
}
void setHeartbeatEvent(void)
{
	flagHeartbeatOn = 1;
}
void clearHeartbeatEvent(void)
{
	flagHeartbeatOn = 0;
}

void bounceHeartbeat(void)
{
	u32 i;

	if (1 == isHeartbeatEvent())
	{
		clearHeartbeatEvent();

		//if (OK == )
		//추후 추가조건 설정이 필요하다... 

		// toggle (p4.1)
		i = (*R_GPILEV(4))&(1<<1);
		if ( 0 == i )
		{
			*R_GPOHIGH(4) |= (1<<1);
		}
		else
		{
			*R_GPOLOW(4) |= (1<<1);
		}
	}
}


static int enableHeartbeat = 1;
void setEnableHeartbeat(int a_val)
{
	debugprintf("heartbeat(%d)\r\n", a_val);
	enableHeartbeat = a_val;
}

int getEnableHeartbeat(void)
{
	return(enableHeartbeat);
}

void task_sysMCU(void)
{
	static int idx = 999;
	char ch;

	// flush uart buffer.
	uart_getch(1, &ch);

	// heartbeat
	if (getEnableHeartbeat()==1)
		bounceHeartbeat();

	switch (idx)
	{
		case 0:

			break;

		default:
			idx = 0;
			break;
	}
}


