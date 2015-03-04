#pragma once


// #define Q_GPS_MAX  		(512)	//(1024*4)
#define Q_GPS_MAX  		(1024*4)
#define GPS_LINE_MAX  	200


void gps_q_put(char ch);
char gps_q_get(void);
void gps_q_init(void);
void init_gps_data(void);

u32 get_gps_q_len(void);
void gps_neo6q_init(void);

void get_gps_time( rtcTime *a_time);
// rtcTime get_gps_time(void);

int get_gps_lat_d(void);
int get_gps_lat_m(void);
int get_gps_lat_mf(void);
int get_gps_lon_d(void);
int get_gps_lon_m(void);
int get_gps_lon_mf(void);


double pos_distance(double P1_latitude/*C10*/, double P1_longitude/*C11*/, double P2_latitude/*C12*/, double P2_longitude/*C13*/);

void set_gps_rawdata_display(void);
int is_gps_rawdata_display(void);

//-----------------------------------------------
#ifdef __K_GPS_C__
//-----------------------------------------------

int isGpsUpdated = 0;

U32 tmr_GPS_data_display=0;

rtcTime gps_time;

gps_data_T gps;

//-----------------------------------------------
#else
//-----------------------------------------------

extern gps_data_T gps;

extern int isGpsUpdated;

extern double d1_GpsLatitude;
extern char d1_GpsLatitude_NS;
extern double d1_GpsLongitude;
extern char d1_GpsLongitude_EW;
extern int d1_GpsYear;
extern int d1_GpsMonth;
extern int d1_GpsDay;
extern int d1_GpsHour;
extern int d1_GpsMin;
extern int d1_GpsSec;

extern rtcTime gps_time;

//extern gps_data_T gps;

extern t_SENSOR_Q gps_q;
extern ALIGN4 char gps_q_buf[];
extern ALIGN4 char gps_line[];

extern U32 tmr_GPS_data_display;

//-----------------------------------------------
#endif
//-----------------------------------------------


//void Task_gps(void);

void task_gps(void);
//void gga_gethering_start(void);
//int is_gga_gethering_completed(void);
int is_gps_valid();
void set_gps_valid();
void clr_gps_valid();

void set_gps_hasData(int a_value);
int is_gps_hasData(void);

void set_gps_hasTime(int a_value);
int is_gps_hasTime(void);
//void gps_neo6q_init(void);
//void gps_neo6q_init_pubx(void);

int get_gpsTimr_hour(void);
int get_gpsTimr_min(void);
int get_gpsTimr_sec (void);
int get_gpsTimr_year(void);
int get_gpsTimr_mon(void);
int get_gpsTimr_day (void);

