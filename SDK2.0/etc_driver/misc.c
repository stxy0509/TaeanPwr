
#include "adstar.h"

#define TEST_TIMER 3
// use last timer
#define MY_PRESCALE 32768 

void StartEstimate()
{
	U32 conval;
#if (MY_PRESCALE==32768)
	conval = 7<<1;
#elif (MY_PRESCALE==8192)
	conval = 6<<1;
#elif (MY_PRESCALE==2048)
	conval = 5<<1;
#elif (MY_PRESCALE==512)
	conval = 4<<1;
#elif (MY_PRESCALE==128)
	conval = 3<<1;
#elif (MY_PRESCALE==32)
	conval = 2<<1;
#elif (MY_PRESCALE==8)
	conval = 1<<1;
#elif (MY_PRESCALE==2)
	conval = 0;
#else
#error invalid MY_PRESCALE
	debugstring("Error MAX_TIMER_PRESCALE\r\n");
	return FALSE;
#endif
	//debugstring("StartEstimate function use timer channel 3\r\n");
	//reset
	*(vU32*)TPCTRL_ADDR(TEST_TIMER) = 1<<1;
	*(vU32*)TPCTRL_ADDR(TEST_TIMER) = 0;
	
	*(vU32*)TMCNT_ADDR(TEST_TIMER)= 0xffff;//max
	*(vU32*)TMCTRL_ADDR(TEST_TIMER) = conval|1;
}

U32 EndEstimate(char* str)
{
	*(vU32*)TMCTRL_ADDR(TEST_TIMER) = 0;;//stop
	U32 upcnt = *(vU32*)TMCNT_ADDR(TEST_TIMER);
	U32 ms = (upcnt*1000)/(get_apb_clock()/MY_PRESCALE);
	if(str!=0)
		DEBUGPRINTF("%s : %d ms\r\n",str,ms);
	return ms;

}

void printmem(void* buf, DWORD len)
{
	U32 i;
	U32 k;
	U32 addr = (U32)buf;
	BYTE* pbuf = (BYTE*)buf;
	debugstring("         ");
	for(i=0;i<0x10;i++)
		DEBUGPRINTF("\t %X ",i);
	debugstring("\r\n");
	for(i=0;i<len;)
	{
		DEBUGPRINTF("%08X:",(addr+i)&(~0xf));
		for(k=0;k<((addr+i)%16);k++)
			debugstring("   ");
		for(;k<16;k++)
		{
			DEBUGPRINTF("%02X ",pbuf[i]);
			i++;
			if(!(i<len))
				break;
		}
		debugstring("\r\n");
	}
}

