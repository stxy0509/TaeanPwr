/* moseganalysis.cpp

	2007-10-15 first version

*/
//#include "stdafx.h"

#include <math.h> /* atan2 */
#include <stdio.h> /* sprintf */
#include "MoseGAnalysis.h"

/* four1 Numerical Recipes
   Replaces data[1..2*nn] by its discrete Fourier transform, if isign is input as 1; or replaces
   data[1..2*nn] by nn times its inverse discrete Fourier transform if isign is input as -1.
   data is a complex array of length nn or, equivalently, a real array of length 2*nn. nn MUST
   be an integer of power of 2 (this is not checked for!).
*/
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

/* all heights 5.2f
*/
void LimitH(float *h){
	if (*h>99.99F) *h=99.99F;
}

/* all periods 4.1f
*/
void LimitT(float *t){
	if (*t>99.9F) *t=99.9F;
}

/* MoseGAnalyseSpectrum collects east, north, vert, flag samples, calculates spectrum, analyses it and prepares message string
	one sample input and one spectrum message output at a time
	same arrays used for displacements and Fourier transforms
	present organisation:
		2 Hz sampling rate, half-hourly analysis, 3600 samples, first 14*256=3584 samples used, last 16 samples discarded,
		no overlap, Datawell tapered window
	i 'state index' = 0:FS*TIMEINTERVAL-1
	0..NSPECTRA*NPOINTS-1				store one 3d-displacement sample at a time
	NSPECTRA*NPOINTS-1					calculate Fourier transform
	NSPECTRA*NPOINTS..FS*TIMEINTERVAL-1	do nothing, if >0 then room for spreading process load in time
	0..NPOINTS2-1						output one bin of spectrum at a time
*/
//void MoseGAnalyseSpectrum(float *displacements, unsigned char flag, int i, char *cspc, char *cpeak, char *cmom)
void MoseGAnalyseSpectrum(float *displacements, unsigned char flag, int i, WAVE_DATA_COMMON *result)
{
	int j,jp;	//,nc;
	static short int spectrum(0);
	static short int gpsflag(0);
	static int nflag(0); /* total number of gps flags */
	float w,f,sumww,norm,
		a1,b1,m1,a2,b2,cos2,sin2,m2,n2,
		psd,dir,spread,check,skew,kurt,
		tp,psdp,dirp,spreadp,checkp,skewp,kurtp,
		df,m_2,m_1,m0,mm1,mm2,m3,m4,m21,
		hmo,t1,t3,tz,te,ti,tc,qp,
		teller,noemer;
	static float x[NPOINTS]; /* east */
	static float y[NPOINTS]; /* north */
	static float z[NPOINTS]; /* vert */
	static float cxx[NPOINTS2+1];
	static float cyy[NPOINTS2+1];
	static float czz[NPOINTS2+1];
	static float cyx[NPOINTS2+1];
	static float qzx[NPOINTS2+1];
	static float qzy[NPOINTS2+1];

	/* prepare NMEA string spectrum bin parameters, one bin at a time */
	if ((spectrum)&&(i>=0)&&(i<=NPOINTS2)){
		f=(float)i*FS/(float)NPOINTS;
		/* limit */
		if (f>9.9999F) f=9.9999F;
		if (cxx[i]>(float)9.9999E9) cxx[i]=(float)9.9999E9;
		if ((cxx[i]!=0.0F)&&(cxx[i]<(float)1.0000E-9)) cxx[i]=(float)1.0000E-9;
		cyy[i]=(float)fmod(cyy[i]+360.0F,360.0F);
		if (czz[i]> 99.0F)  czz[i]= 99.0F;
		if (cyx[i]> 9.999F) cyx[i]=  9.999F;
		if (qzx[i]> 99.99F) qzx[i]= 99.99F;
		if (qzx[i]<-99.99F) qzx[i]=-99.99F;
		if (qzy[i]> 99.99F) qzy[i]= 99.99F;
		if (qzy[i]<  0.0F)  qzy[i]=  0.0F;
#if 0
		nc=sprintf(cspc,"%1d,%06.4f,%09.4E",gpsflag,f,cxx[i]);
		cspc[nc-3]=cspc[nc-1]; /* default C++ exponent 3 digits, here 1 digit */
		nc=nc-2+sprintf(cspc+nc-2,",%03.0f,%02.0f,%05.3f,%06.2f,%05.2f",
			cyy[i],czz[i],cyx[i],qzx[i],qzy[i]); /* psd,dir,spread,check,skew,kurt */
#endif
		/* zero co-quad spectra, one bin at a time */
		cxx[i]=0.0;
		cyy[i]=0.0;
		czz[i]=0.0;
		cyx[i]=0.0;
		qzx[i]=0.0;
		qzy[i]=0.0;
		if (i==0) nflag=0;
		if (i==NPOINTS2){
			spectrum=0; /* spectrum output finfished */
#if 0
			sprintf(cpeak,""); /* while spectrum==1 cpeak and cmom also available */
			sprintf(cmom,"");
#endif
		}
	}
#if 0
	else {
		sprintf(cspc,"");
	}
#endif
	/* store samples in cyclic array */
/*
	if ((i>=0)&&(i<NPOINTS*NSPECTRA)){
		x[i%NPOINTS]=displacements[0];
		y[i%NPOINTS]=displacements[1];
		z[i%NPOINTS]=displacements[2];


		CString msg;
		msg.Format(L"i = d, %d.%d, %d.%d , %d.%d",i, (int)displacements[0], (int)((displacements[0]*1000) - displacements[0]),
											(int)displacements[1], (int)((displacements[1]*1000) - displacements[1]),
											(int)displacements[2], (int)((displacements[2]*1000) - displacements[2]));
		RETAILMSG( 1,(TEXT("[sumheave]-%s \r\n"), msg));

		if (flag) nflag++;
	}
*/
	for(int j = 0 ; j < 256; j++)
	{
		x[j]=displacements[0];
		y[j]=displacements[1];
		z[j]=displacements[2];
		displacements++;

	}


	/* calculate and add spectrum */
	if (((i%NPOINTS)==NPOINTS-1) && (i<NPOINTS*NSPECTRA))
	{
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
		}
	}
	/* analyse sum spectrum */
	if (/*i==NPOINTS*NSPECTRA-1*/ 1){
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
				dir   =(float)atan2(b1,-1.0*a1); /* atan2(-east/-north)=[-PI,+PI], wave direction from, 0=north, clockwise */
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
		/* find peak spectrum */
		psdp=-1.0F;
		for (j=0; j<=NPOINTS2; j++)
			if (cxx[j]>psdp){
				jp=j;
				psdp=cxx[j];
			}
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
		LimitT(&tp);
		if (psdp>(float)9.9999E9) psdp=(float)9.9999E9;
		if ((psdp!=0.0F)&&(psdp<(float)1.0000E-9)) psdp=(float)1.0000E-9;
		dirp=(float)fmod(dirp+360.0F,360.0F);
		if (spreadp>99.0F) spreadp  = 99.0F;
		if (checkp>  9.999F) checkp =  9.999F;
		if (skewp > 99.99F) skewp   = 99.99F;
		if (skewp <-99.99F) skewp   =-99.99F;
		if (kurtp > 99.99F) kurtp   = 99.99F;
		if (kurtp <  0.0F) kurtp    =  0.0F;
		if (nflag>1) gpsflag=1;
		else gpsflag=0;
		/* prepare NMEA string spectrum bin parameters peak */
#if 0
		nc=sprintf(cpeak,"%1d,",gpsflag);
		if (tp<9.995F) nc+=sprintf(cpeak+nc,"%4.2f,",tp); else nc+=sprintf(cpeak+nc,"%4.1f,",tp);
		nc+=sprintf(cpeak+nc,"%09.4E",psdp);
		cpeak[nc-3]=cpeak[nc-1]; /* default C++ exponent 3 digits, here 1 digit */
		nc+=-2+sprintf(cpeak+nc-2,",%03.0f,%02.0f,%05.3f,%06.2f,%05.2f",dirp,spreadp,checkp,skewp,kurtp);
#endif

// Get needs Data
		result->WaveDirection = dir;

		/* spectral moment parameters */
		df=FS/NPOINTS;
		m_2*=df;
		m_1*=df;
		m0 *=df;
		mm1*=df;
		mm2*=df;
		m3 *=df;
		m4 *=df;
		m21*=df;
		hmo=4.0F*(float)sqrt(m0);
		ti=0.0;
		te=0.0;
		qp=0.0;
		t1=0.0;
		tz=0.0;
		t3=0.0;
		tc=0.0;
		if ((m0*m0)!=0){
			ti=(float)sqrt(m_2/m0);
			te=m_1/m0;
			qp=2.0F*m21/(m0*m0);
		}
		if (mm1!=0){
			t1=m0/mm1;
		}
		if (mm2!=0.0){
			tz=(float)sqrt(m0/mm2);
		}
		if (m3!=0.0){
			t3=(float)sqrt(mm1/m3);
		}
		if (m4!=0.0){
			tc=(float)sqrt(mm2/m4);
		}
		/* limit */
		LimitH(&hmo);
		LimitT(&ti);
		LimitT(&te);
		LimitT(&t1);
		LimitT(&tz);
		LimitT(&t3);
		LimitT(&tc);
		if (qp>999.99F) qp=999.99F;
#if 0
		/* prepare NMEA string spectral moment parameters */
		nc=sprintf(cmom,"%1d,%05.2f,",gpsflag,hmo);
		if (ti<9.995F) nc+=sprintf(cmom+nc,"%4.2f,",ti); else nc+=sprintf(cmom+nc,"%4.1f,",ti);
		if (te<9.995F) nc+=sprintf(cmom+nc,"%4.2f,",te); else nc+=sprintf(cmom+nc,"%4.1f,",te);
		if (t1<9.995F) nc+=sprintf(cmom+nc,"%4.2f,",t1); else nc+=sprintf(cmom+nc,"%4.1f,",t1);
		if (tz<9.995F) nc+=sprintf(cmom+nc,"%4.2f,",tz); else nc+=sprintf(cmom+nc,"%4.1f,",tz);
		if (t3<9.995F) nc+=sprintf(cmom+nc,"%4.2f,",t3); else nc+=sprintf(cmom+nc,"%4.1f,",t3);
		if (tc<9.995F) nc+=sprintf(cmom+nc,"%4.2f,",tc); else nc+=sprintf(cmom+nc,"%4.1f,",tc);
		sprintf(cmom+nc,"%06.2f",qp);
#endif
		spectrum=1; /* spectrum available */
	}
}

/* StoreHighest stores nmax highest x inputs in xx, and associated y inputs in yy
	n number of x's input, may be larger than nmax
*/
void StoreHighest(unsigned int x, unsigned int *xx, int *n, int nmax, int *imin, unsigned int *xmin, unsigned int y, unsigned int *yy){
	int i;
	const unsigned int maxunsignedint=65535;

	/* append if array not full */
	if (*n<nmax){
		xx[*n]=x;
		yy[*n]=y;
		if (*n==nmax){
			/* locate minimum if array full */
			*xmin=maxunsignedint;
			for (i=0; i<nmax; i++)
				if (xx[i]<*xmin){
					*imin=i;
					*xmin=xx[i];
				}
		}
	/* replace minimum if array full and larger than minimum */
	} else {
		if (x>*xmin){
			xx[*imin]=x;
			yy[*imin]=y;
			/* locate new minimum */
			*xmin=maxunsignedint;
			for (i=0; i<nmax; i++)
				if (xx[i]<*xmin){
					*imin=i;
					*xmin=xx[i];
				}
		}
	}
	*n+=1;
}

/* shell sort-routine Numerical Recipes 8.1
	sorts in ascending order
	modified from float to unsigned integer, and added one array to be sorted in parallel
	no problems arise if a and b point to the same array
*/
void shell(unsigned long n, unsigned int a[], unsigned int b[]) /* modified */
{
	unsigned long i,j,inc;
	unsigned int v,w; /* modified */
	inc=1;
	do {
		inc *= 3;
		inc++;
	} while (inc <= n);
	do {
		inc /= 3;
		for (i=inc+1;i<=n;i++) {
			v=a[i];
			w=b[i]; /* modified */
			j=i;
			while (a[j-inc] > v) {
				a[j]=a[j-inc];
				b[j]=b[j-inc]; /* modified */
				j -= inc;
				if (j <= inc) break;
			}
			a[j]=v;
			b[j]=w; /* modified */
		}
	} while (inc > 1);
}

/* AverageHighest calculates average over 1/3- and 1/10-th highest values of 2 parallel arrays
	values zeroed if no waves or not enough stored waves
*/
void AverageHighest(unsigned int *x, unsigned int *y, int n, int nmax, float *x1_10, float *y1_10, float *x1_3, float *y1_3){

	int i,nn,n10,n3;
	float xsum(0.0),ysum(0.0);

	*x1_10=0.0F;
	*y1_10=0.0F;
	*x1_3 =0.0F;
	*y1_3 =0.0F;
	if (n<nmax) nn=n; else nn=nmax;
	shell((unsigned long)nn,x,y); /* sort x ascending, y sorted in parallel */
	n10=n/10;
	if ((n10>0)&&(n10<=nmax)){ /* (enough stored) waves? */
		for (i=nn-1; i>=nn-n10; i--){
			xsum+=x[i];
			ysum+=y[i];
		}
		*x1_10=xsum/(float)n10;
		*y1_10=ysum/(float)n10;
	}
	n3=n/3;
	if ((n3>0)&&(n3<=nmax)){ /* (enough stored) waves? */
		/* continue summation */
		for (i=nn-n10-1; i>=nn-n3; i--){
			xsum+=x[i];
			ysum+=y[i];
		}
		*x1_3=xsum/(float)n3;
		*y1_3=ysum/(float)n3;
	}
}

/* MoseGAnalyseTimeseries analyses heave timeseries
	user controls timeinterval length and synchronisation
	n>=1 number of new heave samples
	n=0 export waveparemeters and reset
	waveparameters 14: Nwaves, Nflags, Hs, Hmax, THmax, Tmax, Hav, Tav, H1/3, TH1/3, H1/10, TH1/10, T1/3, T1/10
	waveparameters array must be 16 elements long
	so far no: std dev, skewness, kurtosis of heights
*/

//void MoseGAnalyseTimeseries(float *heave, unsigned char *flag, int n, char *cheight, char* cperiod)
void MoseGAnalyseTimeseries(float *heave, unsigned char *flag, int n, WAVE_DATA_COMMON *result, BOOL type)
{
	int i;	//,nc;
	static int   nsample(0);	/* total number of samples */
	static int   nflag(0);		/* total number of gps flags */
	short int    gpsflag;
	static float heave_old(0.0);
	static float heave_min;
	static float heave_max;
	static float sumheave(0.0);
	static float sumsqrheave(0.0);
	static int   nwave(0);		/* total number of waves */
	static float hmax(0.0);		/* height of highest wave */
	static float thmax(0.0);	/* period of highest wave */
	static float tmax(0.0);		/* period of longest wave */
	static float t_zu(0.0);		/* zero-up cross period, in units of sampling time */
	static float sumtzu(0.0);
	static float sumh(0.0);
	static bool firstzeroup(true);
	const int nmax_highest=600; /* maximum number of heighest/longest waves, fraction*time*samplerate*1/2 = 1/3*1800*2*1/2 */
	int ihmin,itmin;
	unsigned int h,t,hmin,tmin;
	static int n_highest(0),n_longest(0);
	static unsigned int h_highest[nmax_highest]; /* H 1/n-th calculation */
	static unsigned int th_highest[nmax_highest]; /* for TH 1/n-th calculation */
	static unsigned int t_longest[nmax_highest]; /* for T 1/n-th calculation */
	float hs,hav,tav,h1_3,t1_3,th1_3,h1_10,t1_10,th1_10;

	for (i=0; i<n; i++){
		if(type == TRUE) {
//			if (flag != 0) nflag++;
			if (flag[i]!=0) nflag++;
		}
		sumheave+=heave[i];
//		-->
//		CString msg;
//		msg.Format(L"i-%d , sumheave-%d.%d, heave-%d.%d",i, (int)sumheave, (int)((sumheave*1000) - sumheave), (int)heave[i], (int)((heave[i]*1000)-heave[i]));
//		RETAILMSG( 1,(TEXT("[sumheave]-%s \r\n"), msg));

		sumsqrheave+=heave[i]*heave[i];
		t_zu+=1.0; /* add one sampling time interval to time from last zero-up cross to current sample time */
		if ((heave_old<0.0)&&(heave[i]>=0.0)){ /* zero-up crossing */
			if (firstzeroup==true){
				firstzeroup=false;
			} else {
				nwave +=1;
				t_zu+=-heave[i]/(heave[i]-heave_old); /* subtract time from interpolated zero-up cross to current sample time */
				t_zu/=FS; /* units seconds */
				sumh  +=heave_max-heave_min;
				sumtzu+=t_zu;
				if ((heave_max-heave_min)>hmax){
					hmax=heave_max-heave_min;
					thmax=t_zu;
				}
				if (t_zu>tmax) tmax=t_zu;
				h=(int)(1000.0*(heave_max-heave_min)); /* unit mm */
				t=(int)(100.0*t_zu); /* unit 10 ms */
				StoreHighest(h,h_highest,&n_highest,nmax_highest,&ihmin,&hmin,t,th_highest);
				StoreHighest(t,t_longest,&n_longest,nmax_highest,&itmin,&tmin,t,t_longest); /* last 2 args dummy */
			}
			heave_min=heave[i];
			heave_max=heave[i];
			t_zu=heave[i]/(heave[i]-heave_old); /* time from interpolated zero-up cross to current sample time */
		}
		if (heave[i]<heave_min) heave_min=heave[i];
		if (heave[i]>heave_max) heave_max=heave[i];
		heave_old=heave[i];
	}
	nsample+=n;

	n = 0; // for temp.... guess it...?

//	--> n value, heave_min, heave_max, sumheave, sumsqrheave, h, t, t_zu

	/* assign waveparemeters */
	if (n==0){
		if (nsample>1)
			hs=4.0F*(float)sqrt((sumsqrheave-sumheave*sumheave/(float)nsample)/((float)nsample-1.0F)); /* Hs */
		else
			hs=0.0F;
		if (nwave>0){
			hav=sumh/(float)nwave;		/* Hav average height of all waves */
			tav=sumtzu/(float)nwave;	/* Tav (=THav) average period of all waves */
		} else {
			hav=0.0;
			tav=0.0;
		}
		AverageHighest(h_highest,th_highest,n_highest,nmax_highest,&h1_10,&th1_10,&h1_3,&th1_3); /* H1/10 and TH1/10 average height and period of 1/10 highest waves, idem H1/3, TH1/3 */
		AverageHighest(t_longest,t_longest,n_longest,nmax_highest,&t1_10,&t1_10,&t1_3,&t1_3); /* T1/10 average period of 1/10 longest waves, idem T1/3 */
		h1_3/=1000.0F; /* mm to m */
		h1_10/=1000.0F;
		t1_3/=100.0F; /* 10 ms to s */
		th1_3/=100.0F;
		t1_10/=100.0F;
		th1_10/=100.0F;
		/* limit */
		if (nwave>999) nwave=999;
		LimitT(&hs);
		LimitT(&hav);
		LimitT(&hmax);
		LimitT(&h1_3);
		LimitT(&h1_10);
		/* prepare NMEA string heights message */
		if (nflag>0) gpsflag=1;
		else gpsflag=0;
#if 1
		printf("%1d,%03d,%05.2f,%05.2f,%05.2f,%05.2f,%05.2f",gpsflag,nwave,hs,hav,hmax,h1_3,h1_10);
#endif
		/* limit */
		LimitT(&tav);
		LimitT(&tmax);
		LimitT(&thmax);
		LimitT(&t1_3);
		LimitT(&th1_3);
		LimitT(&t1_10);
		LimitT(&th1_10);
		/* prepare NMEA string periods message */
#if 0
		nc=sprintf(cperiod,"%1d,",gpsflag);
		if (tav   <9.995F) nc+=sprintf(cperiod+nc,"%04.2f,",tav);    else nc+=sprintf(cperiod+nc,"%04.1f,",tav);
		if (tmax  <9.995F) nc+=sprintf(cperiod+nc,"%04.2f,",tmax);   else nc+=sprintf(cperiod+nc,"%04.1f,",tmax);
		if (thmax <9.995F) nc+=sprintf(cperiod+nc,"%04.2f,",thmax);  else nc+=sprintf(cperiod+nc,"%04.1f,",thmax);
		if (t1_3  <9.995F) nc+=sprintf(cperiod+nc,"%04.2f,",t1_3);   else nc+=sprintf(cperiod+nc,"%04.1f,",t1_3);
		if (th1_3 <9.995F) nc+=sprintf(cperiod+nc,"%04.2f,",th1_3);  else nc+=sprintf(cperiod+nc,"%04.1f,",th1_3);
		if (t1_10 <9.995F) nc+=sprintf(cperiod+nc,"%04.2f,",t1_10);  else nc+=sprintf(cperiod+nc,"%04.1f,",t1_10);
		if (th1_10<9.995F) nc+=sprintf(cperiod+nc,"%04.2f", th1_10); else nc+=sprintf(cperiod+nc,"%04.1f" ,th1_10);
#endif

// Get needs Data
		result->Hs = hs;
		result->Hmax = hmax;		//(최대파고)
//		result->THmax = thmax;		//(최대파주기)
		result->Tmax = tmax;		//(최대주기)
		result->Hav = hav;			//(높이평균)
		result->Tav = tav;			//(주기평균)
		result->H1_3 = t1_3;
		result->TH1_3 = th1_3;
		result->H1_10 = h1_10;
		result->TH1_10 = th1_10;
		result->T1_3 = t1_3;
		result->T1_10 = t1_10;

		/* reset */
		firstzeroup=true;
		nsample=0;
		nflag=0;
		nwave=0;
		sumheave=0.0;
		sumsqrheave=0.0;
		hmax=0.0;
		tmax=0.0;
		sumh=0.0;
		sumtzu=0.0;
		n_highest=0;
		n_longest=0;
	} else {
		RETAILMSG( 1,(TEXT("[Nsample over 0 ->  \r\n")));

#if 0
		sprintf(cheight,"");
		sprintf(cperiod,"");
#endif
	}
}

