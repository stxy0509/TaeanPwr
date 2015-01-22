/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/
#include "adstar.h"
#include "etc_driver/AK4183.h"

#define AK4183_ADDR		0x92

#define	CMD_SLEEP		0x70
#define CMD_Driver_X	0x80
#define CMD_Driver_Y	0x90
#define CMD_Driver_YP	0xA0
#define CMD_Driver_XM	0xB0
#define	CMD_READ_X		0xC0
#define	CMD_READ_Y		0xD0
#define CMD_Z1			0xE0
#define CMD_Z2			0xF0

/*
 * ak4183 control command
 * BIT	Name	Function
 *  7	 S	Start Bit 0: Sleep mode, 1: Accelerate and Axis
 * 6-4	A2-A0	Channel Selection Bits
 *  3	X1	Don't care
 *  2	PD0	Power down
 *  1	Mode	Resolution of A/D converter. 0: 12bit, 1: 8bit
 *  0	X2	Don't care
 */


#define ERR_TWI_ACK_TIME_OVER	0xff// ACK Time Over
#define ERR_TWI_TC_TIME_OVER	0xfe// Transfrer complete Time over
#define ERR_TWI_RX_TIME_OVER	0xfd// RX full check time over

static U8 TwiCompliteCheck(U32 waite_time)
{// 0->OK, 1->Error
	U8 state =1,aa;
	
	while(waite_time){
		aa= *R_TWISTAT;
		if(aa & 0x80){
			state =0;
			break;
		}
		waite_time--;
	}
	return state;
}
static U8 TwiBusBusyCheck(U32 waite_time)
{// 0->OK, 1->Error
	U8 state =1,aa;
	
	while(waite_time){
		aa= *R_TWISTAT;
		if((aa & 0x20)==0){
			state =0;
			break;
		}
		waite_time--;
	}
	return state;
}
static U8 TwiStop(void)
{
	volatile U32 dtime;
	*R_TWICTRL =0;
	*R_TWIDATA = 0xfe;
	for(dtime=0;dtime<1000; dtime++);
	*R_TWICTRL = F_TWICTRL_TWIEN | F_TWICTRL_TWIMOD_MSTART | F_TWICTRL_TWITR_MTRM | F_TWICTRL_TWIAK_NACK ;
	for(dtime=0;dtime<1000; dtime++);
	
	return 0;
}

static char AK4183Write( U8 twi_data)
{
	if(TwiBusBusyCheck(1000)){
		TwiStop();
		if(TwiBusBusyCheck(1000)){
			TwiStop();
			return -1;
		}
	}
	*R_TWIDATA = AK4183_ADDR;// Divice ID Write
	*R_TWICTRL = F_TWICTRL_TWIEN | F_TWICTRL_TWIMOD_MSTART | F_TWICTRL_TWITR_MTRM;// start condiction
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}

	*R_TWIDATA = twi_data;// data write
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	
	// stop condiction
	*R_TWICTRL &= ~(F_TWICTRL_TWIMOD_MSTART);
	if(TwiBusBusyCheck(1000)){
		TwiStop();
		return -1;
	}

	return 0;
}

static int AK4183Read()
{
	U16 twi_data=0;

	if(TwiBusBusyCheck(1000)){
		TwiStop();
		if(TwiBusBusyCheck(1000)){
			TwiStop();
			return -1;
		}
	}

	*R_TWIDATA = AK4183_ADDR|0x01;
	*R_TWICTRL = F_TWICTRL_TWIEN | F_TWICTRL_TWIMOD_MSTART;
	
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}

	twi_data=*R_TWIDATA;
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	
	twi_data=*R_TWIDATA;
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	
	*R_TWICTRL |= F_TWICTRL_TWIAK_NACK ;
	twi_data<<=8;
	twi_data|=*R_TWIDATA;
	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}

	if(TwiCompliteCheck(1000)){
		TwiStop();
		return -1;
	}
	return twi_data>>=4;
}
#define TOUCH_MIN_X 100
#define TOUCH_MAX_X 4000

#define TOUCH_MIN_Y 80
#define TOUCH_MAX_Y 3850
#define SKIP_DIF 30

#define SAMPLES 5

static int tc_avr(int* databuf,int cnt)
{
	int i;
	int x = 0;
	int avr;
	for (i = 0; i < cnt; i++) {
		x += databuf[i];
	}
	avr = x/cnt;
	//평균보다 오차가 큰 좌표는 버리고 다시 계산
	int validcnt=0;
	x=0;
	for (i = 0; i < cnt; i++) 
	{
		if( ( (avr+SKIP_DIF) > databuf[i] ) && ((avr-SKIP_DIF) < databuf[i]) )
		{
			x += databuf[i];
			validcnt++;
		}
	}
	if(validcnt<cnt-2)
	{
		return -1;
	}
	avr = x/validcnt;
	return avr;
}
static inline int TouchReadX(void)
{
	int i, j;
	int x_raw[SAMPLES];
	int tmp;
	memset(x_raw, 0, sizeof(unsigned int)*SAMPLES);

	AK4183Write(CMD_Driver_X);
	for (i = 0,j = 0; i < SAMPLES; i++) {
		tmp = AK4183Read();
		if((tmp < TOUCH_MIN_X) || (tmp > TOUCH_MAX_X))
			continue;
		x_raw[j++] = tmp;
	}
	//if (j > SAMPLES-(SAMPLES/4)) {
	if (j>SAMPLES-2) {
		return  tc_avr(x_raw,j);
	}
	return -1;
}
static inline int TouchReadY(void)
{
	int i, j;
	int y_raw[SAMPLES];
	int tmp= 0;

	memset(y_raw, 0, sizeof(unsigned int)*SAMPLES);

	AK4183Write(CMD_Driver_Y);
	for (i = 0,j = 0; i < SAMPLES; i++) {
		tmp = AK4183Read();
		if((tmp < TOUCH_MIN_Y) || (tmp > TOUCH_MAX_Y))
			continue;
		y_raw[j++] = tmp;
	}
	if (j > SAMPLES-2) {
		return  tc_avr(y_raw,j);
	}
	return -1;
}
static inline int TouchPress1()
{
	int z;
	AK4183Write(CMD_Z1);
	z = AK4183Read();
	return z;
}
static inline int TouchPress2()
{
	int z;
	AK4183Write(CMD_Z2);
	z = AK4183Read();
	return z;
}

#define SCREEN_W 800
#define SCREEN_H 480

inline void touch2screen(int touch_x, int touch_y, int* sx, int* sy)
{
	if(touch_x<TOUCH_MIN_X)
	{
		touch_x=TOUCH_MIN_X;
	}
	else if(touch_x>TOUCH_MAX_X)
	{
		touch_x=TOUCH_MAX_X;
	}

	*sx = (touch_x-TOUCH_MIN_X)*SCREEN_W/(TOUCH_MAX_X-TOUCH_MIN_X);
	
	if(touch_y<TOUCH_MIN_Y)
	{
		touch_y=TOUCH_MIN_Y;
	}
	else if(touch_y>TOUCH_MAX_Y)
	{
		touch_y=TOUCH_MAX_Y;
	}

	*sy = SCREEN_H-(touch_y-TOUCH_MIN_Y)*SCREEN_H/(TOUCH_MAX_Y-TOUCH_MIN_Y);
}

 
/**< Circular Buffer Types */
typedef unsigned char INT8U;
typedef int KeyType;
typedef struct
{
   int writePointer; /**< write pointer */
   int readPointer;  /**< read pointer */
   int size;         /**< size of circular buffer */
   KeyType keys[0];    /**< Element of circular buffer */
   KeyType lastkey;
} CircularBuffer;
CircularBuffer* KeyQue;
 
/**< Init Circular Buffer */
static CircularBuffer* CircularBufferInit(CircularBuffer** pQue, int size)
{
        int sz = size*sizeof(KeyType)+sizeof(CircularBuffer);
        *pQue = (CircularBuffer*) malloc(sz);
        if(*pQue)
        {
            (*pQue)->size=size;
            (*pQue)->writePointer = 0;
            (*pQue)->readPointer  = 0;
        }
        return *pQue;
}
 
inline bool CircularBufferIsFull(CircularBuffer* que)
{
     return ((que->writePointer + 1) % que->size == que->readPointer);
}
 
inline bool CircularBufferIsEmpty(CircularBuffer* que)
{
     return (que->readPointer == que->writePointer);
}
 
inline bool CircularBufferEnque(CircularBuffer* que, KeyType k)
{
	if(CircularBufferIsFull(que)==true)
		return false;
	CRITICAL_ENTER();
	que->lastkey = k;
	que->keys[que->writePointer] = k;
    que->writePointer++;
    que->writePointer %= que->size;
	CRITICAL_EXIT();
    return true;
}
 
inline bool CircularBufferDeque(CircularBuffer* que, KeyType* pK)
{
	if(CircularBufferIsEmpty(que)==true)
		return false;
	CRITICAL_ENTER();
	*pK = que->keys[que->readPointer];
    que->readPointer++;
    que->readPointer %= que->size;
	CRITICAL_EXIT();
    return true;
}


inline void CircularBufferClear(CircularBuffer* que)
{
	CRITICAL_ENTER();
	que->writePointer = 0;
	que->readPointer  = 0;
	CRITICAL_EXIT();
}

bool get_touchxy_buffered(int* x, int* y)
{
	KeyType key;
	if(CircularBufferDeque(KeyQue,&key)==false)
		return false;
	*x = key>>16;
	*y = key&0xffff;
	return true;
}
static bool get_touchxy_cur(int* x, int* y)
{
	bool b = false;
	int level = *R_EINTMOD;
	if((level & (1<<3))==0) //check low level
	{
		int touch_x,touch_y;
		int z1;
		touch_x = TouchReadX();
		touch_y = TouchReadY();
		if((touch_x !=-1) && (touch_y !=-1))
		{
			z1 = TouchPress1();
			if(z1 > 10)
			{
				touch2screen(touch_x,touch_y,x,y);
				b = true;
			}
		}
	}

	return b;
}

static bool bKeyBuffered = false;
bool get_touchxy(int* x, int* y)
{
	if(bKeyBuffered)
	{
		//only buffered last data
		if(CircularBufferIsEmpty(KeyQue)==true)
			return false;
		CRITICAL_ENTER();
		//clear queue
		*x = KeyQue->lastkey>>16;
		*y = KeyQue->lastkey&0xffff;
		CRITICAL_EXIT();
		CircularBufferClear(KeyQue);
		return true;
	}
	else
	{
		return get_touchxy_cur(x,y);
	}
}


static void get_tcdata()
{
	int touch_x,touch_y;
	int x,y;
	int z1;
	touch_x = TouchReadX();
	touch_y = TouchReadY();
	if(touch_x ==-1)
	{
		return ;
	}

	if(touch_y ==-1)
	{
		return ;
	}
	z1 = TouchPress1();
	if(z1 < 10)
	{
		return ;
	}
	touch2screen(touch_x,touch_y,&x,&y);
	CircularBufferEnque(KeyQue,x<<16|y);
}

//--------------------------------------------------------------
static void irq_isr(void)
{
#if 0
	while(1)
	{
		int level = *R_EINTMOD;
		if(level & (1<<3))
			break;
		get_tcdata();
	}
#else
	int level = *R_EINTMOD;
	if((level & (1<<3))==0) //check low level
	{
		get_tcdata();
	}
#endif
}

void touchinit()
{
	*R_PAF4 &= 0xF0FC;
	*R_PAF4 |= F_PAF4_0_EIRQ0|F_PAF4_4_TWI_SCL|F_PAF4_5_TWI_SDA; 
	// *R_EINTMOD = F_EINTMOD_0MOD_REDGE;
	*R_EINTMOD = F_EINTMOD_0MOD_LOW;
	set_interrupt(INTNUM_EIRQ0,irq_isr);
	CircularBufferInit(&KeyQue, KEY_BUFFER_SIZE);
}

bool enable_touch_bufferd_mode(bool b)
{
	bool oldmode = bKeyBuffered;
	if(b)
	{
		bKeyBuffered = true;
		enable_interrupt(INTNUM_EIRQ0,TRUE);
	}
	else
	{
		bKeyBuffered = false;
		if(KeyQue)
			CircularBufferClear(KeyQue);
		enable_interrupt(INTNUM_EIRQ0,FALSE);
	}
	return oldmode;
}

