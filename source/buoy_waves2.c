#define	__BUOY_WAVES2_C__

//#include "adStar.h"
#include "k_includes.h"
#include "buoy_waves2.h"

// typedef struct _MOSE_OUTDATA {
//     U8 year;
//     U8 mon;
//     U8 day;
//     U8 hour;
//     U8 min;
//     U8 sec[6];
//     double hmax;
//     double tmax;
//     double h3;
//     double t3;
//     double pwd;
// } _MOSE_OUTDATA_T;

extern _MOSE_OUTDATA_T mose_out_data;


// number of arrays
int nf =0 ; //CCY nt,
// spacing
float df=0;//CCY ,dtheta;
// measured data
float ax[NoData],ay[NoData],az[NoData];
// fourier frequency parameters
// spectral
float fr[NoFreq],wk[NoFreq],th[nd],ws1d[NoFreq],phase[NoFreq],Dr[NoFreq][nd];
// zero-up crossing
float crest[NoFreq],trough[NoFreq];
float wh1[NoFreq],wh2[NoFreq];
float wp1[NoFreq],wp2[NoFreq];
int pos[NoData];
float et[NoData];
// wave
float Hs=0,Tp=0,mwd=0,pwd=0,spr=0;
int nw;
float etmax=0,etmin=0,etavg=0,Hmax=0,Tmax=0,H3=0,T3=0,Hm=0,Tz=0;
float temp[NoData];

int NSPECTRA=0; /* number of spectra */

typedef struct waves
{
    int nw;                         //- zero focus 웨이브 개수
    float Hmax; float Tmax;                     //-최대 파고
    float H3;   float T3;           //- 유의파고 , 평균파고
    float Hm;   float Tz;           //- 유의파 주기 , 평균파 주기
    float Hs;   float Tp;           //-  유의파고 (스펙트럼상의 파고) , 첨두 주기
    float mwd;  float pwd;          // - 평균파향, 피크파향
    float spr;                      //- 파향의 분산 개수
    float fr[NoFreq];               //- 주파수 0.00~~
    float sf[NoFreq];               //- 벡터 값
    float dr[NoFreq][nd];           //- 디렉션
} wave_data;
wave_data waveH,waveL;


static int mose_data_valid = 0;
//Hmax,Tmax,H3,T3,pwd
float get_mose_Hmax(void)	{ return waveH.Hmax;}
float get_mose_Tmax(void)	{ return (waveH.Tmax*10.0f);}     //(*)131015
float get_mose_H3(void)	{ return waveH.H3;}
float get_mose_t3(void)	{ return (waveH.T3*10.0f);}           //(*)131015
float get_mose_pwd(void)	{ return waveH.pwd;}

float get_moseL_Hmax(void)	{ return waveL.Hmax;}
float get_moseL_Tmax(void)	{ return (waveL.Tmax*10.0f);}    //(*)131015
float get_moseL_H3(void)	{ return waveL.H3;}
float get_moseL_t3(void)	{ return (waveL.T3*10.0f);}      //(*)131015
float get_moseL_pwd(void)	{ return waveL.pwd;}

int get_mose_data_valid(void)	{ return mose_data_valid;}
void set_mose_data_valid(int a_val)	{ mose_data_valid = a_val; }




//void myprintf(const char * rsp,	...);

// wd는 30.0을 넣으라고 했으며, dtH는 설정된 주기 : 현재 0.5초와 5.0초를 준다.
void call_mose1000_HF(float (* datH)[3],float wd,float dtH,int nH, WAVE_DATA_COMMON *result)
{
    int i,j;
    // high frequency section
    Hmax = 0.0;
    Tmax = 0.0;
    H3 = 0.0;
    T3 = 0.0;
    Hm = 0.0;
    Tz = 0.0;
	nw = 0;
    Hs = 0.0;
    Tp = 0.0;
    mwd = 0.0;
    pwd = 0.0;
    spr = 0.0;

	memset(fr, 0x00, sizeof(fr));
	memset(wk, 0x00, sizeof(wk));
	memset(th, 0x00, sizeof(th));
	memset(ws1d, 0x00, sizeof(ws1d));
	memset(phase, 0x00, sizeof(phase));
	memset(Dr, 0x00, sizeof(Dr));
	memset(temp, 0x00, sizeof(temp));

	memset(ax, 0x00, sizeof(ax));
	memset(ay, 0x00, sizeof(ay));
	memset(az, 0x00, sizeof(az));
	memset(crest, 0x00, sizeof(crest));
	memset(trough, 0x00, sizeof(trough));
	memset(wh1, 0x00, sizeof(wh1));
	memset(wh2, 0x00, sizeof(wh2));
	memset(wp1, 0x00, sizeof(wp1));
	memset(wp2, 0x00, sizeof(wp2));
	memset(pos, 0x00, sizeof(pos));
	memset(et, 0x00, sizeof(et));

/*	calc_mose1000(datH,wd,dtH,nH,1.0f/30.0f); KimDH */
	calc_mose1000(datH,wd,dtH,nH,2.0f);
    waveH.Hmax=Hmax;
    waveH.Tmax=Tmax;
    waveH.H3=H3;
    waveH.T3=T3;
    waveH.Hm=Hm;
    waveH.Tz=Tz;
    waveH.nw=nw;
    waveH.Hs=Hs;
    waveH.Tp=Tp;
    waveH.mwd=mwd;
    waveH.pwd=pwd;
    waveH.spr=spr;
    for (i=0;i<nf;i++)
    {
        waveH.fr[i]=fr[i];
        waveH.sf[i]=ws1d[i];
    }
    for (i=0;i<nf;i++)
    {
        for (j=0;j<nd;j++)
            waveH.dr[i][j]=Dr[i][j];
    }

    result->Hmax = Hmax;
    result->Tmax = Tmax;
    result->H1_3 = H3;
    result->T1_3 = T3;
    result->H1_10 = Hm;
    result->TH1_10 = Tz;
    result->Hs = H3;    //Hs;
    //	result->Tav =
    //	result->Hav =
    result->WaveDirection = pwd;
}

void call_mose1000_LF(float (* datL)[3],float wd, float dtL,int nL, WAVE_DATA_COMMON *result)
{
    int i,j;
    // Low frequency section
    Hmax = 0.0;
    Tmax = 0.0;
    H3 = 0.0;
    T3 = 0.0;
    Hm = 0.0;
    Tz = 0.0;
	nw = 0;
    Hs = 0.0;
    Tp = 0.0;
    mwd = 0.0;
    pwd = 0.0;
    spr = 0.0;

	memset(fr, 0x00, sizeof(fr));
	memset(wk, 0x00, sizeof(wk));
	memset(th, 0x00, sizeof(th));
	memset(ws1d, 0x00, sizeof(ws1d));
	memset(phase, 0x00, sizeof(phase));
	memset(Dr, 0x00, sizeof(Dr));
	memset(temp, 0x00, sizeof(temp));

	memset(ax, 0x00, sizeof(ax));
	memset(ay, 0x00, sizeof(ay));
	memset(az, 0x00, sizeof(az));
	memset(crest, 0x00, sizeof(crest));
	memset(trough, 0x00, sizeof(trough));
	memset(wh1, 0x00, sizeof(wh1));
	memset(wh2, 0x00, sizeof(wh2));
	memset(wp1, 0x00, sizeof(wp1));
	memset(wp2, 0x00, sizeof(wp2));
	memset(pos, 0x00, sizeof(pos));
	memset(et, 0x00, sizeof(et));


/*	calc_mose1000(datL,wd,dtL,nL,0.0f); KimDH */
	calc_mose1000(datL,wd,dtL,nL,0.2f);
    waveL.Hmax=Hmax;
    waveL.Tmax=Tmax;
    waveL.H3=H3;
    waveL.T3=T3;
    waveL.Hm=Hm;
    waveL.Tz=Tz;
    waveL.nw=nw;
    waveL.Hs=Hs;
    waveL.Tp=Tp;
    waveL.mwd=mwd;
    waveL.pwd=pwd;
    waveL.spr=spr;
	//CCY printf("CCY CHK0 waveL.pwd = %f", waveL.pwd);
    for (i=0;i<nf;i++)
    {
        waveL.fr[i]=fr[i];
        waveL.sf[i]=ws1d[i];
    }
    for (i=0;i<nf;i++)
    {
        for (j=0;j<nd;j++)
            waveL.dr[i][j]=Dr[i][j];
    }

    result->Hmax = Hmax;
    result->Tmax = Tmax;
    result->H1_3 = H3;
    result->T1_3 = T3;
    result->H1_10 = Hm;
    result->TH1_10 = Tz;
    result->Hs = H3;        // Hs;
//	result->Tav =
//	result->Hav =
    result->WaveDirection = pwd;
	//CCY printf("CCY CHK1 result->WaveDirection = %f", result->WaveDirection);
}


void calc_mose1000(float (* dat)[3],float wd,float dt,int n,float cut_off)
{
    int i;
    for (i=0;i<n;i++)
    {
        ax[i]=dat[i][0];
        ay[i]=dat[i][1];
        az[i]=dat[i][2];


    }
    // 3. processing the waves
    calc_wave(ax,ay,az,n,dt,wd,2,cut_off);
    // 4. print results
/*
    if (BuoyWaveTestFlag==1)
    {
        printf("=======================\n");
        printf("------ wave information -----\n");
        printf("Hmax : %lf, nw   : %d \n",Hmax,nw);
        printf("H3   : %lf, T3   : %lf\n",H3,T3);
        printf("Hm   : %lf, Tz   : %lf\n",Hm,Tz);
        printf("Hs   : %lf, Tp   : %lf\n",Hs,Tp);
        printf("mwd  : %lf, pwd  : %lf,  spr  : %lf\n",mwd,pwd,spr);
        printf("+++++++++++++++++++++++\n");
    }
    */
}

// target : acceleration or dispacement
// version : 3.5
// Apr., 24, 2010
// ax : time series data of x-dir horizontal acceleration or displacement
// ay : time series data of y-dir horizontal acceleration or displacement
// az : time series data of vertical acceleration displacement
// prop : switch whether the input data set is acceleration(=1) or dispacement(=2)
// cut_off : low cut_off frequency
int calc_wave(float ax[],float ay[],float az[],int n,float dt,float wd,int prop,float cut_off)
{
//	float *zeroup_waves;
    int i,t;
//	FILE *fp;
	float temp1=0,temp2=0,temp3=0;
    // if Westopion
    if (WestOption==1)
    {
        for (i=0;i<n;i++)
            ax[i]=ax[i]*(-1.0);
    }
    // de-mean
    temp1=0.0;
    temp2=0.0;
    temp3=0.0;
    for (i=0;i<n;i++)
    {
        temp1+=ax[i];
        temp2+=ay[i];
        temp3+=az[i];
    }
    temp1=temp1/(float)(n);
    temp2=temp2/(float)(n);
    temp3=temp3/(float)(n);
    if (BuoyWaveTestFlag==1)
        printf("heave : %f, north : %f, west : %f\r\n",temp1,temp2,temp3);
    for (i=0;i<n;i++)
    {
        ax[i]-=temp1;
        ay[i]-=temp2;
        az[i]-=temp3;
    }
    // 1. calculate wave spectrum
    spec1d(n,dt,ax,ay,az,wd,prop,cut_off);
    if (BuoyWaveTestFlag==1)
    {
        //printf(">>	Hs : %f\r\n",Hs);
        //printf(">>	Tp : %f\r\n",Tp);
        printf(">>  mwd : %f\r\n",mwd);
        printf(">>  spr : %f\r\n",spr);


#if 0
        fp=fopen("test_ws1d.out","w");
        for (i=0;i<nf;i++)
            fprintf(fp,"%f %f\n",fr[i],ws1d[i]);
        fclose(fp);
#endif
    }
    // 2. generating eta
    if (prop==1)
	{	for(t=0;t<n;t++)
        {
            et[t]=0.0f;
            for (i=0;i<nf;i++)
                et[t]+=sqrt(2.0f*ws1d[i]*df)
    			*cos(2*PI*fr[i]*(float)(t)*dt+phase[i]);
        }
    }
    else
    {
		for(t=0;t<n;t++){
            et[t]=az[t];
		}
    }
    if (BuoyWaveTestFlag==1)
    {
#if 0
        fp=fopen("test_se.out","w");
        for (t=0;t<n;t++)
            fprintf(fp,"%f\n",eta[t]);
        fclose(fp);
#endif
    }
    // 3. zero-up crossing analysis
	//printf("Bef ZeroUp_waveC-> n= %d, dt=%f \n",n,dt);
    ZeroUp_waveC(n,dt,et);

    if (BuoyWaveTestFlag==1)
    {
        printf("==================================\r\n");
        printf("------ calc_wave -----------------\r\n");
        //printf("number of waves : %d\r\n",nw);
        printf("Hmax : %lf, Tmax   : %lf\r\n",Hmax,Tmax);
        printf("H3   : %lf, T3     : %lf\r\n",H3,T3);
        printf("Hm   : %lf, Tz     : %lf\r\n",Hm,Tz);
        printf("Hs   : %lf, Tp     : %lf\r\n",Hs,Tp);
        printf("pwd  : %lf\r\n",pwd);
        printf("++++++++++++++++++++++++++++++++++\r\n");


        //debugprintf("$,%02d,%02d,%02d,%02d,%02d,%s,", mose_out_data.year, mose_out_data.mon, mose_out_data.day, mose_out_data.hour,mose_out_data.min, mose_out_data.sec );
        //debugprintf("%lf,%lf,%lf,%lf,%lf\r\n",Hmax,Tmax,H3,T3,pwd);
        //myprintf("$,%02d,%02d,%02d,%02d,%02d,%s,", mose_out_data.year, mose_out_data.mon, mose_out_data.day, mose_out_data.hour,mose_out_data.min, mose_out_data.sec );
        //myprintf("%lf,%lf,%lf,%lf,%lf\r\n",(Hmax/10.0f),Tmax,(H3/10.0f),T3,pwd);
        //myprintf("%lf,%lf,%lf,%lf,%lf\r\n",(Hmax),Tmax,(H3),T3,pwd);
        set_mose_data_valid(1);
    }
    // 5. delete memories
    return 0;
}

void spec1d(int n,float delt,float *ax,float *ay,float *az,float wd,int prop,float cut_off)
{
	int i=0, j=0;//, t;
	float w=0,f=0,sumww=0,norm=0,
		a1=0,b1=0,m1=0,a2=0,b2=0,cos2=0,sin2=0,m2=0,n2=0,
		psd=0,dir=0,spread=0,check=0,skew=0,kurt=0,
		tp=0,psdp=0,dirp=0,spreadp=0,checkp=0,skewp=0,kurtp=0,
		df=0,m_2=0,m_1=0,m0=0,mm1=0,mm2=0,m3=0,m4=0,m21=0,
		hmo=0,teller,noemer=0,
		S=0,C=0,FS=0; /* Add S, C for mean wave dir. KimDH */

	static float x[NPOINTS]; /* east */
	static float y[NPOINTS]; /* north */
	static float z[NPOINTS]; /* vert */
	static float cxx[NPOINTS2+1];
	static float cyy[NPOINTS2+1];
	static float czz[NPOINTS2+1];
	static float cyx[NPOINTS2+1];
	static float qzx[NPOINTS2+1];
	static float qzy[NPOINTS2+1];

	memset(x, 0x00, sizeof(x));
	memset(y, 0x00, sizeof(y));
	memset(z, 0x00, sizeof(z));
	memset(cxx, 0x00, sizeof(cxx));
	memset(cyy, 0x00, sizeof(cyy));
	memset(czz, 0x00, sizeof(czz));
	memset(cyx, 0x00, sizeof(cyx));
	memset(qzx, 0x00, sizeof(qzx));
	memset(qzy, 0x00, sizeof(qzy));

	FS = cut_off;
	//CCY 130529if(FS==2.0) NSPECTRA=8;  //해운대 , KOGA HF
    //if(FS==2.0) NSPECTRA=2;  //해운대 , KOGA HF
	if(FS==2.0f) NSPECTRA=8;  //해운대 , KOGA HF
	if(FS==0.2f) NSPECTRA=4;  //KOGA LF

for (i=0; i<NPOINTS*NSPECTRA; i++) {

	x[i%NPOINTS] = ax[i];
	y[i%NPOINTS] = ay[i];
	z[i%NPOINTS] = az[i];

	if (((i%NPOINTS)==NPOINTS-1) && (i<NPOINTS*NSPECTRA))
        {
		//CCY printf("In if (((i%NPOINTS)==NPOINTS-1) && (i<NPOINTS*NSPECTRA)) \n");
		/* apply Datawell tapered window */
		sumww=(float)NPOINTS-1.25F*(float)NTAPER-1.0F; /* for Datawell tapered window NPOINTS=256, NTAPER=32, sumww=215 */
		for (j=0; j<NTAPER; j++){
			w=0.5F*(1.0F-(float)cos(PI*j/NTAPER));
			x[j]*=w;
			y[j]*=w;
			z[j]*=w;
			x[NPOINTS-1-j]*=w;
			y[NPOINTS-1-j]*=w;
			z[NPOINTS-1-j]*=w;
        }
		/* fft in-place, xyz-1 because realft starts at xyz[1], while data start at xyz[0] */
		realft(x-1,NPOINTS,1); /* convention h_j=(1/N)Sum_k=0:N-1 H_k exp(-2*pi*i*k*j/N) */
		realft(y-1,NPOINTS,1); /* sine gives positive (imaginary) H for positive (k=0..N/2) frequencies and vice versa */
		realft(z-1,NPOINTS,1); /* sin(x) = i(+1/2)exp(-ix) + i(-1/2)exp(+ix) met x=2*pi*f*t en t=j*dt, f=(k/N)*fs, fs=1/dt */
		//for (j=0; j<NPOINTS; j++) printf("%3i %f %f\n",j,x[j],y[j]);

		/* co- and quad-spectra */
		cxx[0]+=x[0]*x[0];
		cyy[0]+=y[0]*y[0];
		czz[0]+=z[0]*z[0];
		cyx[0]+=y[0]*x[0];
		/* qxz[0]+=0.0; imaginary part x,y,z[0] zero
		qyz[0]+=0.0; */
		cxx[NPOINTS2]+=x[1]*x[1];
		cyy[NPOINTS2]+=y[1]*y[1];
		czz[NPOINTS2]+=z[1]*z[1];
		cyx[NPOINTS2]+=y[1]*x[1];
		/* qxz[NPOINTS2]+=0.0; imaginary part x,y,z[NPOINTS2] zero
		qyz[NPOINTS2]+=0.0; */
		for (j=1; j<NPOINTS2; j++){
			cxx[j]+=2.0F*(x[2*j]*x[2*j] + x[2*j+1]*x[2*j+1]);
			cyy[j]+=2.0F*(y[2*j]*y[2*j] + y[2*j+1]*y[2*j+1]);
			czz[j]+=2.0F*(z[2*j]*z[2*j] + z[2*j+1]*z[2*j+1]);
			cyx[j]+=2.0F*(y[2*j]*x[2*j] + y[2*j+1]*x[2*j+1]);
			qzx[j]+=2.0F*(z[2*j]*x[2*j+1] - z[2*j+1]*x[2*j]); /* convention Qzx=ReZ*ImX-ImZ*ReX */
			qzy[j]+=2.0F*(z[2*j]*y[2*j+1] - z[2*j+1]*y[2*j]); /* wave to +x direction has x=sin, z=cos, Qzx=+1 */
			//CCY printf("In for (j=1; j<NPOINTS2; j++)==>qzx[%d]=%f,  qzy[%d]=%f \n",j, qzx[j], j, qzy[j] );
    }
        }
        }

/* Analysis Sum Spectrum
from MoseGAnalysis.cpp
KimDH 2012.08.12		*/

		/* no smoothing, because no 'thinning out' */
		m_2=0.0;
		m_1=0.0;
		m0 =0.0;
		mm1=0.0;
		mm2=0.0;
		m3 =0.0;
		m4 =0.0;
		m21=0.0;
		/* normalization
		norm-factor only used for psd, cancels in all other parameters
		power spectrum (1/N) Sum|ci|^2 = (1/(sumww*N)) Sum|Ck|^2
		power spectral density (1/(FS/N))*(1/(sumww*N)) Sum|Ck|^2 = (1/(FS*sumww)) Sum|Ck|^2 */
		norm=FS*sumww*(float)(NSPECTRA);
		/* co- and quad-spectra
		internal parameters (a1,b1,m1,n1,a2,b2,cos2,sin2,m2,n2) all mathematical angle (anticlockwise) and direction TO
		output parameters (psd,dir,spread,check,skew,kurt) all geographical angle (clockwise) and direction FROM */
		for (j=0; j<=NPOINTS2; j++){
			if ((czz[j]!=0.0F)&&((cxx[j]+cyy[j])!=0.0F)){
				psd   =czz[j]/norm;
				noemer=(float)sqrt(czz[j]*(cxx[j]+cyy[j]));
				if (noemer>0.0F){
					a1    =qzy[j]/noemer;
					b1    =-1.0F*qzx[j]/noemer;
				} else {
					a1=0.0;
					b1=0.0;
        }
				dir   =(float)atan2(b1,(float)-1.0*a1); /* atan2(-east/-north)=[-PI,+PI], wave direction from, 0=north, clockwise */
				/* in all following equations use mathematical angle, anticlockwise, wave direction to: theta=pi-dir, [Datawell-conventies, H.Noteborn]*/
				m1    =(float)sqrt(a1*a1+b1*b1);
				noemer=cxx[j]+cyy[j];
				if (noemer>0.0F){
					a2    =(cyy[j]-cxx[j])/noemer;
					b2    =-2.0F*cyx[j]/noemer;
				} else {
					a2=0.0;
					b2=0.0;
        }
				cos2  =(float)cos(2.0*dir); /* cos(2theta)=cos(2pi-2dir)=cos(2dir) */
				sin2  =(float)sin(-2.0*dir); /* sin(2theta)=sin(2pi-2dir)=sin(-2dir) */
				m2    = a2*cos2+b2*sin2;
				n2    =-a2*sin2+b2*cos2;
				teller=2.0F*(1.0F-m1);
				if (teller<0.0F) teller=0.0F;
				spread=(float)sqrt(teller);
	   			check =(float)sqrt((cxx[j]+cyy[j])/czz[j]);
				teller=(1.0F-m2)/2.0F;
				//printf("%i %f %f %f %f %f %f %f %f ",j,a2,b2,cos2,sin2,m1,m2,teller,n2);
				if (teller>0.0F){
					skew  =n2/(teller*(float)sqrt(teller)); /* consistent met wave dir from */
				} else {
					skew=0.0F;
    }
				teller=6.0F-8.0F*m1+2.0F*m2;
				noemer=4.0F-8.0F*m1+4.0F*m1*m1;
				//printf("%f %f\n",teller,noemer);
				if (noemer!=0.0F){
					kurt=teller/noemer; /* delta = (6-8*m1+2*m2)/((2-2*m1)^2) [Kuik] */
				} else {
					kurt=0.0F;
    }
				/* store spectrum bin parameters */
				cxx[j]=psd;
				cyy[j]=(180.0F/PI)*dir;
				if (cyy[j]<=0.0) cyy[j] = cyy[j] + 360.0; //KimDH
				cyy[j] = cyy[j] - (float)((int)(cyy[j]/360))*360.0; //KimDH
				//printf("cxx[%d]=%f , cyy[%d]=%f \n", j, cxx[j], j, cyy[j]);
				czz[j]=(180.0F/PI)*spread;
				cyx[j]=check;
				qzx[j]=skew;
				qzy[j]=kurt;
			} else {
				cxx[j]=0.0F;
				cyy[j]=0.0F;
				czz[j]=0.0F;
				cyx[j]=0.0F;
				qzx[j]=0.0F;
				qzy[j]=0.0F;
        }
			/* calculate moments */
			m0+=psd;
			if (j!=0){
				f   =j*FS/NPOINTS;
				m_2+=psd/(f*f);
				m_1+=psd/f;
				mm1+=psd*f;
				mm2+=psd*f*f;
				m3 +=psd*f*f*f;
				m3 +=psd*f*f*f*f;
				m21+=psd*psd*f;
    }
    }

/* END part of Analysis Sum Spectrum  */

		/* find peak spectrum */
		int jp = 0;
		psdp=-1.0F;
		for (j=0; j<=NPOINTS2; j++) {
			if (cxx[j]>psdp){
				jp=j;
				psdp=cxx[j];
    }
			/* for mean wave dir. -KimDH */
			//float fccy = PI/180;
			//printf("fccy=%f", fccy);
			S += sin(cyy[j]*PI/180);
			C += cos(cyy[j]*PI/180);
    }

		/* cal. of mwd - KimDH */
		//ORG mwd = 270.0 - atan2(S,C)*180.0/PI;
		mwd = atan2(S,C)*180.0/PI;
		if (mwd<=0.0)	mwd = mwd + 360.0; //KimDH
		mwd = mwd -(float)((int)(mwd/360))*360.0;	//KimDH
		if (jp!=0){
			tp=NPOINTS/(jp*FS);
		} else {
			tp=0.0;
    }
		dirp   =cyy[jp];
		spreadp=czz[jp];
		checkp =cyx[jp];
		skewp  =qzx[jp];
		kurtp  =qzy[jp];
		/* limit */
		/* LimitT(&tp); */
		if (psdp>(float)9.9999E9) psdp=(float)9.9999E9;
		if ((psdp!=0.0F)&&(psdp<(float)1.0000E-9)) psdp=(float)1.0000E-9;
		dirp=(float)fmod(dirp+360.0F,360.0F);
		if (spreadp>99.0F) spreadp  = 99.0F;
		if (checkp>  9.999F) checkp =  9.999F;
		if (skewp > 99.99F) skewp   = 99.99F;
		if (skewp <-99.99F) skewp   =-99.99F;
		if (kurtp > 99.99F) kurtp   = 99.99F;
		if (kurtp <  0.0F) kurtp    =  0.0F;

		/* spectral moment parameters */
		df = FS/NPOINTS;
		m0 *= df;
		/* hmo=4.0F*(float)sqrt(m0);	*/ //KimDh
		//ORG Hs = 4.0F*(float)sqrt(m0);
		Hs = 4.0F*(float)SQUARE_ROOT(m0);
		pwd = dirp;
		spr = spreadp;
		Tp = tp;
}

#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
void four1(float data[], unsigned long nn, int isign)
{
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	float tempr,tempi;

	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) {
		if (j > i) {
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
    }
		m=n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
    }
		j += m;
        }
	mmax=2;
	while (n > mmax) {
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
    }
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
    }
}
#undef SWAP

/* realft Numerical Recipes
   Calculates the Fourier transform of a set of n real valued data points. Replaces this data
   (which is stored in array data[1..n]) by the positive half of its complex Fourier transform.
   The real-valued first and last components of the complex transform are returned as elements
   data[1] and data[2], respectively. n must be a power of 2. This routine also calculates the
   inverse transform of a complex data array if it is the transform of real data. (Result in
   this case must be multiplied by 2/n).
*/
void realft(float data[], unsigned long n, int isign)
{
	void four1(float data[], unsigned long nn, int isign);
	unsigned long i,i1,i2,i3,i4,np3;
	float c1=0.5,c2,h1r,h1i,h2r,h2i;
	double wr,wi,wpr,wpi,wtemp,theta;

	theta=3.141592653589793/(double) (n>>1);
	if (isign == 1) {
		c2 = -0.5;
		four1(data,n>>1,1);
	} else {
		c2=0.5;
		theta = -theta;
}
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	wr=1.0+wpr;
	wi=wpi;
	np3=n+3;
	for (i=2;i<=(n>>2);i++) {
		i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
		h1r=c1*(data[i1]+data[i3]);
		h1i=c1*(data[i2]-data[i4]);
		h2r = -c2*(data[i2]+data[i4]);
		h2i=c2*(data[i1]-data[i3]);
		data[i1]=h1r+wr*h2r-wi*h2i;
		data[i2]=h1i+wr*h2i+wi*h2r;
		data[i3]=h1r-wr*h2r+wi*h2i;
		data[i4] = -h1i+wr*h2i+wi*h2r;
		wr=(wtemp=wr)*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
    }
	if (isign == 1) {
		data[1] = (h1r=data[1])+data[2];
		data[2] = h1r-data[2];
	} else {
		data[1]=c1*((h1r=data[1])+data[2]);
		data[2]=c1*(h1r-data[2]);
		four1(data,n>>1,-1);
    }
}

// Jul,15,2010
// zeroup_crossing
//
int ZeroUp_waveC(int n,float dt,float *dat)
{
// to the global	float et[NoData];
    int i,i1,i2,j;
    int cc;
    int n1;
    float maxval,tempval;
    int maxpos;
    //float Hmax,Tmax,H3,T3,Hm,Tz;
//	FILE *fp;
    // 1. get etmin, etmax, and etavg
    etmin=1000.0;
    etmax=-1000.0;
    etavg=0.0;
    for (i=0;i<n;i++)
    {
#if 0
		printf("dat[%d]=%f \n",i, dat[i]);
#endif
        if (dat[i]>etmax) etmax=dat[i];
        if (dat[i]<etmin) etmin=dat[i];
        etavg+=dat[i];
    }
	etavg=etavg/(float)(n);
    // 2. de-mean and allovating et
    //	et=new float [n];
	printf("n= : %d  , dt= %f\n",n, dt);
    for (i=0;i<n;i++)
    {
        dat[i]=dat[i]-etavg;
        et[i]=dat[i];
		//printf("et[i]= %f\n", i , et[i]);
    }
    // 2.-1 filtering by KMAHN.
	// 130529 ccy
    for (i=3;i<n-4;i++)
    {
        if (fabs(et[i])<=toler)
        {
            et[i]=(dat[i-3]+dat[i-2]+dat[i-1]+dat[i]
			      +dat[i+3]+dat[i+2]+dat[i+1])/(float)(7);
        }
    }
	//
    // 3. get the position of zero-up cross
    for (i=0;i<n;i++)
        pos[i]=-9;
    cc=0;
    for (i=0;i<n;i++)
    {
        i1=max(i-1,0);
        i2=min(i+1,n-1);
        if (((et[i]<0.0)&&(et[i2]>0.0))||
        ((et[i]*et[i2]==0)&&(et[i1]<0.0)&&(et[i2]>0.0)) )
        {
            pos[cc]=i;
            cc++;
        }
    }
    nw=cc-1;
    // 4. get the crest and trough
    for (i=0;i<nw;i++)
    {
        crest[i]=-1000;
        trough[i]=1000;
        for (j=pos[i];j<pos[i+1];j++)
        {
            if (crest[i]<et[j]) crest[i]=et[j];
            if (trough[i]>et[j]) trough[i]=et[j];
        }
        wh1[i]=crest[i]-trough[i];
		wp1[i]=dt*((float)(pos[i+1])-(float)(pos[i]));
    }

// test wave
    if (BuoyWaveTestFlag==1)
    {
#if 0
        fp=fopen("zeroup_no.out","w");
        for (i=0;i<nw;i++)
            fprintf(fp,"%lf	%lf\n",wh1[i],wp1[i]);
        fclose(fp);
#endif
    }

    // 5. sorting data
    for (i=0;i<nw;i++)
    {
        wh2[i]=wh1[i];
        wp2[i]=wp1[i];
    }
    for (i=0;i<nw;i++)
    {
        maxval=wh2[i];
        maxpos=i;
        for (j=i+1;j<nw;j++)
        {
            if (wh2[j]>maxval)
            {
                maxval=wh2[j];
                maxpos=j;
            }
        }
        tempval=wh2[i];
        wh2[i]=wh2[maxpos];
        wh2[maxpos]=tempval;
        tempval=wp2[i];
        wp2[i]=wp2[maxpos];
        wp2[maxpos]=tempval;
    }
    // 6. get the wave parameters
    Hmax=wh2[0];    Tmax=wp2[0];
	n1=(int)(nw/3);
    if (n1==0) n1=1;
    Hm=0;   Tz=0;
    H3=0;   T3=0;
    for (i=0;i<nw;i++)
    {
        Hm+=wh2[i];
        Tz+=wp2[i];
        if (i<n1)
        {
            H3+=wh2[i];
            T3+=wp2[i];
        }
    }
	Hm=Hm/(float)(nw);
	Tz=Tz/(float)(nw);
	H3=H3/(float)(n1);
	T3=T3/(float)(n1);
    if (BuoyWaveTestFlag==1)
    {
		//CCY printf("=======================\n");
		//CCY printf("------ zeruoup_wave ---\n");
		//CCY printf("zero-crossing : %d\n",nw);
		//CCY printf("Hmax : %lf, Tmax : %lf\n",Hmax,Tmax);
		//CCY printf("H3   : %lf, T3   : %lf\n",H3,T3);
		//CCY printf("Hm   : %lf, Tz	 : %lf\n",Hm,Tz);
		printf("+++++++++++++++++++++++++++\n");
    }
    return 0;
}

void filePointer(FILE *fileWriter)
{
	//fileWrite = fileWriter;
}
