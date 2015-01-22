#pragma once

#ifndef _utils_h
#define _utils_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#define pi 3.14159
#define g  9.81
float dREL(float om,float wd);
float dRELc(float w,float h,float ux,float uy,float an);

float atan_xy(float y,float x);
#define POWER(a,b) (a==0? 0:pow(a,b))
#define dabs(a) (a>=0? a:-a)

#if 0
void AllocTable(float ****table,int nt,int nx,int ny);
void FreeTable(float  ****table,int nt,int nx,int ny);
#endif
#endif


