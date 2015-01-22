#pragma once
#ifndef _fftpack_h
#define _fftpack_h

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
//#include <iostream>
//using namespace std;



#define MaxArrSize 1025
#define pi 3.14159
// Jan.,12,2007
// Macro, Introduced By, Press, W.H. et al. (1992)
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr;

// Jan.,12,2007
// 1-Dimensional FFT
// By Press,W.H., Teukolsky,S.A.,Vettering,W.T.,and Flanner,B.P.(1992)
//    Numerical recipes in C - the art of scientific computing 2nd edtion
//    Cambridge university press, pp.507~508
void four1(float *data,unsigned long nn,int isign);
void fft_1d(int nx,float *ar,float *ai,int isign);
// Jan.,15,2007
// 2-D FFT
// based on four1
void four2(int nx,int ny,float **ar,float **ai,int isign);
// Jan.,16,2007
// 3-D FFT
// base on four1
void four3(int n1,int n2,int n3,float ***ar,float ***ai,int isign);
#endif
