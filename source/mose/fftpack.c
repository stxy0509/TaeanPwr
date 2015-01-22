//#include "stdafx.h"
#include "fftpack.h"

// Jan.,12,2007
// 1-Dimensional FFT
// By Press,W.H., Teukolsky,S.A.,Vettering,W.T.,and Flanner,B.P.(1992)
//    Numerical recipes in C - the art of scientific computing 2nd edtion
//    Cambridge university press, pp.507~508
void four1(float *data,unsigned long nn,int isign)
{
	unsigned long n,mmax,m,j,istep,i;
	float wtemp,wr,wpr,wpi,wi,theta;
	float tempr,tempi;
	n=nn<<1;
	j=1;
	for(i=1;i<n;i+=2)
	{
		if (j>i)
		{
			SWAP(data[j],data[i]);
			SWAP(data[j+1],data[i+1]);
		}
		m=nn;
		while((m>=2)&&(j>m))	{
			j-=m;
			m=m>>1;
		}
		j+=m;
	}
	mmax=2;
	while(n>mmax)
	{
		istep=mmax<<1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr  =-2.0*wtemp*wtemp;
		wpi  =sin(theta);
		wr   =1.0;
		wi   =0.0;
		for(m=1;m<mmax;m+=2)
		{
			for(i=m;i<=n;i+=istep)
			{
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i]  +=tempr;
				data[i+1]+=tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi   +wr;
			wi=wi*wpr        +wtemp*wpi+wi;
		}
		mmax=istep;
	}
}

void fft_1d(int nx,float *ar,float *ai,int isign)
{
	int i;
	float *dat1;
	//dat1=new float [2*nx+1];
   dat1= (float *)malloc(sizeof(float)*(2*nx+1));
	dat1[0]=0.0;
	for(i=0;i<nx;i++)
	{
		dat1[2*i+1]=ar[i];
		dat1[2*i+2]=ai[i];
	}
	four1(dat1,(unsigned int)nx,isign);
	for(i=0;i<nx;i++)
	{
		if (isign==1)
		{
			ar[i]=dat1[2*i+1]/nx;
			ai[i]=dat1[2*i+2]/nx*(-1);
		}
		else
		{
			ar[i]=dat1[2*i+1];
			ai[i]=dat1[2*i+2];
		}
	}
	if (dat1!=NULL)
	{
	    free(dat1);		// delete [] dat1;
	    dat1=NULL;
	}
}

void four2(int nx,int ny,float **ar,float **ai,int isign)
{
	int i,j;
	float *dat1,*dat2;

	//dat1=new float [2*nx+1];
   dat1= (float *)malloc(sizeof(float)*(2*nx+1));
	for(i=0;i<ny;i++)
	{
		for(j=0;j<nx;j++)
		{
			dat1[2*j+1]=ar[i][j];
			dat1[2*j+2]=ai[i][j];	}
		four1(dat1,(unsigned int)nx,isign);
		for(j=0;j<nx;j++)
		{
			if (isign==1)
			{
				ar[i][j]=dat1[2*j+1]/nx;
				ai[i][j]=dat1[2*j+2]/nx;	}
			else
			{
				ar[i][j]=dat1[2*j+1];
				ai[i][j]=dat1[2*j+2];		}
		}
	}
	if (dat1!=NULL)
	{
       free(dat1);		//delete [] dat1;
	    dat1=NULL;
	 }

	//dat2=new float [2*ny+1];
   dat2= (float *)malloc(sizeof(float)*(2*ny+1));
	for(j=0;j<nx;j++)
	{
		for(i=0;i<ny;i++)
		{
			dat2[2*i+1]=ar[i][j];
			dat2[2*i+2]=ai[i][j];	}
		four1(dat2,(unsigned int)ny,isign);
		for(i=0;i<nx;i++)
		{
			if (isign==1)
			{
				ar[i][j]=dat2[2*i+1]/ny;
				ai[i][j]=dat2[2*i+2]/ny;	}
			else
			{
				ar[i][j]=dat2[2*i+1];
				ai[i][j]=dat2[2*i+2];		}
		}
	}
	if (dat2!=NULL)
	{
	    free(dat2);		//delete [] dat2;
	    dat2=NULL;
	 }
}

void four3(int n1,int n2,int n3,float ***ar,float ***ai,int isign)
{
	int i,j,k;
	float *dat1,*dat2,*dat3;

	//dat1=new float [2*n2+1];
   dat1= (float *)malloc(sizeof(float)*(2*n2+1));
	for(k=0;k<n3;k++)
	{
		for(i=0;i<n1;i++)
		{
			for(j=0;j<n2;j++)
			{
				dat1[2*j+1]=ar[i][j][k];
				dat1[2*j+2]=ai[i][j][k];
			}
			four1(dat1,(unsigned int)n2,isign);
			for(j=0;j<n2;j++)
			{
				if (isign==1)
				{
     				ar[i][j][k]=dat1[2*j+1]/n2;
					ai[i][j][k]=dat1[2*j+2]/n2;		}
				else
				{
					ar[i][j][k]=dat1[2*j+1];
					ai[i][j][k]=dat1[2*j+2];		}
				}
		}
	}
	if (dat1!=NULL)
	{
	    free(dat1);		//delete [] dat1;
	    dat1=NULL;
	 }

	//dat2=new float [2*n1+1];
   dat2= (float *)malloc(sizeof(float)*(2*n1+1));
	for(k=0;k<n3;k++)
	{
		for(j=0;j<n2;j++)
		{
			for(i=0;i<n1;i++)
			{
				dat2[2*i+1]=ar[i][j][k];
				dat2[2*i+2]=ai[i][j][k];	}
			four1(dat2,(unsigned int)n1,isign);
			for(i=0;i<n1;i++)
			{
				if (isign==1)
				{
					ar[i][j][k]=dat2[2*i+1]/n1;
					ai[i][j][k]=dat2[2*i+2]/n1;	}
				else
				{
					ar[i][j][k]=dat2[2*i+1];
					ai[i][j][k]=dat2[2*i+2];		}
			}
		}
	}
	if (dat2!=NULL)
	{
	    free(dat2);		//delete [] dat2;
	    dat2=NULL;
	 }

	//dat3=new float [2*n3+1];
   dat3= (float *)malloc(sizeof(float)*(2*n3+1));
	for(i=0;i<n1;i++)
	{
		for(j=0;j<n2;j++)
		{
			for(k=0;k<n3;k++)
			{
				dat3[2*k+1]=ar[i][j][k];
				dat3[2*k+2]=ai[i][j][k];	}
			four1(dat3,(unsigned int)n3,isign);
			for(k=0;k<n3;k++)
			{
				if (isign==1)
				{
					ar[i][j][k]=dat3[2*k+1]/n3;
					ai[i][j][k]=dat3[2*k+2]/n3;	}
				else
				{
					ar[i][j][k]=dat3[2*k+1];
					ai[i][j][k]=dat3[2*k+2];		}
			}
		}
	}

	if (dat3!=NULL)
	{
	    free(dat3);		//delete [] dat3;
	    dat3=NULL;
	 }

}



