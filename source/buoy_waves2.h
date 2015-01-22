#pragma once

//#ifndef _buoy_waves2_h
//#define _buoy_waves2_h

#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include "fftpack.h"
//#include "utils.h"
//#include "common.h"

#define NoData 8192
#define	NoFreq 4097
#define nd	72
#define toler 0.25

//#define pi (4.0f*atan(1.0f))
#define MaxStrLen 1000
#define max(a,b) (a>b? a:b)
#define min(a,b) (a>b? b:a)
#define POWER(a,b) (a==0? 0:pow(a,b))
#define SQUARE_ROOT(a) (a==0? 0:sqrt(a))


typedef struct _WAVE_DATA_COMMON {
    float Hmax;   // = Hmax;
    float Tmax;   // = Tmax;
    float H1_3;   // = H3;
    float T1_3;   // = T3;
    float H1_10;  // = Hm;
    float TH1_10; // = Tz;
    float Hs;     // = H3;    //Hs;
    //Tav;    // =
    //Hav;    // =
    float WaveDirection;  // = pwd;
} WAVE_DATA_COMMON;


#define WaveAnalysisTestFlag 0
#define total_waves_testflag 1
#define TideAnalysisTestFlag 1
#define BuoyWaveTestFlag 1
#define WestOption 1
#define SmothOption 1
#define BuoyWaveTestFlag 1

//void realft2(float *datH);
void realft(float data[], unsigned long n, int isign);

void call_mose1000_HF(float (* datH)[3],float wd,
					  float dtH,int nH, WAVE_DATA_COMMON *result);
void call_mose1000_LF(float (* datL)[3],float wd,
					  float dtL,int nL, WAVE_DATA_COMMON *result);

//ccy
void filePointer(FILE *fileWriter);
// processing routines
/*
void call_mose1000(float datH[7200][3],float datL[7200][3],float wd,
				   float dtH,float dtL,int nH,int nL);
void calc_mose1000(float dat[7200][3],float wd,float dt,int n,float cut_off);
*/
void calc_mose1000(float (* dat)[3],float wd,float dt,int n,float cut_off);

// Aug., 19, 2010
void calc_mose1000H(float wd,float dtH,int nH);
void calc_mose1000L(float wd,float dtL,int nL);

int calc_wave(float ax[],float ay[],float az[],
			  int n,float dt,float wd,int prop,float cut_off);
void spec1d(int n,float delt,float *ax,float *ay,float *az,
			float wd,int prop,float cut_off);
int ZeroUp_waveC(int n,float dt,float *dat);

// KimDH	2012.08.12
//const int NPOINTS=256;
#define	 NPOINTS	256
//const int NPOINTS2=128;
#define NPOINTS2	128
//const int NTAPER=32;  /* number of tapered samples, one side, must be dividable by 4 */
#define NTAPER	32  /* number of tapered samples, one side, must be dividable by 4 */
//const int NSPECTRA=14;	/* number of spectra */

//const float PI=(float)3.1415926535898; /* (float)(4.0*atan(1.0)); */
#define PI	((float)3.1415926535898) /* (float)(4.0*atan(1.0)); */
/* const float FS=2.0F; */

/* KimDH	2012.08.12
void fltr_Hann(float dat[],int n,float *rat);
void smoth_spec(int nf,float df,float sf[]);
void DrSpFunc02(int f);
*/

//#endif


//-----------------------------------------------
#ifdef __BUOY_WAVES2_C__
//-----------------------------------------------

WAVE_DATA_COMMON result_hf;
WAVE_DATA_COMMON result_lf;


//-----------------------------------------------
#else
//-----------------------------------------------

extern WAVE_DATA_COMMON result_hf;
extern WAVE_DATA_COMMON result_lf;

//-----------------------------------------------
#endif
//-----------------------------------------------



