//#include "stdafx.h"
// for managed application
#include "utils.h"
// see a note of radar, on Feb,2,2006
float dREL(float om,float wd)
{
	float x,xn;
	float f,df;
	int itermx=50;
	int i;

	// 1) initial guess
	x=(float)(pow(om,2)/g);
	// 2) calculate xn
	for(i=0;i<itermx;i++)
	{
		f = (float)(pow(om,(float)(2))-g*x/wd*tanh(x));
		df=(float)(-g/wd*tanh(x)-g*x/wd*(1-pow(tanh(x),(float)(2))));
		xn=x-f/df;
		if (fabs(xn-x)<=1.e-5)
			break;
		else
			x=xn;
	}
	return(x/wd);
}

float dRELc(float w,float h,float ux,float uy,float an)
{
	float rat=1.e-5;
	int itermx=50;
	int i;
	float u;
//	float init2;
	float x,xn;
	float f,df;
	// u
	u=(float)(ux*cos(an)+uy*sin(an));
	if ((u<0)&&(abs(u)>sqrt(g*h)))	u=(float)(u*(-1.));
	// initial guess
	x=(float)((POWER(w,(float)(2))/(g+u))*h);
	x=(float)((POWER(w,(float)(2))/(g*tanh(x*h)+u))*h);
	//
	for(i=0;i<itermx;i++)
	{
		f =(float)(POWER(w-(x*u/h),(float)(2))-g*x*tanh(x)/h);
		df=(float)(-2.0*(w-(u/w*x))*(u/h)-g*tanh(x)/h-g*(1-POWER(tanh(x),(float)(2)))*x/h);
		xn=x-f/df;
		if (fabs(xn-x)<=rat)
			break;
		else
			x=xn;
	}
	return(x/h);
}

float atan_xy(float y,float x)
{
	if ((x==0)&&(y==0))
		return(0.0);
	else
		return (float)(atan2(y,x));
}

#if 0
void AllocTable(float ****table,int nt,int nx,int ny)
{
	int t,j,k;
	(*table)=new float**[nt];

	for(t=0;t<nt;t++)
		(*table)[t]=new float*[ny];
	for(t=0;t<nt;t++)
	{	for(j=0;j<ny;j++)
			(*table)[t][j]=new float [nx];
	}
	for(t=0;t<nt;t++)
	{
		for(j=0;j<ny;j++)
		{	for(k=0;k<nx;k++)
			(*table)[t][j][k]=0.0f;		}
	}
}

void FreeTable(float ****table,int nt,int nx,int ny)
{
	int t,j;
	for(t=0;t<nt;t++)
	{
		for(j=0;j<ny;j++)
			delete [] (*table)[t][j];
		delete [] (*table)[t];
	}
	delete [] (*table);
	(*table)=NULL;
}
#endif

