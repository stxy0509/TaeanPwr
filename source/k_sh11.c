#define __K_SH11_C__

#include "k_includes.h"



//#if ( MCU_G_SENSOR==1 )
//MDS_CPE_2W
// sbit poMDS_CPE_Clk      = SFR_P1^6;
// sbit pioMDS_CPE_Data    = SFR_P1^7;
//sbit poMDS_CPE_Rst = SFR_P0^3;

//------------------------------------------------------------
// define
//------------------------------------------------------------

#define ACK            1
#define NOACK          0

#define MDS_CPE_ACK            1
#define MDS_CPE_NACK           0
#define MEASURE_TEMPERATURE_COMMAND 0x03
#define MEASURE_HUMIDITY_COMMAND    0x05
#define MEASURING_TIME 1000 //mSec Rate


// #define SH_PIN_CLK_H()	(*R_GPOHIGH(6) |= (1<<3) )
// #define SH_PIN_CLK_L()	(*R_GPOLOW(6)  |= (1<<3) )

// #define SH_PIN_DATA_H()	(*R_GPOHIGH(6) |= (1<<4) )
// #define SH_PIN_DATA_L()	(*R_GPOLOW(6)  |= (1<<4) )

// #define SH_PIN_DATA_INPUT()	 *R_GPIDIR(6) |= (1<<4);
// #define SH_PIN_DATA_OUTPUT() *R_GPODIR(6) |= (1<<4);


//------------------------------------------------------------
// Function Header
//------------------------------------------------------------

void SHT7x_Transmission_Start();
u32 SHT7x_Write(unsigned char a_data);	 //read=1, write = 0
//int  SHT7x_Write(unsigned char k);
int  SHT7x_ACK_Delay_Check();
void SHT7x_Soft_Reset();
void SHT7x_Connection_Reset_Sequence();
unsigned char SHT7x_Read(int ack);
void SHT7x_init();

/*
//void MDS_CPE_transmission_start();
//void MDS_CPE_init();
unsigned char MDS_CPE_write_data(unsigned char a_data);
int MDS_CPE_ack_delay_check();
unsigned char MDS_CPE_read_data(unsigned char ack);

unsigned char MDS_CPE_start_calibration(unsigned char command);
unsigned char MDS_CPE_calibration(unsigned char command);
unsigned char MDS_CPE_end_calibration(unsigned char command);
unsigned char MDS_CPE_read_azimuth(unsigned char command);
void MDS_CPE_read_calib_data(void);
void MDS_CPE_write_calib_data(void);
*/
void Task_sh11(void);

// volatile void delay_us(unsigned char a_us);
// volatile void delay_ms(unsigned char a_ms);

//for var.
//xdata U8 calib_data[10];// = {0x01,0x92,0x02,0xbb,0x09,0xea,0x0a,0x36,0x00,0xf8};
//code U8 calib_data_dflt[10] = {0x01,0x92,0x02,0xbb,0x09,0xea,0x0a,0x36,0x00,0xf8};
volatile unsigned int start_calibration_data;
volatile int start_calibration_flag;

volatile unsigned int calibraion_data;
volatile int calibration_flag;

volatile unsigned int end_calibration_data;
volatile int end_calibration_flag;

volatile unsigned int azimuth_data;
volatile unsigned int azimuth_data0;
volatile unsigned int azimuth_data1;
volatile unsigned int azimuth_data_clone;

//volatile unsigned char FND_display_data[4];

volatile int NEXT_flag;
volatile int STEP_flag;
volatile unsigned char key_scan_value;
//volatile unsigned char az_data[4];

volatile U8 idx_Task;
int fgMax_complete;
int fgAzimuth_complete;

U16 tm_delay;

typedef struct
{
    float temp_1;
    float humi_1;
    U16 tm30sec_for_send;
    U16 temp_0;
    U16 humi_0;
    U8 inetia;
    U8 temp_humi_ok;
}gv_t;
gv_t gv;
//gv_t gv;

struct
{
    unsigned int used_shock_data: 1;
    unsigned int req_send: 1;
}fg1;



//계산용 상수 테이블 선언
static const float c1 = -2.0468;           // V4 : -2.0468          V3 : -4.0000
static const float c2 =  0.0367;           // V4 :  0.0367          V3 : 0.0405
static const float c3 = -0.000001595;        // V4 : -0.000001595     V3 : -0.0000028
static const float t1 = 0.01;
static const float t2 = 0.00008;
static const float d1 = -39.65;
static const float d2 = 0.01;

//처리용 지역 변수 선언
u32 error = 0;
u16 temperature = 0;
u16 humidity = 0;
float calculated_temperature = 0.0;
float calculated_humidity_linear = 0.0;
float calculated_humidity_true = 0.0;


volatile float avg_temperature = 0.0,      avg_temperature_arr[3] ,
             previous_temperature = 0.0, curr_avg_temperature = 0.0;

volatile int   avg_temperature_count = 0;

volatile float avg_humidity = 0.0,      avg_humidity_arr[3] ,
             previous_humidity = 0.0,  curr_avg_humidity = 0.0;

volatile int   avg_humidity_count = 0;

static float valid_temp = 0.0f;
static float valid_humi = 0.0f;
float getShtTemp(void)	{	return(valid_temp);}//calculated_temperature);		}
float getShtHumi(void)	{	return(valid_humi);}//calculated_humidity_true);	}

//void ziro_make_send_data(void);

// float d1C;
// float d2C;
// float C1;
// float C2;
// float C3;
// float t1;
// float t2;

//float RH_linear;


//#endif


//U16 tm_reset_2min;

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then waits for SMBus
// communication.
//
// void main (void)
/*
void main_init (void)
{


// #if ( MCU_G_SENSOR==1 )
    // d1C = -39.7;
    // d2C = 0.01;
    // C1 = -2.0468;
    // C2 = 0.0367;
    // C3 = -0.0000059;
    // t1 = 0.01;
    // t2 = 0.00008;


    gv.temp_humi_ok = 0;

    tm_delay = (1000*2)/2; //10sec

    start_calibration_data = 0;
    start_calibration_flag = 0;

    calibraion_data = 0;
    calibration_flag = 0;

    end_calibration_data = 0;
    end_calibration_flag = 0;

    azimuth_data = 0;
    azimuth_data_clone = 0;

    //FND_display_data[4];

    NEXT_flag = 0;
    STEP_flag = 0;
    key_scan_value = 0;
    //az_data[4];

// #endif  //#if defined (MCU_G_SENSOR)


    //MDS_CPE_init();


}
*/


//------------------------------------------------------------
void SHT7x_main()
//------------------------------------------------------------
{
    // //계산용 상수 테이블 선언
    // const float c1 = -4.0000;           // V4 : -2.0468          V3 : -4.0000
    // const float c2 =  0.0405;           // V4 :  0.0367          V3 : 0.0405
    // const float c3 = -0.0000028;        // V4 : -0.000001595     V3 : -0.0000028
    // const float t1 = 0.01;
    // const float t2 = 0.00008;
    // const float d1 = -40.1;
    // const float d2 = 0.01;

    // //처리용 지역 변수 선언
    // int error = 0;
    // unsigned int temperature = 0;
    // unsigned int humidity = 0;
    // float calculated_temperature = 0.0;
    // float calculated_humidity_linear = 0.0;
    // float calculated_humidity_true = 0.0;

    //초기화 작업
    // Usart_init();  //USART Setting : 9200bps, 8-1-non
    // Port_init();   //Sensor controll Port setting

// PRINTLINE;

    SHT7x_init();

    delayms(20);

    SHT7x_Connection_Reset_Sequence();


    // while(1)

    {
          /***********************************************************************
          Measure Temperature
          ***********************************************************************/

        //u8 j = 0;
        error = 0;
        SHT7x_Transmission_Start();

        error += SHT7x_Write(MEASURE_TEMPERATURE_COMMAND); // Measure Temperature
        error += SHT7x_ACK_Delay_Check();

        //j =(unsigned int)(SHT7x_Read(ACK) << 8);
        //temperature =(unsigned int)(SHT7x_Read(ACK) << 8);
        temperature =(u16)(SHT7x_Read(ACK) << 8);
        temperature|=(u16)SHT7x_Read(NOACK);
          // j=SHT7x_Read(NOACK);
        // debugprintf("temperature(%x)(%x)\r\n",temperature,j);

          //**************************  CRC넣을 자리  *****************************/

          if(!error) //No Error일때만 진입 for SHT7x_ACK_Delay_Check()
          {
          		// temperature = 0x1dc;
                calculated_temperature = d1 + (d2  * temperature);   //온도값 계산
          		debugprintf("calculated_temperature(%f)\r\n",calculated_temperature);

                avg_temperature_arr[avg_temperature_count] = calculated_temperature;
                avg_temperature_count++;

                      if(avg_temperature_count >= 2)
                      {
                            avg_temperature_count = 0;

                            avg_temperature  = (avg_temperature_arr[0] + avg_temperature_arr[1]) / 2.0f;
                            curr_avg_temperature = (previous_temperature + avg_temperature) / 2.0f;
                            previous_temperature = curr_avg_temperature;
                      }
          }
          else
          {
                SHT7x_Connection_Reset_Sequence();
                SHT7x_Transmission_Start();
          }




          /*********************************************************************
          Measure Humidity
          **********************************************************************/

          error = 0;
          SHT7x_Transmission_Start();

          error += SHT7x_Write(MEASURE_HUMIDITY_COMMAND); // Measure Humidity
          error += SHT7x_ACK_Delay_Check();

          humidity =(u16)(SHT7x_Read(ACK) << 8);
          humidity|=(u16)SHT7x_Read(NOACK);
          // j=SHT7x_Read(NOACK);
          // debugprintf("humidity(%x)(%x)\r\n", humidity,j);

          //**************************  CRC넣을 자리  *****************************

          if(!error) //No Error일때만 진입 for SHT7x_ACK_Delay_Check()
          {
          		// humidity = 0x4b7;
                calculated_humidity_linear = (c1 + (c2 * humidity)) + (c3 * (humidity * humidity)) ; //RH linear formula
                calculated_humidity_true   = (calculated_temperature - 25.0)* ((t1 + (t2 * humidity))) + calculated_humidity_linear;
          		debugprintf("calculated_humidity_true(%f)\r\n",calculated_humidity_true);

                avg_humidity_arr[avg_humidity_count] = calculated_humidity_true;
                avg_humidity_count++;

                      if(avg_humidity_count >= 2)
                      {
                            avg_humidity_count = 0;

                            avg_humidity  = (avg_humidity_arr[0] + avg_humidity_arr[1]) / 2.0f;
                            curr_avg_humidity = (previous_humidity + avg_humidity) / 2.0f;
                            previous_humidity = curr_avg_humidity;

                      }
          }
                 else
                 {

                            SHT7x_Connection_Reset_Sequence();
                            SHT7x_Transmission_Start();

                 }


          // delay_ms(MEASURING_TIME);   //센서 측정 주기 결정

    } //while(1) End

} // void main() End



void SHT7x_Connection_Reset_Sequence()
{
    int i = 0;

// PRINTLINE;

    SH_PIN_DATA_OUTPUT();	//DDRC|= 0x30;
    SH_PIN_CLK_L();	SH_PIN_DATA_H();	//SCK = 0; DATA = 1;

    for(i=0; i<=8; i++)
    {
		SH_PIN_CLK_H();	//SCK = 1;
		delayus(20);
		SH_PIN_CLK_L();	//SCK = 0;
		delayus(20);
    }

    // debugstring("SHT Connection_Reset\n\r");
}

//------------------------------------------------------------
void SHT7x_init()
{
	//reset

	//처리용 지역 변수 선언
	error = 0;
	temperature = 0;
	humidity = 0;
	calculated_temperature = 0.0;
	calculated_humidity_linear = 0.0;
	calculated_humidity_true = 0.0;

// PRINTLINE;

	delayms(2);

	SH_PIN_DATA_OUTPUT();
	SH_PIN_CLK_H();	SH_PIN_DATA_H();

    SHT7x_Connection_Reset_Sequence();

	// debugprintf("MDS_CPE_init..\r\n");
}


void SHT7x_Transmission_Start()
{
	SH_PIN_DATA_OUTPUT();  //Data을 출력으로 바꾼다.

	SH_PIN_CLK_L();
	delayus(10);	SH_PIN_DATA_H();

	delayus(10);	SH_PIN_CLK_H();
	delayus(10);	SH_PIN_DATA_L();
	delayus(10);	SH_PIN_CLK_L();
	delayus(20);	SH_PIN_CLK_H();
	delayus(10);	SH_PIN_DATA_H();
	delayus(10);	SH_PIN_CLK_L();
	delayus(10);	SH_PIN_DATA_L();
}

// #define SHT7x_Write	MDS_CPE_write_data

//------------------------------------------------------------
// u32 MDS_CPE_write_data(unsigned char a_data)	 //read=1, write = 0
u32 SHT7x_Write(unsigned char a_data)	 //read=1, write = 0
{
	int i = 0;
	int error_count = 0;
	u32 ret_val = 0;

// PRINTLINE;

	SH_PIN_DATA_OUTPUT();  //Data을 출력으로 바꾼다.

	for (i=7; i>=0; i--)
	{

		if ((a_data >> i) & 0x01) 	SH_PIN_DATA_H();
		else				   		SH_PIN_DATA_L();

		delayus(20);
		SH_PIN_CLK_H();		delayus(20);
		SH_PIN_CLK_L();		//delayus(20);
	}

	//SH_PIN_DATA_H();
	SH_PIN_DATA_INPUT();	delayus(20);

	SH_PIN_CLK_H();			delayus(20);

	while (SH_PIN_DATA_LEVEL() & (1<<4))
	{
		delayus(20);
		error_count++;
		if(error_count >= 3000)
		{
		    ret_val = 1;
		    debugstring("Error : SHT ACK Error\r\n");
		    break;
		}
	}

	SH_PIN_CLK_L();		delayus(20);
	SH_PIN_DATA_OUTPUT();

	return ret_val;	//ACK가 왔으면 err는 0(no error)이다.
}


/*
//------------------------------------------------------------
unsigned char MDS_CPE_write_data_NACK(unsigned char a_data)	 //read=1, write = 0
{
	int i = 0;
	int error_count = 0;
	int ret_val = 0;

	//DDRC|=0x03;  //set Output

	for (i=7; i>=0; i--)
	{

		if ((a_data >> i) & 0x01) SH_PIN_DATA_H(); //pioMDS_CPE_Data = 1;
		else				   SH_PIN_DATA_L(); //pioMDS_CPE_Data = 0;
		delay_us(20);
		SH_PIN_CLK_H(); //poMDS_CPE_Clk = 1;
		delay_us(20);
		SH_PIN_CLK_L(); //poMDS_CPE_Clk = 0;
		delay_us(20);
	}

	SH_PIN_DATA_H(); //pioMDS_CPE_Data = 1;
	delay_us(20);

	SH_PIN_CLK_H(); //poMDS_CPE_Clk = 1;
	delay_us(20);
	SH_PIN_CLK_L(); //poMDS_CPE_Clk  = 0;
	delay_us(20);

	return ret_val;	//ACK가 왔으면 err는 0(no error)이다.
}
*/

/*
//------------------------------------------------------------
unsigned char MDS_CPE_start_calibration(unsigned char command)
{
}
*/

//------------------------------------------------------------
int SHT7x_ACK_Delay_Check()
{
    int error_count = 0;
    int error = 0;

// PRINTLINE;

	//SH_PIN_DATA_OUTPUT();  //Data을 출력으로 바꾼다.
	//SH_PIN_DATA_H();
    //delayus(200);

	SH_PIN_DATA_INPUT();  //Data을 입력으로 바꾼다.
    while(SH_PIN_DATA_LEVEL() & (1<<4))
    {
        delayms(1);
        error_count++;
        if(error_count > 500)
        {
            error = 1; //ack대기시간이 500ms를 넘어가면
            break;   //에러를 출력하고 빠저나옴.
        }
    }

    return error;
}


/*
{
	int error_count = 0;
	int error = 0;

	//DGS change
	//DDRC|=0x03;   //set output
	SH_PIN_DATA_H(); //pioMDS_CPE_Data = 1;				 // make high data

	// delay_us(100);

	if (SH_PIN_DATA_H() == 1) //pioMDS_CPE_Data == 1)
	{
		while (SH_PIN_DATA_H() ==1) //pioMDS_CPE_Data == 1);	// data line이 Not ACK이면 진입 후 대기.
	}

	//DDRC&=~(0x02);  //set input of data port

	SH_PIN_CLK_H(); //poMDS_CPE_Clk = 1;
	delay_us(20);

	SH_PIN_CLK_L(); //poMDS_CPE_Clk = 0;

	//DGS add
	delay_us(20);


	return error;

}
*/

//------------------------------------------------------------
u8 SHT7x_Read(int ack)
{
	int i = 0;
	u8 a_data = 0;

// PRINTLINE;

	SH_PIN_DATA_INPUT(); //input mode
	delayus(20);

	for (i=7; i>=0; i--)
	{
		SH_PIN_CLK_H(); 	delayus(20);

		//a_data <<=1;

		if (SH_PIN_DATA_LEVEL()&(1<<4))
		{
			a_data |= 1<<i;
		}

		SH_PIN_CLK_L();		delayus(20);
	}

	// debugprintf("%x\r\n",a_data);

	SH_PIN_DATA_OUTPUT(); 	//output mode

	if (ack==MDS_CPE_ACK) 	SH_PIN_DATA_L();
	else		 			SH_PIN_DATA_H();
	delayus(20);

	SH_PIN_CLK_H();		delayus(20);
	SH_PIN_CLK_L(); 	delayus(20);

	//SH_PIN_DATA_H();

	return a_data;
}


/*
//------------------------------------------------------------
unsigned char MDS_CPE_read_azimuth(unsigned char command)
{
	int error = 0;
	unsigned char C_MSB = 0;
	unsigned char C_LSB = 0;
	unsigned char C_SUM = 0;

	MDS_CPE_transmission_start();

	error += MDS_CPE_write_data(command);					// measure_sot 0X33 (0011 0011)
	error += MDS_CPE_ack_delay_check();						// delay_ms;
	C_MSB = (unsigned char)MDS_CPE_read_data(MDS_CPE_ACK);			  // MSB
	C_LSB = (unsigned char)MDS_CPE_read_data(MDS_CPE_ACK);			  // LSB
	C_SUM = (unsigned char)MDS_CPE_read_data(MDS_CPE_NACK);			// CheckSum

	azimuth_data = (unsigned int)C_MSB;
	azimuth_data = (unsigned int)(azimuth_data << 8);
	azimuth_data|= (unsigned int)C_LSB;

	//printf("%d\n",azimuth_data);
	return error;
}
*/

/*
int SHT7x_ACK_Delay_Check()
{
	int error_count = 0;
	int error = 0;


	SH_PIN_DATA_OUTPUT();  //Data을 출력으로 바꾼다.
	//DDRC|=0x30;
	SH_PIN_CLK_H();	//DATA = 1;
	delayus(200);

	SH_PIN_DATA_INPUT();	//DDRC&=~(0x10);  //input
	while(SH_PIN_DATA_LEVEL() == (1<<4))
	{
		delayms(1);
		error_count++;
		if(error_count > 500)
		{
			error = 1; 	//ack대기시간이 500ms를 넘어가면
			break;   	//에러를 출력하고 빠저나옴.
		}
	}

	return error;
}
*/

#if 1
void Task_sh11(void)
{
	//U8 i;
	static int idx_sht = 0;
	//static U8 item_no = 0;  //0:temp 1:humi
	//static U8 ts_repeat = 0;

	switch (idx_sht)
	{
		case 0:
			SHT7x_init();
            idx_sht = 100;
            tick_SHT = 3;
			break;

        case 100:
        	if (tick_SHT==0)
    		{
		        error = 0;
		        SHT7x_Transmission_Start();

	            idx_sht = 200;
    		}
            break;

		case 200:
	        error += SHT7x_Write(MEASURE_TEMPERATURE_COMMAND); // Measure Temperature
	        if (!error)
	        {
				SH_PIN_DATA_INPUT();  //Data을 입력으로 바꾼다.
				tick_SHT = 50;
				idx_sht = 300;
	        }
	        else
	        {
	            idx_sht = 9000;
	        }
			break;

		// SHT7x_ACK_Delay_Check();
		case 300:
			if ((SH_PIN_DATA_LEVEL() & (1<<4)))
			{
				//1
				if (tick_SHT==0)
				{
					//error++; //ack대기시간이 500ms를 넘어가면
					idx_sht = 9000;
				}
			}
			else
			{
				// 0
				idx_sht = 400;
			}
			break;

		case 400:
	        temperature =(u16)(SHT7x_Read(ACK) << 8);
			idx_sht = 500;
			break;

        case 500:
	        temperature|=(u16)SHT7x_Read(NOACK);
            idx_sht = 600;
            break;

        case 600:
            calculated_temperature = d1 + (d2  * temperature);   //온도값 계산
      		// debugprintf("calculated_temperature(%f)\r\n",calculated_temperature);
            idx_sht = 700;
            break;

		case 700:
			error = 0;
			SHT7x_Transmission_Start();
			idx_sht = 800;
			break;

		case 800:
          	error += SHT7x_Write(MEASURE_HUMIDITY_COMMAND); // Measure Humidity
	        if (!error)
	        {
				SH_PIN_DATA_INPUT();  //Data을 입력으로 바꾼다.
				tick_SHT = 50;
				idx_sht = 900;
	        }
	        else
	        {
	            idx_sht = 9000;
	        }
			break;

		// SHT7x_ACK_Delay_Check();
		case 900:
			if ((SH_PIN_DATA_LEVEL() & (1<<4)))
			{
				//1
				if (tick_SHT==0)
				{
					//error++; //ack대기시간이 500ms를 넘어가면
					idx_sht = 9000;
				}
			}
			else
			{
				// 0
				idx_sht = 1000;
			}
			break;

		case 1000:
          	humidity =(u16)(SHT7x_Read(ACK) << 8);
			idx_sht = 1100;
			break;

		case 1100:
          	humidity|=(u16)SHT7x_Read(NOACK);
			idx_sht = 1200;
			break;

		case 1200:
            calculated_humidity_linear = (c1 + (c2 * humidity)) + (c3 * (humidity * humidity)) ; //RH linear formula
            calculated_humidity_true   = (calculated_temperature - 25.0)* ((t1 + (t2 * humidity))) + calculated_humidity_linear;

            // (+)131112 --->
            if (calculated_humidity_true > 99.0f)
                calculated_humidity_true = 99.0f;
            else if (calculated_humidity_true < 1.0f)
                calculated_humidity_true = 1.0f;
            // <----

            valid_temp= calculated_temperature;     //131008
            valid_humi = calculated_humidity_true;  //131008
      		// debugprintf("*** Internal Temp: %3.1f Humi: %d\r\n", calculated_temperature, (int)calculated_humidity_true);
  			idx_sht = 9000;
      		break;
/*
  		case 2000:
          	MDS_CPE_read_data(MDS_CPE_NACK);			// CheckSum

                if (item_no==0)
                {
                    gv.temp_0 = (azimuth_data1*256);
                    gv.temp_0 += azimuth_data0;
                    item_no = 1;

                    // 변환
                    gv.temp_1 = (gv.temp_0*d2C);
                    gv.temp_1 += d1C;
                    gv.temp_humi_ok++;  //온도계산 완료.

                }
                else
                {
                    if (gv.temp_humi_ok>0) {
                        float a_f1,a_f2;
                        gv.humi_0 = (azimuth_data1*256);
                        gv.humi_0 += azimuth_data0;
                        item_no = 0;

                        // 변환
                        RH_linear = C1;
                        a_f1 = (C2*gv.humi_0);
                        RH_linear += a_f1;
                        a_f1 = C3*gv.humi_0*gv.humi_0;
                        RH_linear += a_f1;

                        gv.humi_1 = (gv.temp_1-25);
                        a_f1 = (t2*gv.humi_0);
                        a_f2 += t1;
                        gv.humi_1 *= a_f1;
                        gv.humi_1 += RH_linear;

                        if (gv.humi_1>100.0) gv.humi_1 = 100.0;
                        if (gv.humi_1<0.1) gv.humi_1 = 0.1;

                        gv.temp_humi_ok++;  //습도계산 완료.
                    }
                }

                //tm_delay = (1000*10)/2; //10sec
                tm_delay = (1000*2)/2; //10sec
			idx_sht = IDX_300;
			break;

		case IDX_300:
*/
		case 9000:
  			tick_SHT = (60*1000)/10;	//10sec delay
			idx_sht = 9100;
			break;

		case 9100:
			if (tick_SHT==0)
			{
	            SHT7x_Connection_Reset_Sequence();
	            SHT7x_Transmission_Start();
				idx_sht = 9200;
			}
			break;

		case 9200:
		default:
			idx_sht = 0;
			break;
	}
}

#endif

// #endif




