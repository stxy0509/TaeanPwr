/******************************************************************************
 Copyright (C) 2011      Advanced Digital Chips Inc. 
						http://www.adc.co.kr
 Author : Software Team.

******************************************************************************/

#include "adstar.h"

U32 get_pll(int num)
{
	int m,n,o;
	int is,os;
	int pllcon;
	pllcon = *R_PLLCON;
	switch(num)
	{
	case 0:
		os = (pllcon>>4) & 0x7;
		is = pllcon & 0x7;
		break;
	case 1:
		os = (pllcon>>12) & 0x7;
		is = (pllcon>>8) & 0x7;

		break;
	default:
		return 0;
	}

	switch(is)
	{
	case 0:
		n=1;		break;
	case 1:
		n=2;		break;
	case 2:
		n=4;		break;
	case 3:
		n=6;		break;
	case 4:
		n=8;		break;
	case 5:
		n=10;		break;
	case 6:
		n=12;		break;
	case 7:
		n=16;		break;
	default:		return 0;
	}
	switch(os)
	{
	case 0:
		m=180;o=1;		break;
	case 1:
		m=120;o=1;		break;
	case 2:
		m=81;o=1;		break;
	case 3:
		m=48;o=1;		break;
	case 4:
		m=180;o=2;		break;
	case 5:
		m=81;o=3;		break;
	case 6:
		m=48;o=2;		break;
	case 7:
		m=48;o=4;		break;
	default:		return 0;
	}
	return  OSC_CLOCK*m/(n*o);
}

U32 get_ahb_clock()
{
	U8 pres;
    U32 ahbclock;
    U32 mainclock;//after prescaler
    if ((*R_CLKCON) & 0x1)//PLL?
    {
        mainclock = get_pll(0);
    }
    else
	{
        return OSC_CLOCK;
	}

    //compute prescaler
    pres = (*R_CLKCON)>>16;
	pres &=0xf;
    ahbclock = mainclock/(pres+1);
    return (U32)ahbclock;
}

U32 get_apb_clock()
{
    U32 ahbclock = get_ahb_clock();
    return ahbclock/2;
}

